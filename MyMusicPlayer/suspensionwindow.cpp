//这是一个悬浮框
//用来和主窗口互换视图
#include "suspensionwindow.h"
#include <QHBoxLayout> //水平布局

SuspensionWindow::SuspensionWindow(QWidget *parent) :
    QWidget(parent)
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
    layout->addWidget(likeBtn);

    //初始化显示播放列表的按钮
    showListsBtn = new QPushButton;
    showListsBtn->setFlat(true);
    showListsBtn->setToolTip("点击显示歌单");
    showListsBtn->setIcon(QIcon(":/icon/res/showSongList.png"));
    showListsBtn->setIconSize(QSize(25,25));
    layout->addWidget(showListsBtn);

    //初始化恢复大窗口按钮
    resizeBtn = new QPushButton;
    resizeBtn->setFlat(true);
    resizeBtn->setIcon(QIcon(":/icon/res/resize.png"));
    resizeBtn->setIconSize(QSize(25,25));
    layout->addWidget(resizeBtn);

    //初始化中心组件
    layout->setMargin(0);
    this->setLayout(layout);
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

void SuspensionWindow::setSongsInfo(const QString &value)
{
    songsInfo = value;
}

void SuspensionWindow::setCoverImage(const QPixmap &value)
{
    coverImage = value;
}
