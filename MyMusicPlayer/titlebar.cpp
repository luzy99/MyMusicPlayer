#include "titleBar.h"
#include "errorwindow.h"
#include <QLabel>
#include <QPushButton>
#include <QEvent>
#include <QMouseEvent>
#include <QApplication>
#include <QPalette>
#include <QDebug>

TitleBar::TitleBar(QWidget *parent)
    : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAutoFillBackground(true);
    QPalette backPalette;
    backPalette.setColor(QPalette::Window,QColor(25,25,25));
    this->setPalette(backPalette);
    this->setFixedHeight(60);

    //初始化页面布局
    QHBoxLayout *layout = new QHBoxLayout;

    //初始化标图标Label
    iconLabel = new QLabel;
    iconLabel->setObjectName("iconLabel");
    QPixmap iconImage(":/icon/res/suspensionWindow.png");
    QPixmap labelImage = iconImage.scaled(QSize(35,35),Qt::KeepAspectRatio,Qt::SmoothTransformation);
    iconLabel->setPixmap(labelImage);
    iconLabel->setFixedWidth(35);
    layout->addWidget(iconLabel);

    //初始化标题Label
    titleLabel = new QLabel;
    titleLabel->setObjectName("titleLabel");
    QFont font("Comic Sans MS", 10, QFont::Normal);
    titleLabel->setFont(font);
    titleLabel->setText("Rainbow Music Player");
    titleLabel->setFixedWidth(200);
    QPalette palette = titleLabel->palette();
    palette.setColor(QPalette::WindowText,Qt::white);
    titleLabel->setPalette(palette);
    layout->addWidget(titleLabel);

    //初始化搜索框
    searchBar = new QLineEdit;
    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addStretch();
    searchLayout->setSpacing(0);
    searchLayout->setContentsMargins(0,2,2,2);
    searchBtn = new QPushButton(searchBar);
    searchBtn->setObjectName("searchBtn");
    searchBtn->setIcon(QIcon(":/icon/res/search.png"));
    searchBtn->setIconSize(QSize(15,15));
    searchBtn->setFlat(true);
    searchBtn->setStyleSheet("border: none;");
    searchBtn->setAttribute(Qt::WA_TranslucentBackground);
    searchBtn->installEventFilter(this);
    searchBtn->setToolTip("点击开始搜索");
    searchBtn->setCursor(QCursor(Qt::ArrowCursor));
    searchLayout->addWidget(searchBtn);
    searchBar->setObjectName("searchBar");
    searchBar->setFixedWidth(250);
    searchBar->setPlaceholderText("输入搜索信息");
    searchBar->setStyleSheet("background: rgb(50,50,50);" \
                             "border:1px solid rgb(70,70,70);" \
                             "border-radius: 10px;" \
                             "color: white;");
    searchBar->setTextMargins(0,0,searchBtn->width(),0);
    searchBar->setLayout(searchLayout);
    layout->addWidget(searchBar);

    //初始化搜索结果的组件
    searchResult = new ResultWidget;
    searchResult->setWindowFlags(Qt::FramelessWindowHint);

    //初始化填充空行的
    spacingLabel = new QLabel;
    spacingLabel->setAttribute(Qt::WA_TranslucentBackground);
    layout->addWidget(spacingLabel);

    //初始化用户登录按钮
    userBtn = new QPushButton;
    userBtn->setObjectName("userBtn");
    userBtn->setFixedSize(120,30);
    userBtn->setAttribute(Qt::WA_TranslucentBackground);
    userBtn->setFlat(true);
    userBtn->setIcon(QIcon(":/icon/res/defaultUser.png"));
    userBtn->setIconSize(QSize(30,30));
    userBtn->setText(" 未登录");
    userBtn->setAttribute(Qt::WA_Hover,true);
    userBtn->installEventFilter(this);
    userBtn->setStyleSheet("color: #dcdcdc;" \
                           "border: none;" \
                           "font-family: Microsoft YaHei;" );
    layout->addWidget(userBtn);

    //初始化皮肤按钮
    skinBtn = new QPushButton;
    skinBtn->setObjectName("skinBtn");
    skinBtn->setAttribute(Qt::WA_TranslucentBackground);
    skinBtn->setIcon(QIcon(":/icon/res/skin.png"));
    skinBtn->setIconSize(QSize(35,35));
    skinBtn->setFixedSize(45,35);
    skinBtn->setFlat(true);
    skinBtn->setStyleSheet("border: none;");
    skinBtn->setToolTip("点击更换皮肤");
    skinBtn->setAttribute(Qt::WA_Hover,true);
    skinBtn->installEventFilter(this);
    layout->addWidget(skinBtn);

    //初始化手势按钮
    gestureBtn = new QPushButton;
    gestureBtn->setObjectName("gestureBtn");
    gestureBtn->setAttribute(Qt::WA_TranslucentBackground);
    gestureBtn->setIcon(QIcon(":/icon/res/gesture.png"));
    gestureBtn->setIconSize(QSize(27,27));
    gestureBtn->setFixedSize(45,35);
    gestureBtn->setFlat(true);
    gestureBtn->setStyleSheet("border: none;");
    gestureBtn->setToolTip("点击开启手势识别");
    gestureBtn->setAttribute(Qt::WA_Hover,true);
    gestureBtn->installEventFilter(this);
    layout->addWidget(gestureBtn);
    gestureControl = false;

    //初始化设置按钮
    settingsBtn = new QPushButton;
    settingsBtn->setObjectName("settingsBtn");
    settingsBtn->setAttribute(Qt::WA_TranslucentBackground);
    settingsBtn->setIcon(QIcon(":/icon/res/setting.png"));
    settingsBtn->setIconSize(QSize(25,25));
    settingsBtn->setFixedSize(45,35);
    settingsBtn->setFlat(true);
    settingsBtn->setStyleSheet("border: none;");
    settingsBtn->setToolTip("点击更换皮肤");
    settingsBtn->setAttribute(Qt::WA_Hover,true);
    settingsBtn->installEventFilter(this);
    layout->addWidget(settingsBtn);

    //初始化小窗按钮
    resizeBtn = new QPushButton;
    resizeBtn->setObjectName("resizeBtn");
    resizeBtn->setAttribute(Qt::WA_TranslucentBackground);
    resizeBtn->setIcon(QIcon(":/icon/res/miniWindow.png"));
    resizeBtn->setIconSize(QSize(28,28));
    resizeBtn->setFixedSize(30,30);
    resizeBtn->setFlat(true);
    resizeBtn->setStyleSheet("border: none;");
    resizeBtn->setToolTip("点击进入小窗模式");
    resizeBtn->setAttribute(Qt::WA_Hover,true);
    resizeBtn->installEventFilter(this);
    layout->addWidget(resizeBtn);

    //初始化最小化按钮
    minimizeBtn = new QPushButton;
    minimizeBtn->setObjectName("minimizeBtn");
    minimizeBtn->setAttribute(Qt::WA_TranslucentBackground);
    minimizeBtn->setIcon(QIcon(":/icon/res/titleMinimize.png"));
    minimizeBtn->setIconSize(QSize(23,23));
    minimizeBtn->setFixedSize(25,25);
    minimizeBtn->setFlat(true);
    minimizeBtn->setStyleSheet("border: none;");
    minimizeBtn->setToolTip("点击最小化窗口");
    minimizeBtn->setAttribute(Qt::WA_Hover,true);
    minimizeBtn->installEventFilter(this);
    layout->addWidget(minimizeBtn);

    //初始化最大化&还原按钮
    maximizeBtn = new QPushButton;
    maximizeBtn->setObjectName("maximizeBtn");
    maximizeBtn->setAttribute(Qt::WA_TranslucentBackground);
    maximizeBtn->setIcon(QIcon(":/icon/res/titleMaximize.png"));
    maximizeBtn->setIconSize(QSize(23,23));
    maximizeBtn->setFixedSize(25,25);
    maximizeBtn->setFlat(true);
    maximizeBtn->setStyleSheet("border: none;");
    maximizeBtn->setToolTip("点击最大化/还原窗口");
    maximizeBtn->setAttribute(Qt::WA_Hover,true);
    maximizeBtn->installEventFilter(this);
    layout->addWidget(maximizeBtn);

    //初始化关闭窗口
    closeBtn = new QPushButton;
    closeBtn->setObjectName("closeBtn");
    closeBtn->setAttribute(Qt::WA_TranslucentBackground);
    closeBtn->setIcon(QIcon(":/icon/res/titleClose.png"));
    closeBtn->setIconSize(QSize(23,23));
    closeBtn->setFixedSize(25,25);
    closeBtn->setFlat(true);
    closeBtn->setToolTip("点击关闭窗口");
    closeBtn->setAttribute(Qt::WA_Hover,true);
    closeBtn->installEventFilter(this);
    layout->addWidget(closeBtn);

    //添加布局到标题栏组件
    this->setLayout(layout);

    //初始化搜索菜单
    initActions();
    //关联信号与槽
    initSignalsAndSlots();
}

