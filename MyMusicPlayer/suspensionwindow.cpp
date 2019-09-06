#include "suspensionwindow.h"
#include <QHBoxLayout> //水平布局
#include <QDebug>
#include <QPainter>
#include <QBitmap>
#include <QApplication>
#include <QTimer>

SuspensionWindow::SuspensionWindow(QWidget *parent)
    : QWidget(parent),
      status(Normal)
{
    //设置窗口样式
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setObjectName("SuspensionWindow");
    this->setAttribute(Qt::WA_Hover,true);
    this->installEventFilter(this);
    this->setWindowOpacity(0.9);
    this->setStyleSheet("background-color: rgb(255,255,255)");
    this->setFixedSize(350,60); //悬浮窗大小不可改变

    //绘制圆角窗口
    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setRenderHint(QPainter::Antialiasing); // 反锯齿;
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),2,2);
    setMask(bmp);

    //获取桌面屏幕大小
    QRect deskRect = QApplication::desktop()->screenGeometry();
    screenWidth = deskRect.width();
    screenHeight = deskRect.height();

    //水平布局
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(5);
    layout->setContentsMargins(0,0,5,0);

    //初始化小窗标签按钮
    suspensionBtn = new QPushButton;
    suspensionBtn->setObjectName("suspensionBtn");
    suspensionBtn->setWindowFlags(Qt::FramelessWindowHint);
    suspensionBtn->setAttribute(Qt::WA_TranslucentBackground);
    suspensionBtn->setIcon(QIcon(":/icon/res/suspensionWindow.png"));
    suspensionBtn->setIconSize(QSize(60,60));
    suspensionBtn->setFixedSize(60,60);
    suspensionBtn->setFlat(true);
    suspensionBtn->setAttribute(Qt::WA_Hover,true);
    suspensionBtn->installEventFilter(this);
    suspensionBtn->setStyleSheet("border: none;");

    //初始化专辑封面t5
    coverLabel = new QLabel;
    coverLabel->setObjectName("coverLabel");
    coverImage.load(":/icon/res/default_cover.png");
    QPixmap newImage = coverImage.scaled(QSize(60,60), Qt::KeepAspectRatio,Qt::FastTransformation);
    coverLabel->setPixmap(newImage);
    layout->addWidget(coverLabel);

    stackedLayout = new QStackedLayout;
    stackedLayout->setStackingMode(QStackedLayout::StackOne);
    stackedLayout->setContentsMargins(0,0,0,0);

    //初始化按钮组
    buttonWidget = new QWidget;
    buttonWidget->setWindowFlags(Qt::FramelessWindowHint);
    buttonWidget->setAttribute(Qt::WA_TranslucentBackground);
    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(0);
    buttonLayout->setContentsMargins(2,2,2,2);
    previousBtn = new QPushButton;
    previousBtn->setFlat(true);
    previousBtn->setIcon(QIcon(":/icon/res/previous.png"));
    previousBtn->setIconSize(QSize(30,30));
    previousBtn->setFixedSize(30,30);
    previousBtn->setStyleSheet("border: none;");
    buttonLayout->addWidget(previousBtn);
    playBtn = new QPushButton;
    playBtn->setFlat(true);
    playBtn->setIcon(QIcon(":/icon/res/pause.png"));
    playBtn->setIconSize(QSize(35,35));
    playBtn->setFixedSize(35,35);
    playBtn->setStyleSheet("border: none;");
    buttonLayout->addWidget(playBtn);
    nextBtn = new QPushButton;
    nextBtn->setFlat(true);
    nextBtn->setIcon(QIcon(":/icon/res/next.png"));
    nextBtn->setIconSize(QSize(30,30));
    nextBtn->setFixedSize(30,30);
    nextBtn->setStyleSheet("border: none;");
    buttonLayout->addWidget(nextBtn);
    buttonWidget->setLayout(buttonLayout);
    buttonWidget->setFixedHeight(60);
    stackedLayout->insertWidget(0,buttonWidget);

    //初始化歌词滚动的标签
    lyricsLabel = new QLabel;
    lyricsLabel->setObjectName("lyricsLabel");
    lyricsLabel->setText("  暂时无音乐播放");
    lyricsLabel->setFixedHeight(60);
    stackedLayout->insertWidget(1,lyricsLabel);
    stackedLayout->setCurrentIndex(1);

    layout->addLayout(stackedLayout);

    //初始化我喜欢按钮
    likeBtn = new QPushButton;
    likeBtn->setFlat(true);
    likeBtn->setIcon(QIcon(":/icon/res/favourite_d.ico"));
    likeBtn->setIconSize(QSize(25,25));
    likeBtn->setFixedSize(27,27);
    layout->addWidget(likeBtn);

    //初始化显示播放列表的按钮
    showListsBtn = new QPushButton;
    showListsBtn->setFlat(true);
    showListsBtn->setStyleSheet("border: none;");
    showListsBtn->setObjectName("showListsBtn");
    showListsBtn->setAttribute(Qt::WA_TranslucentBackground);
    showListsBtn->setToolTip("点击显示歌单");
    showListsBtn->setIcon(QIcon(":/icon/res/showSongList.png"));
    showListsBtn->setIconSize(QSize(27,27));
    showListsBtn->setFixedSize(29,29);
    //开启鼠标悬停事件
    showListsBtn->setAttribute(Qt::WA_Hover,true);
    //安装事件过滤器
    showListsBtn->installEventFilter(this);
    layout->addWidget(showListsBtn);

    //初始化恢复大窗口按钮
    resizeBtn = new QPushButton;
    resizeBtn->setFlat(true);
    resizeBtn->setStyleSheet("border: none;");
    resizeBtn->setObjectName("resizeBtn");
    resizeBtn->setToolTip("点击切换窗口模式");
    resizeBtn->setIcon(QIcon(":/icon/res/resize.png"));
    resizeBtn->setIconSize(QSize(25,25));
    resizeBtn->setFixedSize(27,27);
    resizeBtn->setAttribute(Qt::WA_Hover,true);
    resizeBtn->installEventFilter(this);
    layout->addWidget(resizeBtn);

    //初始化右键菜单
    rightClickedMenu = new QMenu(this);
    initActions(rightClickedMenu);

    //初始化中心组件
    layout->setMargin(0);
    this->setLayout(layout);

    //初始化信号与槽
    initSignalsAndSlots();
}

