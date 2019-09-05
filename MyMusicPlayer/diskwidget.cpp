#include "diskWidget.h"

diskWidget::diskWidget(QWidget *parent, int r)
    : QWidget(parent)//输入圆的半径
{
    this->setAttribute(Qt::WA_TranslucentBackground);//设置窗口透明
    //this->resize(parent->width(),2*r+80);
    count = 0;
    needleCount=0;
    i_radius = r;
    d_rate = 1;
    paintFlag =0;
    pixmapPath=":/icon/res/disk.png";//默认图片
    diskPic.load(":/icon/res/disk.png");//加载唱片图
    needlePic.load(":/icon/res/ic_needle.png");//加载唱针图
    timer = new QTimer;//初始化计时器
    connect(timer, SIGNAL(timeout()),
            this, SLOT(rotate()));
    timer->setInterval(30);
    timer->start();

    stopTimer = new QTimer;//初始化暂停动画计时器
    connect(stopTimer, SIGNAL(timeout()),
            this, SLOT(rotate()));
    stopTimer->setInterval(30);

    needleFlag =0;
}

void diskWidget::rotate()
{
    if(count >= 120000)
    {
        count = 0;
    }
    count = (count + d_rate*300);
    switch(animiMode)
    {
    case 1://播放
        if(needleCount < 20)
        {
            needleCount += 1;
        }
        break;
    case 2://暂停
        if(needleCount >=0)
        {
            needleCount -= 1;
        }
        else
        {
            stopTimer->stop();
        }
        break;
    case 3://切歌
        if(needleFlag==0)//移开动画
        {
            if(needleCount >= 0)
            {
                needleCount -= 1;
            }
            else
            {
                needleFlag=1;
            }
        }
        else//移入动画
        {
            if(needleCount<20)
            {
                needleCount+=1;
            }
            else
            {
                needleFlag=0;
                animiMode=1;
            }
        }
        break;
    default:
        break;
    }

    update();
}

void diskWidget::rotateStop()
{
    animiMode=2;
    timer->stop();
    stopTimer->start();
}

void diskWidget::rotateStart()
{
    stopTimer->stop();
    timer->start();
    animiMode=1;
}

void diskWidget::nextSong()
{
    if(animiMode==2)
    {
        needleCount=0;
        animiMode=3;
        timer->start();
        stopTimer->stop();
        needleFlag=1;
    }
    else
    {
        stopTimer->stop();
        animiMode=3;
        needleFlag=0;
        //needleCount=20;
    }
}

void diskWidget::changePic(QString newPicPath)
{
    this->pixmapPath =newPicPath;
    paintFlag =0;
    nextSong();
}

void diskWidget::paintEvent(QPaintEvent *event)
{
    //qDebug()<<"painting...";
    if(paintFlag==0)//设置蒙版，将图片切成圆形（首次加载时执行）
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
    //旋转变换，重新绘制
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    painter.translate(5, 20);//移动坐标原点（留出边框位置）

    //绘制背景
    QColor backColor(200,200,200);
    backColor.setAlphaF(0.7);
    painter.setBrush(backColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(-5,-5,i_radius*2+10,i_radius*2+10);
    //this->resize(i_radius*2+20,i_radius*2+20);

    //绘制旋转唱片
    painter.save();//保存配置
    diskPic=diskPic.scaled(2*i_radius,2*i_radius);
    painter.translate(i_radius, i_radius);
    painter.rotate(count * 0.003);
    painter.translate(i_radius*(-1), i_radius*(-1));

    painter.drawPixmap(0,0,diskPic);
    painter.drawPixmap(0.4*i_radius,0.4*i_radius,coverPic);
    painter.restore();//恢复配置

    //绘制唱针
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);
    needlePic=needlePic.scaled(1.8*i_radius,1.8*i_radius);
    painter.translate(0.9*i_radius, -0.8*i_radius);//初始位置
    painter.translate(0.9*i_radius, 0.9*i_radius);
    painter.rotate(needleCount*1.5-60);//-30~-60
    painter.translate(-0.9*i_radius, -0.9*i_radius);

    painter.drawPixmap(0,0,needlePic);

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

