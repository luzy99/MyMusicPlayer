#include "AnimatedWallWG.h"
#include <QLayout>
#include <QEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <QMouseEvent>
#include <QLabel>

AnimatedWallWG *AnimatedWallWG::m_pInstance = NULL;

AnimatedWallWG::AnimatedWallWG(QWidget *parent) :
    QWidget(parent)
{
    //1. 初始化
    initVar();
    initWallpaper();
    initLRButton();
    m_pbtnwidget = new QWidget(this);
    initIndexButton();
    m_pbtnwidget->setWindowFlag(Qt::WindowStaysOnTopHint);
    m_pbtnwidget->setWindowFlag(Qt::Widget);

    this->setFixedHeight(fixedheight);

    this->setMinimumWidth(750);
    this->setMaximumWidth(1400);

    //2. 开启定时器8秒轮换一次
    m_idTimer = startTimer(8000);

    //3. 安装事件过滤器捕获窗口大小改变事件
    installEventFilter(this);
}

//初始化变量
void AnimatedWallWG::initVar()
{
    m_iWall = 0;
    m_iRaise = 0;
    m_idTimer = 0;
    m_bAnimation = true;
    m_cover=0;
}

//初始化背景墙
void AnimatedWallWG::initWallpaper()
{
    //创建COUNT个Wall并且设置背景图
    for (int i = 0; i < 6; i++)
    {
        m_pWall[i] = new QLabel(this);
        setWallpaper(m_pWall[i], i);
    }

    //默认索引为1的Wall置顶
    m_pWall[1]->raise();
    //m_pbtnwidget->raise();

    //为了好看暂时把没用的Wall先移走
    m_pWall[3]->move(3920, 0);
    m_pWall[4]->move(3920, 0);
    m_pWall[5]->move(3920, 0);

    m_pWall[0]->setObjectName("label0");
    m_pWall[1]->setObjectName("label1");
    m_pWall[2]->setObjectName("label2");
    m_pWall[3]->setObjectName("label3");
    m_pWall[4]->setObjectName("label4");
    m_pWall[5]->setObjectName("label5");

}


// 初始化左右两个点击按钮
void AnimatedWallWG::initLRButton()
{
    //创建两个移动按钮
    m_pLRButton[0] = new QPushButton(this);
    m_pLRButton[1] = new QPushButton(this);

    m_pLRButton[0]->setAttribute(Qt::WA_Hover,true);
    m_pLRButton[1]->setAttribute(Qt::WA_Hover,true);


    this->installEventFilter(m_pLRButton[0]);
    this->installEventFilter(m_pLRButton[1]);

    m_pLRButton[0]->hide();
    m_pLRButton[1]->hide();

    connect(m_pLRButton[0], SIGNAL(clicked()),
            this, SLOT(moveToRight()));
    connect(m_pLRButton[1], SIGNAL(clicked()),
            this, SLOT(moveToLeft()));

    //设置按钮的图标
    QString style1 = QString("QPushButton{border: 0px; background: url(:/Image/MoveL.png) center no-repeat;}"
                             "QPushButton:hover{background-color: rgba(0, 0, 0, 30%);}"
                             "QPushButton:pressed{background-color: rgba(0, 0, 0, 50%);}");
    m_pLRButton[0]->setStyleSheet(style1);

    QString style2 = QString("QPushButton{border: 0px; background: url(:/Image/MoveR.png) center no-repeat;}"
                             "QPushButton:hover{background-color: rgba(0, 0, 0, 30%);}"
                             "QPushButton:pressed{background-color: rgba(0, 0, 0, 50%);}");
    m_pLRButton[1]->setStyleSheet(style2);
}


