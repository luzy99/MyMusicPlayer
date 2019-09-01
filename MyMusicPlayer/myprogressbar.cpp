#include "myprogressbar.h"

MyProgressBar::MyProgressBar(QWidget *parent)
    : QWidget(parent),
      rotateAngle(0),
      persent(0)
{
    this->setFixedSize(200,200);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
}

//绘制圆形进度条
void MyProgressBar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    //计算转过的角度
    rotateAngle = 360*persent/100;

    //设置外框
    int side = qMin(width(), height());
    QRectF outRect(0, 0, side, side);
    QRectF inRect(20, 20, side-40, side-40);
    QString progressPercentage = QString("%1%").arg(QString::number(persent));

    //画底圆
    painter.setPen(Qt::NoPen);
    QPixmap background = QPixmap(":/icon/res/progressBackground.png");
    painter.drawPixmap(outRect,background,outRect);

    //画内弧
    QPixmap frontCircle = QPixmap(":/icon/res/progressFrontCircle.png");
    painter.setBrush(QBrush(frontCircle));
    painter.drawPie(outRect,(90-rotateAngle)*16, rotateAngle*16);

    //画文字
    QFont f = QFont("Comic Sans MS", 20, QFont::Bold);
    painter.setFont(f);
    painter.setPen(QColor(255,255,255));
    painter.drawText(inRect, Qt::AlignCenter, progressPercentage);
}

//进度的set函数
//进度更新后进行重绘事件
void MyProgressBar::setPersent(int value)
{
    persent = value;
    repaint();
}
