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
    pixmapPath=":/icon/res/default_cover.ico";
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(rotate()));
    timer->setInterval(50);
    timer->start();
}

void diskWidget::rotate()
{
    if(count >= 60000)
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
        QSize size(2*i_radius, 2*i_radius);
        QImage tempimg(pixmapPath);
        if(tempimg.load(pixmapPath) == false)
        {
            qDebug() << "no image";
            tempimg.load(":/icon/res/default_cover.ico");
        }
        coverPic=QPixmap::fromImage(tempimg.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        this->resize(coverPic.size());

        QPixmap mask(size);
        QPainter painter2(&mask);
        painter2.setRenderHint(QPainter::Antialiasing);
        painter2.setRenderHint(QPainter::SmoothPixmapTransform);
        painter2.fillRect(0, 0, size.width(), size.height(), Qt::white);//白色矩形
        painter2.setBrush(Qt::black);//黑色圆形
        //保留黑色部分，蒙版为圆形
        painter2.drawEllipse(0, 0, size.width(), size.height());

        coverPic.setMask(mask);//图片蒙版
        this->setMask(coverPic.mask());//窗口蒙版
        paintFlag =1;
    }
    QPainter painter(this);
    painter.translate(i_radius, i_radius);
    painter.rotate(count * 0.006);
    painter.translate(i_radius*(-1), i_radius*(-1));
    painter.drawPixmap(0,0,coverPic);

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