//初始化下面的按钮
void AnimatedWallWG::initIndexButton()
{
    for(int index=0;index<6;index++)
    {
        m_pbtnwidget->setGeometry(0,fixedheight-20,width(),20);
        m_pIndexButton[index]=new QPushButton;
        m_pIndexButton[index]->setFixedSize(QSize(30,4));
        QString style1=QString("QPushButton:hover{border:0;background-color:rgba(255,255,255,90);}"
                              "QPushButton{border:0;background-color:rgba(0,0,0,90);}");
        m_pIndexButton[index]->setStyleSheet(style1);

    }
    connect(m_pIndexButton[0],SIGNAL(clicked()),
            this,SLOT(on_btnClicked0()));
    connect(m_pIndexButton[1],SIGNAL(clicked()),
            this,SLOT(on_btnClicked1()));
    connect(m_pIndexButton[2],SIGNAL(clicked()),
            this,SLOT(on_btnClicked2()));
    connect(m_pIndexButton[3],SIGNAL(clicked()),
            this,SLOT(on_btnClicked3()));
    connect(m_pIndexButton[4],SIGNAL(clicked()),
            this,SLOT(on_btnClicked4()));
    connect(m_pIndexButton[5],SIGNAL(clicked()),
            this,SLOT(on_btnClicked5()));


    QHBoxLayout *hy1=new QHBoxLayout;
    hy1->setAlignment(Qt::AlignHCenter);
    hy1->addSpacing(width()/2);
    hy1->addWidget(m_pIndexButton[0]);
    hy1->addSpacing(10);
    hy1->addWidget(m_pIndexButton[1]);
    hy1->addSpacing(10);
    hy1->addWidget(m_pIndexButton[2]);
    hy1->addSpacing(10);
    hy1->addWidget(m_pIndexButton[3]);
    hy1->addSpacing(10);
    hy1->addWidget(m_pIndexButton[4]);
    hy1->addSpacing(10);
    hy1->addWidget(m_pIndexButton[5]);
    hy1->addSpacing(width()/2);
    m_pbtnwidget->setStyleSheet("background-color:rgba(0,0,0,0);");
    //m_pbtnwidget->setStyleSheet("background-color:white;");
    m_pbtnwidget->setLayout(hy1);
    //m_pbtnwidget->show();

}


//设置动画移动的几个范围
void AnimatedWallWG::setWallRect()
{
    int w, h;

    if(width()<fixedwidth*2.72-60)
    {
        m_cover=fixedwidth*0.86-(width()-fixedwidth)/2;
    }
    else
    {
        m_cover=30;
    }

    //中间的矩形geometry
    h = fixedheight;
    w = fixedwidth ;
    m_rtWall[1] = QRect(fixedwidth*SCALE-m_cover, 0, w, h);

    //左边矩形的geometry、右边矩形的geometry
    h = fixedheight * SCALE;
    w = fixedwidth  * SCALE;
    m_rtWall[0] = QRect(m_rtWall[1].left() -w + m_cover, fixedheight - h, w, h);
    m_rtWall[2] = QRect(m_rtWall[1].right() - m_cover,    fixedheight - h, w,h);


    //从左到右变化过程中的五个geometry
    m_rtLChange[0] = QRect(0 - w,                          fixedheight - h, w, h);
    m_rtLChange[1] = QRect(m_rtLChange[0].x() + w,         fixedheight - h, w, h);
    m_rtLChange[2] = QRect(m_rtLChange[1].x() + w - m_cover, fixedheight - h, w, h);
    m_rtLChange[3] = QRect(m_rtLChange[2].x() + w - m_cover, fixedheight - h, w, h);
    m_rtLChange[4] = QRect(m_rtWall[2].x() + w - m_cover,    fixedheight - h, w, h);

    //从右到左变化过程中的五个geometry
    m_rtRChange[0] = QRect(width(),                        fixedheight - h, w, h);
    m_rtRChange[1] = QRect(m_rtRChange[0].x() - w,         fixedheight - h, w, h);
    m_rtRChange[2] = QRect(m_rtRChange[1].x() - w + m_cover, fixedheight - h, w, h);
    m_rtRChange[3] = QRect(m_rtRChange[2].x() - w + m_cover, fixedheight - h, w, h);
    m_rtRChange[4] = QRect(m_rtWall[0].x() - w + m_cover,    fixedheight - h, w, h);

    //根据索引得到当前活动的三个Wall的下标
    int i, j, k, l;
    wallIndex(true, m_iWall, i, j, k, l);

    m_pWall[i]->setGeometry(m_rtWall[0]);
    m_pWall[j]->setGeometry(m_rtWall[1]);
    m_pWall[k]->setGeometry(m_rtWall[2]);

    this->installEventFilter(m_pWall[0]);
    this->installEventFilter(m_pWall[1]);
    this->installEventFilter(m_pWall[2]);
    this->installEventFilter(m_pWall[3]);
    this->installEventFilter(m_pWall[4]);
    this->installEventFilter(m_pWall[5]);
}