//初始化型号与槽
void SuspensionWindow::initSignalsAndSlots()
{
    connect(resizeBtn,SIGNAL(clicked()),
            this,SIGNAL(hideWindow()));
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

//鼠标放下进行检测
void SuspensionWindow::mouseReleaseEvent(QMouseEvent *event)
{
    //记录当前窗口的位置
    int x=this->x();
    int y=this->y();
    //如果位置是合法的
    if(x > 0 && y >= 0 && x+350 < screenWidth)
    {
        status = Normal;
        return;
    }

    //如果位置触顶了,将它移回去
    if(y < 0)
    {
        this->move(x,0);
    }
    else {}

    //如果位置触边了,设置触边事件
    if(x <= 0)
    {
        status = LeftOut;
        moveOut();
    }
    else if( x+350 >= screenWidth)
    {
        status = RightOut;
        moveOut();
    }
    else
    {
    }
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
    if(obj->objectName()=="SuspensionWindow")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            stackedLayout->setCurrentIndex(0);
        }
        if(event->type() == QEvent::HoverLeave)
        {
            stackedLayout->setCurrentIndex(1);
        }
    }

    //"显示歌单"按钮的事件过滤器
    if(obj->objectName() == "showListsBtn")
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
    else if(obj->objectName() == "resizeBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            resizeBtn->setIcon(QIcon(":/icon/res/resizeHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            resizeBtn->setIcon(QIcon(":/icon/res/resize.png"));
        }
    }
    else if(obj->objectName() == "suspensionBtn")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            moveIn();
        }
    }
    else
    {

    }

    return QWidget::eventFilter(obj,event);
}

//表示接下来播放
void SuspensionWindow::onBecomePlaying()
{
    playBtn->setIcon(QIcon(":/icon/res/pause.png"));
}

//表示接下来暂停
void SuspensionWindow::onBecomePausing()
{
    playBtn->setIcon(QIcon(":/icon/res/play.png"));
}

void SuspensionWindow::on_showListBtn_clicked(bool checked)
{
    //点击变色且显示歌单
    if(!checked)
    {
//        showListsBtn->setIcon(QIcon(":/icon/res/showSongListHover.png"));
//        //选中后关闭事件过滤器
//        showListsBtn->setAttribute(Qt::WA_Hover,false);
    }
}

//修改当前的播放列表
//点击小窗化之后要传入
void SuspensionWindow::setShowListsMenu(QMenu *value)
{
    showListsMenu = value;
}

//窗口移入
void SuspensionWindow::moveOut()
{
    QPoint start = this->pos();
    if(status == LeftOut)
    {
        QPoint end(-350,start.y());
        QPropertyAnimation *animation = new QPropertyAnimation;
        animation->setTargetObject(this);
        animation->setPropertyName("pos");
        animation->setDuration(500);
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->start();

        suspensionBtn->setProperty("pos",QPoint(0,start.y()));
        suspensionBtn->show();
    }
    else
    {
        QPoint end(screenWidth,start.y());
        QPropertyAnimation *animation = new QPropertyAnimation;
        animation->setTargetObject(this);
        animation->setPropertyName("pos");
        animation->setDuration(500);
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->start();

        suspensionBtn->setProperty("pos",QPoint(screenWidth-60,start.y()));
        suspensionBtn->show();
    }
}

//窗口移出
void SuspensionWindow::moveIn()
{
    QPoint start = this->pos();
    if(status == LeftOut)
    {
        QPoint end(0,start.y());
        QPropertyAnimation *animation = new QPropertyAnimation;
        animation->setTargetObject(this);
        animation->setPropertyName("pos");
        animation->setDuration(500);
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->start();
    }
    else
    {
        QPoint end(screenWidth-350,start.y());
        QPropertyAnimation *animation = new QPropertyAnimation;
        animation->setTargetObject(this);
        animation->setPropertyName("pos");
        animation->setDuration(500);
        animation->setStartValue(start);
        animation->setEndValue(end);
        animation->start();
    }

    //弹出后不在收回
    suspensionBtn->hide();
    status = Normal;
}

void SuspensionWindow::changeSong(SongInfo &info)
{
    QPixmap image(info.album_cover);
    QPixmap coverImage = image.scaled(QSize(60,60), Qt::KeepAspectRatio,Qt::FastTransformation);
    coverLabel->setPixmap(coverImage);
    lyricsLabel->setText("  " + info.title + ' ' + info.artist);
}

//修改正在播放的歌曲的信息
//给主界面提供一个同步的接口
void SuspensionWindow::setSongsInfo(const QString &value)
{
    songsInfo = value;
    lyricsLabel->setText(songsInfo);
}

//修改正在播放的歌曲的专辑封面
//给主界面提供一个同步的接口
void SuspensionWindow::setCoverImage(const QPixmap &value)
{
    coverImage = value;
    QPixmap newImage = coverImage.scaled(QSize(60,60), Qt::KeepAspectRatio,Qt::FastTransformation);
    coverLabel->setPixmap(newImage);
}
