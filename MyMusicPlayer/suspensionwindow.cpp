//这是一个悬浮框
//用来和主窗口互换视图
#include "suspensionwindow.h"
#include <QHBoxLayout> //水平布局
#include <QDebug>

SuspensionWindow::SuspensionWindow(QWidget *parent)
    :QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(350,60); //悬浮窗大小不可改变

    QHBoxLayout *layout = new QHBoxLayout;

    //初始化专辑封面
    coverLabel = new QLabel;
    coverImage.load(":/icon/res/defaultCover.png");
    QPixmap newImage = coverImage.scaled(QSize(60,60), Qt::KeepAspectRatio,Qt::FastTransformation);
    coverLabel->setPixmap(newImage);
    layout->addWidget(coverLabel);

    //初始化歌词滚动的标签
    lyricsLabel = new QLabel;
    lyricsLabel->setText("暂时无音乐播放");
    layout->addWidget(lyricsLabel);

    //初始化我喜欢按钮
    likeBtn = new QPushButton;
    likeBtn->setFlat(true);
    likeBtn->setIcon(QIcon(":/icon/res/favourite_d.ico"));
    likeBtn->setIconSize(QSize(25,25));
    likeBtn->setMaximumSize(27,27);
    layout->addWidget(likeBtn);

    //初始化显示播放列表的按钮
    showListsBtn = new QPushButton;
    showListsBtn->setFlat(true);
    showListsBtn->setObjectName("showListsBtn");
    showListsBtn->setToolTip("点击显示歌单");
    showListsBtn->setIcon(QIcon(":/icon/res/showSongList.png"));
    showListsBtn->setIconSize(QSize(25,25));
    showListsBtn->setMaximumSize(27,27);
    //开启鼠标悬停事件
    showListsBtn->setAttribute(Qt::WA_Hover,true);
    //安装事件过滤器
    showListsBtn->installEventFilter(this);
    layout->addWidget(showListsBtn);

    //初始化恢复大窗口按钮
    resizeBtn = new QPushButton;
    resizeBtn->setFlat(true);
    resizeBtn->setToolTip("点击切换窗口模式");
    resizeBtn->setIcon(QIcon(":/icon/res/resize.png"));
    resizeBtn->setIconSize(QSize(25,25));
    resizeBtn->setMaximumSize(27,27);
    layout->addWidget(resizeBtn);

    //初始化右键菜单
    rightClickedMenu = new QMenu(this);
    initActions(rightClickedMenu);

    //初始化歌曲菜单
    showListsMenu = new QMenu;
    showListsBtn->setMenu(showListsMenu);

    //初始化中心组件
    layout->setMargin(0);
    this->setLayout(layout);

    //初始化信号与槽
    initSignalsAndSlots();
}

//初始化型号与槽
void SuspensionWindow::initSignalsAndSlots()
{
}

//初始化右键菜单的Actions
void SuspensionWindow::initActions(QMenu *menu)
{
    //关闭事件
    QAction *actClose = new QAction;
    actClose->setIcon(QIcon(":/icon/res/close.png"));
    actClose->setText("关闭窗口");
    connect(actClose,SIGNAL(triggered()),
            this,SLOT(close()));
    menu->addAction(actClose);
}

//重写鼠标按下事件
void SuspensionWindow::mousePressEvent(QMouseEvent *event)
{
    mouseStartPoint = event->globalPos(); //记录鼠标的初始位置
    windowsStartPoint = this->pos(); //记录窗口的初始位置
}

//重写鼠标拖动事件
void SuspensionWindow::mouseMoveEvent(QMouseEvent *event)
{
    //窗口和鼠标偏移量一致
    QPoint offset = event->globalPos() - mouseStartPoint ;
    QPoint p = windowsStartPoint + offset;
    this->move(p);
}

//重写右键菜单事件
void SuspensionWindow::contextMenuEvent(QContextMenuEvent *event)
{
    rightClickedMenu->exec(QCursor::pos());
    event->accept();
}

//事件过滤器
bool SuspensionWindow::eventFilter(QObject *obj, QEvent *event)
{
    //"显示歌单"按钮的事件过滤器
    if(obj->objectName()=="showListsBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            showListsBtn->setIcon(QIcon(":/icon/res/showSongListHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            showListsBtn->setIcon(QIcon(":/icon/res/showSongList.png"));
        }
    }
    return QWidget::eventFilter(obj,event);
}

//**********************************
//注意这里我么的写完！！！！！！！！！！
//显示播放列表按钮按下时触发的槽函数
void SuspensionWindow::on_showListBtn_clicked(bool checked)
{
    //点击变色且
    if(!checked)
    {
        showListsBtn->setIcon(QIcon(":/icon/res/showSongListHover.png"));
        //选中后关闭事件过滤器
        showListsBtn->setAttribute(Qt::WA_Hover,false);
    }
}

//修改当前的播放列表
//点击小窗化之后要传入
void SuspensionWindow::setShowListsMenu(QMenu *value)
{
    showListsMenu = value;
}

//修改正在播放的歌曲的信息
//给主界面提供一个同步的接口
void SuspensionWindow::setSongsInfo(const QString &value)
{
    songsInfo = value;
}

//修改正在播放的歌曲的专辑封面
//给主界面提供一个同步的接口
void SuspensionWindow::setCoverImage(const QPixmap &value)
{
    coverImage = value;
}
