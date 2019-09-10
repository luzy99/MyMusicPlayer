#include "splashscreen.h"
#include<QProgressBar>
#include<QDebug>
#include<QString>
#include<QPalette>
SplashScreen::SplashScreen(QPixmap pixmap,QWidget*parent)
    :QSplashScreen (parent,pixmap)
{
    bar = new QProgressBar(this);
    QString str="QProgressBar"
                "{border: 2px solid grey;"
            "border-radius: 10px;}"
        "QProgressBar::chunk {"
            "background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(95,131,210, 255), stop:1 rgba(255,222,228, 255));}";

    bar->setStyleSheet(str);
    QFont font;
    font.setPointSize(15);
    font.setFamily("微软雅黑");
    QPalette pal(Qt::red);
    bar->setPalette(pal);
    bar->setFont(font);
    //设置进度条的位置
    bar->setGeometry(0,pixmap.height()/5*4,pixmap.width(),pixmap.height()/20);
    resize(pixmap.size());
}

SplashScreen::~SplashScreen()
{

}


void SplashScreen::setRange(int min, int max)
{
    bar->setRange(min, max);
}

void SplashScreen::updateProgress(int num)
{

    bar->setValue(num);

    repaint();
}

void SplashScreen::showProgressMessage(int total, const QString &msg)
{
    bar->setRange(0, total);
    showMessage(msg);
}