//设置左右移动两个按钮的大小及位置
void AnimatedWallWG::setLRButtonRect()
{
    //设置按钮的大小
    int w = LRWIDTH;
    int h = height() * SCALE;
    m_pLRButton[0]->setGeometry(0,           height() - h, w, h);
    m_pLRButton[1]->setGeometry(width()-w, height() - h, w, h);
}

//设置墙纸
void AnimatedWallWG::setWallpaper(QLabel *pWall, int i)
{
    //if (pWall == NULL) return;

    QString style = QString("QLabel{border:100px;}"
                            "QLabel:hover{background-color: rgba(255, 255, 255, 30%);}");
    QString index=index.number(i+1);
    QString path=":/providedPosts/res/"+index+".JPG";
    pWall->setFixedHeight(fixedheight);
    pWall->setStyleSheet(style);
    pWall->setPixmap(QPixmap(path).scaled(fixedwidth,fixedheight));

//    qDebug()<<pWall;
}

//通过索引得到相对应的控件的下标
void AnimatedWallWG::wallIndex(bool b, int n, int &i, int &j, int &k, int &l)
{
    switch (n)
    {
    case 0: i = 0; j = 1; k = 2; l = b ? 3 : 5; break;
    case 1: i = 1; j = 2; k = 3; l = b ? 4 : 0; break;
    case 2: i = 2; j = 3; k = 4; l = b ? 5 : 1; break;
    case 3: i = 3; j = 4; k = 5; l = b ? 0 : 2; break;
    case 4: i = 4; j = 5; k = 0; l = b ? 1 : 3; break;
    case 5: i = 5; j = 0; k = 1; l = b ? 2 : 4; break;
    }
}

