#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "titlebar.h"
#include "songinfo.h"
#include "songinfoshow.h"
#include <QVBoxLayout>

#include<QDebug>
//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#include"audiotag.h"
#endif
/*
 * 205 10 10 深红
 * 240 12 16 选中红色
 * 255 255 255 白色
 * 219 208 208 深灰
 * 244 239 239 浅灰
 */
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //Qt::FramelessWindowHint设置窗口标志为无边框，而Qt::WindowStaysOnTopHint使窗口位于所有界面之上
    this->setWindowFlags(Qt::FramelessWindowHint);
    //背景透明
    //setAttribute(Qt::WA_TranslucentBackground, true);

    //使用调色板设置窗口的背景色
    QPalette pal_windows(palette());
    pal_windows.setColor(QPalette::Background, QColor(255, 255, 255));
    setAutoFillBackground(false);
    setPalette(pal_windows);
    setMinimumSize(400 , 300);
    //定义自定义标题栏对象
    titleBar *pTitleBar = new titleBar(this);
    installEventFilter(pTitleBar);


    resize(960, 540);
    setWindowTitle("My Music Player"); //设置窗口名称，会发生窗口标题栏改变事件，随之自定义标题栏的标题会更新
    setWindowIcon(QIcon(":/icon/res/icon.png")); //设置窗口图标，会发生窗口图标改变事件，随之自定义标题栏的图标会更新



    //窗口布局中加标题栏盒子
    QVBoxLayout *pLayout = new QVBoxLayout();


    pLayout->addWidget(pTitleBar);
    pLayout->addStretch();
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(0, 0, 0, 0);

    setLayout(pLayout);

    //m_nBorder表示鼠标位于边框缩放范围的宽度，可以设置为5
    m_nBorderWidth=5;

    SongInfo si;
    AudioTag* aa = new AudioTag(QString("D:/2.mp3"),si);
    aa->getAllinfo();
    aa->idMatch();
    aa->downloadPic();
    qDebug()<<"***************";

    //歌曲信息测试
    SongInfoShow *infoShow = new SongInfoShow(this,si);
    infoShow->setGeometry(250,30,(this->width()-240)/2,this->height()-30);



}

MainWidget::~MainWidget()
{
    delete ui;
}

//nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
bool MainWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        // 如果鼠标位于子控件上，则不进行处理
        if (childAt(nX, nY) != nullptr)
            return QWidget::nativeEvent(eventType, message, result);

        *result = HTCAPTION;

        // 鼠标区域位于窗体边框，进行缩放
        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
                && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    }

    return QWidget::nativeEvent(eventType, message, result);
}