//初始化信号与槽
void TitleBar::initSignalsAndSlots()
{
    //点击手势识别按钮开启&关闭手势识别
    connect(gestureBtn,SIGNAL(clicked()),
            this,SLOT(on_gestureBtn_clicked()));

    //点击小窗化按钮通知主界面隐藏主窗体显示悬浮窗
    connect(resizeBtn,SIGNAL(clicked()),
            this,SIGNAL(showSuspensionWindow()));
    //点击最小化按钮通知主窗体最小化
    connect(minimizeBtn,SIGNAL(clicked()),
            this,SIGNAL(minimizeWindow()));
    //点击最大化按钮通知主窗体最大化&还原
    connect(maximizeBtn,SIGNAL(clicked()),
            this,SIGNAL(maximizeWindow()));
    //点击关闭按钮通知主窗体关闭
    connect(closeBtn,SIGNAL(clicked()),
            this,SIGNAL(closeWindow()));

    //搜索栏文字变化时的操作
    connect(searchBar,SIGNAL(editingFinished()),
            this,SLOT(onEditFinished()));
}

//初始化搜索菜单
void TitleBar::initActions()
{
    //初始化搜索结果菜单
    searchMenu = new QMenu;
    QAction *actSearchLocal = new QAction;
    actSearchLocal->setText("在本地搜索结果 >>>>");
    connect(actSearchLocal,SIGNAL(triggered()),
            this,SLOT(on_actSearchLocal_triggered()));
    searchMenu->addAction(actSearchLocal);
    searchMenu->addSeparator();
    QAction *actSearchOnline = new QAction;
    actSearchOnline->setText("在网易云搜索结果 >>>>");
    connect(actSearchOnline,SIGNAL(triggered()),
            this,SLOT(on_actSearchOnline_triggered()));
    searchMenu->addAction(actSearchOnline);
    searchMenu->addSeparator();
    QAction *actSearchMV = new QAction;
    actSearchMV->setText("搜索Mv >>>>");
    connect(actSearchMV,SIGNAL(triggered()),
            this,SLOT(on_actSearchMV_triggered()));
    searchMenu->addAction(actSearchMV);
}

