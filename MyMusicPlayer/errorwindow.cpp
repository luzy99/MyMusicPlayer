#include "errorwindow.h"
#include <QDebug>
#include <QLabel>
#include <QBitmap>
#include <QPainter>

ErrorWindow::ErrorWindow(QString msg ,QWidget *parent) :
    QMainWindow(parent),
    message(msg)
{
    setFixedSize(320,200);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
//    setAttribute(Qt::WA_TranslucentBackground,true);
    setStyleSheet("background-color:black;"
                  "border-radius:15px;"
                  "color:rgb(255,192,203);");

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),10,10);
    setMask(bmp);

    QLabel *label1 = new QLabel(this);
    label1->setGeometry(0,140,320,27);
    label1->setText(message);
    label1->setAlignment(Qt::AlignHCenter);
    label1->setFont(QFont("微软雅黑", 12));

    QLabel *label2 = new QLabel(this);
    QPixmap pic(":/icon/res/error.png");
    label2->setPixmap(pic);
    label2->setScaledContents(true);
    label2->setFixedSize(60,60);
    label2->setGeometry(130, 54, 60, 60);


    animation = new QPropertyAnimation(this, "windowOpacity");
    connect(animation, SIGNAL(finished()), this, SLOT(close()));
}

ErrorWindow::~ErrorWindow()
{
}

void ErrorWindow::showInstantly()
{
    animation->setDuration(2000);
    animation->setStartValue(0.9);
    animation->setEndValue(0);
    animation->start();
}