//从右往左移动
void AnimatedWallWG::moveToLeft()
{
    //判图片断当前是否可以轮换图片
//    if (!m_bAnimation) return;
//    m_bAnimation = false;

    //根据当前索引获取左、中、右、待出现的控件
    int i, j, k, l;
    wallIndex(true, m_iWall, i, j, k, l);
    m_iRaise = k;

    //创建动画
    QPropertyAnimation *pAnimation[4];

    //左
    pAnimation[0] = new QPropertyAnimation(m_pWall[i], "geometry");
    connect(pAnimation[0], SIGNAL(finished()),
            pAnimation[0], SLOT(deleteLater()));
    pAnimation[0]->setDuration(DURATION);
    pAnimation[0]->setEasingCurve(EASINGCURVE);
    pAnimation[0]->setKeyValueAt(0, m_rtWall[0]);
    pAnimation[0]->setKeyValueAt(EASINGTIME, m_rtRChange[3]);
    pAnimation[0]->setKeyValueAt(1, m_rtRChange[4]);

    //中
    pAnimation[1] = new QPropertyAnimation(m_pWall[j], "geometry");
    connect(pAnimation[1], SIGNAL(finished()),
            pAnimation[1], SLOT(deleteLater()));
    pAnimation[1]->setDuration(DURATION);
    pAnimation[1]->setEasingCurve(EASINGCURVE);
    pAnimation[1]->setKeyValueAt(0, m_rtWall[1]);
    pAnimation[1]->setKeyValueAt(EASINGTIME, m_rtRChange[2]);
    pAnimation[1]->setKeyValueAt(1, m_rtWall[0]);

    //右
    pAnimation[2] = new QPropertyAnimation(m_pWall[k], "geometry");
    connect(pAnimation[2], SIGNAL(finished()),
            pAnimation[2], SLOT(deleteLater()));
    connect(pAnimation[2], SIGNAL(valueChanged(QVariant)), this, SLOT(onValueChanged(QVariant)));
    pAnimation[2]->setDuration(DURATION);
    pAnimation[2]->setEasingCurve(EASINGCURVE);
    pAnimation[2]->setKeyValueAt(0, m_rtWall[2]);
    pAnimation[2]->setKeyValueAt(EASINGTIME, m_rtRChange[1]);
    pAnimation[2]->setKeyValueAt(1, m_rtWall[1]);

    //待出现
    pAnimation[3] = new QPropertyAnimation(m_pWall[l], "geometry");
    connect(pAnimation[3], SIGNAL(finished()),
            pAnimation[3], SLOT(deleteLater()));
    pAnimation[3]->setDuration(DURATION);
    pAnimation[3]->setEasingCurve(EASINGCURVE);
    pAnimation[3]->setKeyValueAt(0, m_rtRChange[0]);
    pAnimation[3]->setKeyValueAt(EASINGTIME, m_rtRChange[0]);
    pAnimation[3]->setKeyValueAt(1, m_rtWall[2]);

    //组
    QParallelAnimationGroup *pGroup = new QParallelAnimationGroup(this);
    connect(pGroup, SIGNAL(finished()), pGroup, SLOT(deleteLater()));
    connect(pGroup, SIGNAL(finished()), this, SLOT(finished()));
    pGroup->addAnimation(pAnimation[0]);
    pGroup->addAnimation(pAnimation[1]);
    pGroup->addAnimation(pAnimation[2]);
    pGroup->addAnimation(pAnimation[3]);
    pGroup->start();

    //序号++
    if (++m_iWall >= 6)
    {
        m_iWall = 0;
    }
}

