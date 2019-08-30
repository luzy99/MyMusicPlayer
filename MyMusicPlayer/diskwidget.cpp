#include <QPainter>
#include <QPaintEvent>
#include <QBitmap>
#include<QPixmap>
#include <QTimer>
#include <QDebug>
#include<QImage>
#include "diskWidget.h"
#include<QWidget>
#include<QLabel>

diskWidget::diskWidget(QWidget *parent, int r) : QWidget(parent)//输入圆的半径
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    count = 0;
    i_radius = r;
    d_rate = 1;
    paintFlag =0;
    pixmapPath=":/icon/res/default_cover.png";
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(rotate()));
    timer->setInterval(80);
    timer->start();
}

void diskWidget::rotate()
{
    if(count >= 120000)
    {
        count = 0;
    }
    count = (count + d_rate*300);
    update();
}

void diskWidget::rotateStop()
{
    timer->stop();
}

void diskWidget::rotateStart()
{
    timer->start();
}

void diskWidget::changePic(QString newPicPath)
{
    this->pixmapPath =newPicPath;
    paintFlag =0;
}

void diskWidget::paintEvent(QPaintEvent *event)
{
    //qDebug()<<"painting...";
    if(paintFlag==0)//设置蒙版将图片切成圆形
    {
        QSize size(1.2*i_radius, 1.2*i_radius);
        QImage tempimg(pixmapPath);
        if(tempimg.load(pixmapPath) == false)
        {
            qDebug() << "no image";
            tempimg.load(":/icon/res/disk.ico");
        }
        coverPic=QPixmap::fromImage(tempimg.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        //this->resize(coverPic.size());

        QPixmap mask(size);
        QPainter painter2(&mask);
        painter2.setRenderHint(QPainter::Antialiasing);
        painter2.setRenderHint(QPainter::SmoothPixmapTransform);
        painter2.fillRect(0, 0, size.width(), size.height(), Qt::white);//白色矩形
        painter2.setBrush(Qt::black);//黑色圆形
        //保留黑色部分，蒙版为圆形
        painter2.drawEllipse(0, 0, size.width(), size.height());

        coverPic.setMask(mask);//图片蒙版
        //this->setMask(coverPic.mask());//窗口蒙版
        paintFlag =1;
    }

    QPainter painter(this);
    QPixmap diskPic(":/icon/res/disk.png");
    diskPic=diskPic.scaled(2*i_radius,2*i_radius);
    painter.drawPixmap(0,0,diskPic);
    painter.translate(i_radius, i_radius);
    painter.rotate(count * 0.003);
    painter.translate(i_radius*(-1), i_radius*(-1));

    painter.drawPixmap(0,0,diskPic);
    painter.drawPixmap(0.4*i_radius,0.4*i_radius,coverPic);

    return QWidget::paintEvent(event);
}

void diskWidget::setRadius(int radius)
{
    i_radius = radius;
}

void diskWidget::setRate(double rate)
{
    d_rate = rate;
}

