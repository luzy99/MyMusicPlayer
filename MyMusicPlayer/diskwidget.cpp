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

diskWidget::diskWidget(QWidget *parent, int r) : QWidget(parent)
{
    this->setAttribute(Qt::WA_TranslucentBackground);
    count = 0;
    i_radius = r;
    d_rate = 1;
    this->PixmapToRound(":/icon/res/default_cover.ico");
    timer = new QTimer;
    connect(timer, SIGNAL(timeout()), this, SLOT(rotate()));
    timer->setInterval(50);
    timer->start();
}

void diskWidget::rotate()
{
    if(count >= 30000)
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
    this->PixmapToRound(newPicPath);
}

void diskWidget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.translate(i_radius, i_radius);
    painter.rotate(count * 0.012);
    painter.translate(i_radius*(-1), i_radius*(-1));
    painter.drawPixmap(0,0,fitpixmap_userIcon);

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

void diskWidget::PixmapToRound(QString pixmapPath)
{
    QSize size(2*i_radius, 2*i_radius);

    QImage tempimg(pixmapPath);
//    if(tempimg.load(pixmapPath) == false)
//    {
//        qDebug() << "no image";
//        pixmap_userIcon=new QPixmap(":/icon/res/default_cover.ico");
//    }
//    else
//    {
//        pixmap_userIcon=new QPixmap(pixmapPath);
//    }

    fitpixmap_userIcon=QPixmap::fromImage(tempimg.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    this->resize(fitpixmap_userIcon.size());

    QBitmap mask(size);
    QPainter painter(&mask);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.fillRect(0, 0, size.width(), size.height(), Qt::white);//白色矩形底板
    painter.setBrush(Qt::black);//圆底板黑色

    painter.drawEllipse(0, 0, size.width(), size.height());

    fitpixmap_userIcon.setMask(mask);//mask是圆形，所以图为圆形
    this->setMask(fitpixmap_userIcon.mask());
}
