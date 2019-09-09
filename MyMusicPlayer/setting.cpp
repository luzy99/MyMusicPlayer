#include <QDir>
#include <QSettings>
#include "setting.h"
#include <ctime>
#include <QMediaPlayer>
#include <QBitmap>
#include <QPainter>
#include"musicplaybar.h"

timerStop::timerStop(QWidget *parent)
    :QWidget(parent)
{
    timerOffset=0;
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowOpacity(0.9);
    this->setFixedSize(325,220);
    this->setStyleSheet("background: rgb(25,25,25);");
    //绘制圆角窗口
     QBitmap bmp(this->size());
     bmp.fill();
     QPainter p(&bmp);
     p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
     p.setPen(Qt::NoPen);
     p.setBrush(Qt::black);
     p.drawRoundedRect(bmp.rect(),2,2);
     setMask(bmp);

    setAutoFillBackground(false);
    setFixedSize(300,200);
    setGeometry(600,600,50,150);
    setWindowFlags(Qt::WindowStaysOnTopHint);
    createLineEditBoxes();//创建延后关闭组件
    createDoubleSpinBoxes();//创建定时关闭组件
    delayedTime->setStyleSheet("QGroupBox{border:none}");
    delayedTime->setWindowFlags(Qt::FramelessWindowHint);
    assignTime->setWindowFlags(Qt::FramelessWindowHint);
    assignTime->setStyleSheet("QGroupBox{border:none}");
    QVBoxLayout *layout=new QVBoxLayout;//布局
    layout->addWidget(delayedTime);
    layout->addWidget(assignTime);

    ok_btn=new QPushButton;
    ok_btn->setText("确定");
    //设置字体颜色

    //ok_btn->setWindowFlags(Qt::FramelessWindowHint);
    ok_btn->setStyleSheet("QPushButton{background-color:rgba(95,131,210,75%);"
                          "color: white; border-radius: 2px;}"
                          "QPushButton:hover{background-color:rgba(82,113,180,75%);"
                          "color: white; border-radius: 2px;}");
    ok_btn->setFixedSize(80,40);
//    ok_btn->setStyleSheet("QPushButton{background-color:rgb(95,131,210)}");
//    ok_btn->setStyleSheet("QPushButton{color:white}");
    QHBoxLayout * qhb=new QHBoxLayout;
    qhb->addSpacing(50);
    qhb->addWidget(ok_btn);
    qhb->addSpacing(50);
    layout->addLayout(qhb);

    setLayout(layout);

    connect(ok_btn,SIGNAL(clicked()),
            this,SLOT(on_ok_btn_click()));
}

timerStop::~timerStop()
{
   timerOffset=0;
}

void timerStop::createDoubleSpinBoxes()
{
    QPalette pl;
    pl.setColor(QPalette::WindowText,Qt::white);

    assignTime=new QGroupBox();
    assigned=new QCheckBox("停止于");
    assigned->setPalette(pl);
    hour=new QSpinBox;
    hour->setPalette(pl);
    hour->setRange(0,23);
    hour->setSingleStep(1);
    hour->setValue(0);
    hour->setStyleSheet("color:white");
    QLabel *l1=new QLabel(tr("时"));
    l1->setPalette(pl);
    min=new QSpinBox;
    min->setRange(0,59);
    min->setSingleStep(1);
    min->setValue(15);
    min->setPalette(pl);
    min->setStyleSheet("color:white");
    QLabel *l2=new QLabel(tr("分"));
    l2->setPalette(pl);
    QHBoxLayout *HBV=new QHBoxLayout;
    HBV->addWidget(assigned);
    HBV->addWidget(hour);
    HBV->addWidget(l1);
    HBV->addWidget(min);
    HBV->addWidget(l2);
    assignTime->setLayout(HBV);
}

void timerStop::createLineEditBoxes()
{
    delayedTime=new QGroupBox();

    QPalette pl;
    pl.setColor(QPalette::WindowText,Qt::white);

    this->setWindowFlags(Qt::FramelessWindowHint);
    delayed=new QCheckBox(tr("当前时间延后"));
    delayed->setPalette(pl);
    line=new QLineEdit;
    line->setPlaceholderText("输入整数");
    line->setPalette(pl);
    line->setStyleSheet("color:white");
    QLabel *l1=new QLabel(tr("分钟"));
    l1->setPalette(pl);
    QHBoxLayout * HBV=new QHBoxLayout;
    HBV->addWidget(delayed);
    HBV->addWidget(line);
    HBV->addWidget(l1);
    delayedTime->setLayout(HBV);
}

qint64 timerStop::getTimer()
{
    qint64 t;
    if(delayed->isChecked())
    {
        t=line->text().toInt()*1000*60;
     }
    else if(assigned->isChecked())
    {
        int h=QTime::currentTime().toString("HH").toInt();
        int m=QTime::currentTime().toString("mm").toInt();
        int s=QTime::currentTime().toString("ss").toInt();
        int ah=hour->text().toInt();
        int am=min->text().toInt();
        t=ah*60*60*1000+am*60*1000-h*3600*1000-m*60*1000-s*1000;
    }
    else
    {
        t=0;
    }
    return t;
}

void timerStop::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos();
    windowsStartPoint = this->pos();
}
void timerStop::mouseMoveEvent(QMouseEvent *event)
{
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}

void timerStop::on_ok_btn_click()
{
    timerOffset=getTimer();
    if(timerOffset==0)
    {
        close();
    }
    else
    {
        close();
        m_timer1.start(timerOffset);
    }

}



