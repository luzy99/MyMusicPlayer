#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include "titleBar.h"

//调用WIN API需要用到的头文件与库
#ifdef Q_OS_WIN
#pragma comment(lib, "user32.lib")
#include <qt_windows.h>
#endif

titleBar::titleBar(QWidget *parent)
    : QWidget(parent)
{
    setFixedHeight(30);

    //给成员变量申请内存
    m_pIconLabel = new QLabel(this);
    m_pTitleLabel = new QLabel(this);
    m_pMinimizeButton = new QPushButton(this);
    m_pMaximizeButton = new QPushButton(this);
    m_pCloseButton = new QPushButton(this);
    m_pSettingButton = new QPushButton(this);
    m_pSkinButton = new QPushButton(this);

    //初始化图标Label
    m_pIconLabel->setFixedSize(20, 20);
    m_pIconLabel->setScaledContents(true);

    m_pTitleLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    //设置按钮的固定大小、图片、取消边框
    m_pMinimizeButton->setIconSize(QSize(27,22));
    m_pMinimizeButton->setIcon(QIcon(":/icon/res/minscreen.png"));
    m_pMinimizeButton->setFlat(true);
    m_pMinimizeButton->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");
    //--
    m_pMaximizeButton->setIconSize(QSize(27,22));
    m_pMaximizeButton->setIcon(QIcon(":/icon/res/maxscreen.png"));
    m_pMaximizeButton->setFlat(true);
    m_pMaximizeButton->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");
    //--
    m_pCloseButton->setIconSize(QSize(27,22));
    m_pCloseButton->setIcon(QIcon(":/icon/res/close.png"));
    m_pCloseButton->setFlat(true);
    m_pCloseButton->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");
    //--
    m_pSettingButton->setIconSize(QSize(27,22));
    m_pSettingButton->setIcon(QIcon(":/icon/res/setting.png"));
    m_pSettingButton->setFlat(true);
    m_pSettingButton->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");
    //--
    m_pSkinButton->setIconSize(QSize(27,22));
    m_pSkinButton->setIcon(QIcon(":/icon/res/skin.png"));
    m_pSkinButton->setFlat(true);
    m_pSkinButton->setStyleSheet("QPushButton:hover{background-color:rgb(244,239,239);}");


    //设置窗口部件的名称
    m_pTitleLabel->setObjectName("whiteLabel");
    m_pMinimizeButton->setObjectName("minimizeButton");
    m_pMaximizeButton->setObjectName("maximizeButton");
    m_pCloseButton->setObjectName("closeButton");
    m_pSettingButton->setObjectName("SettingButton");
    m_pSkinButton->setObjectName("SkinButton");


    //给按钮设置静态tooltip，当鼠标移上去时显示tooltip
    m_pMinimizeButton->setToolTip("Minimize");
    m_pMaximizeButton->setToolTip("Maximize");
    m_pCloseButton->setToolTip("Close");

    //标题栏布局
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->addWidget(m_pIconLabel);
    pLayout->addSpacing(5);
    pLayout->addWidget(m_pTitleLabel);
    pLayout->addWidget(m_pSettingButton);
    pLayout->addWidget(m_pSkinButton);
    pLayout->addWidget(m_pMinimizeButton);
    pLayout->addWidget(m_pMaximizeButton);
    pLayout->addWidget(m_pCloseButton);

    //装入盒子
    g_title = new QGroupBox(this) ;
    g_title->setObjectName("GroupTitle");
    g_title->setFlat(true);
    g_title->setGeometry(0, 0, this->width(), 30);
    g_title->setStyleSheet("background-color: rgb(219, 208, 208);");
    pLayout->setSpacing(0);
    pLayout->setContentsMargins(5, 0, 5, 0);
    g_title->setLayout(pLayout);

    //连接三个按钮的信号槽
    connect(m_pMinimizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pMaximizeButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
    connect(m_pCloseButton, SIGNAL(clicked(bool)), this, SLOT(onClicked()));
}

titleBar::~titleBar()
{

}

//双击标题栏进行界面的最大化/还原
void titleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event); //没有实质性的作用，只是用来允许event可以不使用，用来避免编译器警告

    emit m_pMaximizeButton->clicked(1);
}