TitleBar::~TitleBar()
{

}

//事件过滤器，按钮按下时高亮
bool TitleBar::eventFilter(QObject *obj, QEvent *event)
{
    //小窗化按钮的事件过滤器
    if(obj->objectName() == "resizeBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            resizeBtn->setIcon(QIcon(":/icon/res/miniWindowHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            resizeBtn->setIcon(QIcon(":/icon/res/miniWindow.png"));
        }
    }
    //最小化按钮的事件过滤器
    else if(obj->objectName() == "minimizeBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            minimizeBtn->setIcon(QIcon(":/icon/res/titleMinimizeHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            minimizeBtn->setIcon(QIcon(":/icon/res/titleMinimize.png"));
        }
    }
    //最大化按钮的事件过滤器
    else if(obj->objectName() == "maximizeBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            maximizeBtn->setIcon(QIcon(":/icon/res/titleMaximizeHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            maximizeBtn->setIcon(QIcon(":/icon/res/titleMaximize.png"));
        }
    }
    //关闭按钮的事件过滤器
    else if(obj->objectName() == "closeBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            closeBtn->setIcon(QIcon(":/icon/res/titleCloseHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            closeBtn->setIcon(QIcon(":/icon/res/titleClose.png"));
        }
    }
    //换肤按钮的事件过滤器
    else if(obj->objectName() == "skinBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            skinBtn->setIcon(QIcon(":/icon/res/skinHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            skinBtn->setIcon(QIcon(":/icon/res/skin.png"));
        }
    }
    //设置按钮的事件过滤器
    else if(obj->objectName() == "settingsBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            settingsBtn->setIcon(QIcon(":/icon/res/settingHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            settingsBtn->setIcon(QIcon(":/icon/res/setting.png"));
        }
    }
    //搜索按钮的事件过滤器
    else if(obj->objectName() == "userBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            userBtn->setStyleSheet("color: #ffffff;" \
                                   "border: none;" \
                                   "font-family: Microsoft YaHei;" );
        }
        if(event->type() == QEvent::HoverLeave)
        {
            userBtn->setStyleSheet("color: #dcdcdc;" \
                                   "border: none;" \
                                   "font-family: Microsoft YaHei;" );
        }
    }
    //手势按钮事件过滤器
    else if(obj->objectName() == "gestureBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            gestureBtn->setIcon(QIcon(":/icon/res/gestureHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            gestureBtn->setIcon(QIcon(":/icon/res/gesture.png"));
        }
    }
    else
    {

    }

    return QWidget::eventFilter(obj,event);
}

