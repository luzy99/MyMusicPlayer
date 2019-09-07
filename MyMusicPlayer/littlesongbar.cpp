#include "littlesongbar.h"
#include <QHBoxLayout>
#include <QBitmap>
#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QDebug>

//SongInfo &info,
LittleSongBar::LittleSongBar(QWidget *parent)
    :QWidget(parent)
{
    //初始化窗口信息
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setStyleSheet("background-color: rgb(255,255,255)");

    this->setFixedHeight(60);

    //初始化界面布局(水平布局)
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(8);
    layout->setContentsMargins(5,0,5,0);

    //初始化显示封面的Label
    coverLabel = new QLabel;
    coverLabel->setObjectName("coverLabel");
    QPixmap image(":/icon/res/default_cover.png");
    coverImage = image.scaled(QSize(50,50), Qt::KeepAspectRatio,Qt::FastTransformation);
    coverLabel->setPixmap(coverImage);
    coverLabel->setFixedWidth(50);
    coverLabel->setToolTip("点击查看歌曲详细信息");
    coverLabel->setWindowFlags(Qt::FramelessWindowHint);
    opacityEffect=new QGraphicsOpacityEffect;
    coverLabel->setGraphicsEffect(opacityEffect);
    opacityEffect->setOpacity(1);
    coverLabel->setAttribute(Qt::WA_Hover,true);
    coverLabel->installEventFilter(this);
    layout->addWidget(coverLabel);

    //初始化标签界面的
    QVBoxLayout *labelLayout = new QVBoxLayout;
    labelLayout->setSpacing(0);
    labelLayout->setContentsMargins(0,0,0,0);
    //初始化显示标题的Label
    titleLabel = new QLabel;
    titleLabel->setObjectName("titleLabel");
    titleLabel->setFont(QFont("Microsoft YaHei", 10, 50));
    titleLabel->setText("暂无歌曲在播放");
    labelLayout->addWidget(titleLabel);
    //初始化显示作者的Label
    authorLabel = new QLabel;
    authorLabel->setObjectName("authorLabel");
    authorLabel->setFont(QFont("Microsoft YaHei", 9, 25));
    authorLabel->setText("暂无相关信息");
    labelLayout->addWidget(authorLabel);
    layout->addLayout(labelLayout);

    //设置界面布局
    this->setLayout(layout);
}

//切歌是更新显示信息
void LittleSongBar::changeSong(SongInfo &info)
{
    QPixmap image(info.album_cover);
    coverImage = image.scaled(QSize(50,50), Qt::KeepAspectRatio,Qt::FastTransformation);
    coverLabel->setPixmap(coverImage);
    titleLabel->setText(info.title);
    authorLabel->setText(info.artist);
}

//事件过滤器,主要针对coverLabel
bool LittleSongBar::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->objectName() == "coverLabel")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            //qDebug()<<"hover enter";
            opacityEffect->setOpacity(0.75);
        }

        if(event->type() == QEvent::HoverLeave)
        {
            //qDebug()<<"hover leave";
            opacityEffect->setOpacity(1);
        }

        if(event->type() == QEvent::MouseButtonPress)
        {
            //qDebug()<<"press";
            emit changePage(); //发出换页信号
        }
    }

    return QWidget::eventFilter(obj,event);
}