//从左往右移动
void AnimatedWallWG::moveToRight()
{
    //判图片断当前是否可以轮换图片
//    if (!m_bAnimation) return;
//    m_bAnimation = false;

    //根据当前索引获取左、中、右、待出现的控件
    int i, j, k, l;
    wallIndex(false, m_iWall, i, j, k, l);
    m_iRaise = i;

    //创建动画
    QPropertyAnimation *pAnimation[4];

    //左
    pAnimation[0] = new QPropertyAnimation(m_pWall[i], "geometry");
    connect(pAnimation[0], SIGNAL(finished()),
            pAnimation[0], SLOT(deleteLater()));
    connect(pAnimation[0], SIGNAL(valueChanged(QVariant)),
            this, SLOT(onValueChanged(QVariant)));
    pAnimation[0]->setDuration(DURATION);
    pAnimation[0]->setEasingCurve(EASINGCURVE);
    pAnimation[0]->setKeyValueAt(0, m_rtWall[0]);
    pAnimation[0]->setKeyValueAt(EASINGTIME, m_rtLChange[1]);
    pAnimation[0]->setKeyValueAt(1, m_rtWall[1]);

    //中
    pAnimation[1] = new QPropertyAnimation(m_pWall[j], "geometry");
    connect(pAnimation[1], SIGNAL(finished()),
            pAnimation[1], SLOT(deleteLater()));
    pAnimation[1]->setDuration(DURATION);
    pAnimation[1]->setEasingCurve(EASINGCURVE);
    pAnimation[1]->setKeyValueAt(0, m_rtWall[1]);
    pAnimation[1]->setKeyValueAt(EASINGTIME, m_rtLChange[2]);
    pAnimation[1]->setKeyValueAt(1, m_rtWall[2]);

    //右
    pAnimation[2] = new QPropertyAnimation(m_pWall[k], "geometry");
    connect(pAnimation[2], SIGNAL(finished()),
            pAnimation[2], SLOT(deleteLater()));
    pAnimation[2]->setDuration(DURATION);
    pAnimation[2]->setEasingCurve(EASINGCURVE);
    pAnimation[2]->setKeyValueAt(0, m_rtWall[2]);
    pAnimation[2]->setKeyValueAt(EASINGTIME, m_rtLChange[3]);
    pAnimation[2]->setKeyValueAt(1, m_rtLChange[4]);

    //待出现
    pAnimation[3] = new QPropertyAnimation(m_pWall[l], "geometry");
    connect(pAnimation[3], SIGNAL(finished()),
            pAnimation[3], SLOT(deleteLater()));
    pAnimation[3]->setDuration(DURATION);
    pAnimation[3]->setEasingCurve(EASINGCURVE);
    pAnimation[3]->setKeyValueAt(0, m_rtLChange[0]);
    pAnimation[3]->setKeyValueAt(EASINGTIME, m_rtLChange[0]);
    pAnimation[3]->setKeyValueAt(1, m_rtWall[0]);

    //组
    QParallelAnimationGroup *pGroup = new QParallelAnimationGroup(this);
    connect(pGroup, SIGNAL(finished()),
            pGroup, SLOT(deleteLater()));
    connect(pGroup, SIGNAL(finished()),
            this, SLOT(finished()));
    pGroup->addAnimation(pAnimation[0]);
    pGroup->addAnimation(pAnimation[1]);
    pGroup->addAnimation(pAnimation[2]);
    pGroup->addAnimation(pAnimation[3]);
    pGroup->start();

    //序号--
    if (--m_iWall < 0)
    {
        m_iWall = 6 - 1;
    }
}


//根据时间设置中间控件Raise
void AnimatedWallWG::onValueChanged(QVariant variant)
{
    Q_UNUSED(variant);
    QPropertyAnimation *pAnimation = qobject_cast<QPropertyAnimation *>(sender());
    if (pAnimation == NULL)
    {
        return;
    }

    if (pAnimation->currentTime() > DURATION * EASINGTIME)
    {
        m_pLRButton[0]->raise();
        m_pLRButton[1]->raise();
        m_pWall[m_iRaise]->raise();
        m_pbtnwidget->raise();
    }
}

void AnimatedWallWG::on_btnClicked0()
{
    int pre;
    int cur;
    int next;
    int other;
    wallIndex(true, m_iWall, pre,cur, next, other);
    switch(cur)
    {
    case 0:
        break;
    case 1:
        moveToRight();
        break;
    case 2:
        moveToRight();
        moveToRight();
        break;
    case 3:
        moveToRight();
        moveToRight();
        moveToRight();
        break;
    case 4:
        moveToLeft();
        moveToLeft();
        break;
    case 5:
        moveToLeft();
        break;
    }
}

void AnimatedWallWG::on_btnClicked1()
{
    int pre;
    int cur;
    int next;
    int other;
    wallIndex(true, m_iWall, pre,cur, next, other);
    switch(cur)
    {
    case 1:
        break;
    case 2:
        moveToRight();
        break;
    case 3:
        moveToRight();
        moveToRight();
        break;
    case 4:
        moveToRight();
        moveToRight();
        moveToRight();
        break;
    case 5:
        moveToLeft();
        moveToLeft();
        break;
    case 0:
        moveToLeft();
        break;
    }
}