//双击时产生和点击最大化按钮一样的效果
void TitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    emit maximizeWindow();
}

//输入结束时
void TitleBar::onEditFinished()
{
    //获得去除两边空格的字符串
    QString searchContents = searchBar->text().trimmed();
    if(searchContents != "")
    {
        QPoint showPoint = QWidget::mapToGlobal(searchBar->pos()) + QPoint(0,40);
        searchMenu->exec(showPoint);
    }
    else
    {
        searchMenu->hide();
    }
}

//点击在网易云搜索时
void TitleBar::on_actSearchOnline_triggered()
{
    QString searchContents = searchBar->text().trimmed();
    emit beginSearchOnline(searchContents);
    searchMenu->hide();
}

//点击在本地搜索时
void TitleBar::on_actSearchLocal_triggered()
{
    QString searchContents = searchBar->text().trimmed();
    emit beginSearchLocal(searchContents);
}

//点击搜索MV
void TitleBar::on_actSearchMV_triggered()
{
    QString searchContents = searchBar->text().trimmed();
    emit beginSearchMv(searchContents);
}

//点击开启&关闭手势识别
void TitleBar::on_gestureBtn_clicked()
{
    //开启手势识别
    if(!gestureControl)
    {
        gestureControl = true;
        ErrorWindow *startGesture = new ErrorWindow("打开手势识别功能");
        startGesture->show();
        startGesture->showInstantly();
        emit beginGesture();
    }
    //关闭手势识别
    else
    {
        gestureControl = false;
        ErrorWindow *stopGesture = new ErrorWindow("关闭手势识别功能");
        stopGesture->show();
        stopGesture->showInstantly();
        emit closeGesture();
    }
}