//进行界面的拖动  [一般情况下，是界面随着标题栏的移动而移动，所以我们将事件写在标题栏中比较合理]
void titleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget *pWindow = this->window();
        if (pWindow->isTopLevel())
        {
           SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
       event->ignore();
#else
#endif
}

//使用事件过滤器监听标题栏所在的窗体，所以当窗体标题、图标等信息发生改变时，标题栏也应该随之改变
bool titleBar::eventFilter(QObject *obj, QEvent *event)
{
    switch ( event->type() ) //判断发生事件的类型
    {
        case QEvent::WindowTitleChange: //窗口标题改变事件
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj); //获得发生事件的窗口对象
            if (pWidget)
            {
                //窗体标题改变，则标题栏标题也随之改变
                m_pTitleLabel->setText(pWidget->windowTitle());
                return true;
            }
        }
        break;

        case QEvent::WindowIconChange: //窗口图标改变事件
        {
            QWidget *pWidget = qobject_cast<QWidget *>(obj);
            if (pWidget)
            {
                //窗体图标改变，则标题栏图标也随之改变
                QIcon icon = pWidget->windowIcon();
                m_pIconLabel->setPixmap(icon.pixmap(m_pIconLabel->size()));
                return true;
            }
        }
        break;

        case QEvent::Resize:
            updateMaximize(); //最大化/还原
            return true;

        default:
        return QWidget::eventFilter(obj, event);
    }

    return QWidget::eventFilter(obj, event);
}

//进行最小化、最大化/还原、关闭操作
void titleBar::onClicked()
{
    //QObject::Sender()返回发送信号的对象的指针，返回类型为QObject *
    QPushButton *pButton = qobject_cast<QPushButton *>(sender());

    QWidget *pWindow = this->window(); //获得标题栏所在的窗口

    if (pWindow->isTopLevel())
    {
        //判断发送信号的对象使哪个按钮
        if (pButton == m_pMinimizeButton)
        {
            pWindow->showMinimized(); //窗口最小化显示
        }
        else if (pButton == m_pMaximizeButton)
        {
            pWindow->isMaximized() ? pWindow->showNormal() : pWindow->showMaximized();  //窗口最大化/还原显示
        }
        else if (pButton == m_pCloseButton)
        {
            pWindow->close(); //窗口关闭
        }
        g_title->setGeometry(0, 0, this->width(), 30);
    }
}

//最大化/还原
void titleBar::updateMaximize()
{
    QWidget *pWindow = this->window(); //获得标题栏所在的窗口

    if (pWindow->isTopLevel())
    {
        bool bMaximize = pWindow->isMaximized(); //判断窗口是不是最大化状态，是则返回true，否则返回false
        if (bMaximize)
        {
            //目前窗口是最大化状态，则最大化/还原的toolTip设置为"Restore"
            m_pMaximizeButton->setToolTip(tr("Restore"));
            //设置按钮的属性名为"maximizeProperty"
            m_pMaximizeButton->setIcon(QIcon(":/icon/res/normalscreen.png"));
            m_pMaximizeButton->setProperty("maximizeProperty", "restore");
        }
        else
        {
            //目前窗口是还原状态，则最大化/还原的toolTip设置为"Maximize"
            m_pMaximizeButton->setToolTip(tr("Maximize"));
            //设置按钮的属性名为"maximizeProperty"
            m_pMaximizeButton->setIcon(QIcon(":/icon/res/maxscreen.png"));
            m_pMaximizeButton->setProperty("maximizeProperty", "maximize");
        }

        m_pMaximizeButton->setStyle(QApplication::style());
    }
    g_title->setGeometry(0, 0, this->width(), 30);
}