void AnimatedWallWG::on_btnClicked2()
{
    int pre;
    int cur;
    int next;
    int other;
    wallIndex(true, m_iWall, pre,cur, next, other);
    switch(cur)
    {
    case 2:
        break;
    case 3:
        moveToRight();
        break;
    case 4:
        moveToRight();
        moveToRight();
        break;
    case 5:
        moveToRight();
        moveToRight();
        moveToRight();
        break;
    case 0:
        moveToLeft();
        moveToLeft();
        break;
    case 1:
        moveToLeft();
        break;
    }
}

void AnimatedWallWG::on_btnClicked3()
{
    int pre;
    int cur;
    int next;
    int other;
    wallIndex(true, m_iWall, pre,cur, next, other);
    switch(cur)
    {
    case 3:
        break;
    case 4:
        moveToRight();
        break;
    case 5:
        moveToRight();
        moveToRight();
        break;
    case 0:
        moveToRight();
        moveToRight();
        moveToRight();
        break;
    case 1:
        moveToLeft();
        moveToLeft();
        break;
    case 2:
        moveToLeft();
        break;
    }
}

void AnimatedWallWG::on_btnClicked4()
{
    int pre;
    int cur;
    int next;
    int other;
    wallIndex(true, m_iWall, pre,cur, next, other);
    switch(cur)
    {
    case 4:
        break;
    case 5:
        moveToRight();
        break;
    case 0:
        moveToRight();
        moveToRight();
        break;
    case 1:
        moveToRight();
        moveToRight();
        moveToRight();
        break;
    case 2:
        moveToLeft();
        moveToLeft();
        break;
    case 3:
        moveToLeft();
        break;
    }
}

void AnimatedWallWG::on_btnClicked5()
{
    int pre;
    int cur;
    int next;
    int other;
    wallIndex(true, m_iWall, pre,cur, next, other);
    switch(cur)
    {
    case 5:
        break;
    case 0:
        moveToRight();
        break;
    case 1:
        moveToRight();
        moveToRight();
        break;
    case 2:
        moveToRight();
        moveToRight();
        moveToRight();
        break;
    case 3:
        moveToLeft();
        moveToLeft();
        break;
    case 4:
        moveToLeft();
        break;
    }
}


//事件过滤器
bool AnimatedWallWG::eventFilter(QObject *watched, QEvent *event)
{
    switch ((int)event->type())
    {
    case QEvent::Resize:
        //qDebug()<<"resize";
        //qDebug()<<this->width();
        setWallRect();
        setLRButtonRect();
        initIndexButton();
        break;

    case QEvent::Enter:
        // qDebug()<<"enter";
        m_pLRButton[0]->show();
        m_pLRButton[1]->show();
        break;

    case QEvent::Leave:
        //qDebug()<<"leave";
        m_pLRButton[0]->hide();
        m_pLRButton[1]->hide();
        break;
    }

        //    case QMouseEvent::MouseButtonDblClick:
        //        int i, j, k, l;//0,1,2
        //        wallIndex(true, m_iWall, i, j, k, l);
        //        qDebug()<<i;
        //        qDebug()<<k;
        //        qDebug()<<watched->objectName();
        //        if(watched->objectName()==m_pWall[i]->objectName())
        //        {
        //            qDebug()<<"右";
        //            moveToLeft();
        //        }
        //        if(watched->objectName()==m_pWall[k]->objectName())
        //        {
        //            qDebug()<<"左";
        //            moveToRight();
        //        }
        //        break;

    return QWidget::eventFilter(watched, event);
}



//计时器
void AnimatedWallWG::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != m_idTimer)
    {
        return;
    }

    //默认从右到左
    moveToLeft();
}


// 画背景色
void AnimatedWallWG::paintEvent(QPaintEvent *event)
{
    QPainter paint(this);
    paint.setPen(Qt::NoPen);
    paint.setBrush(QBrush(QColor(250, 250, 252)));
    paint.drawRect(rect());

    QWidget::paintEvent(event);
}
