#include "songinfoshow.h"
#include<QLabel>
#include<QFont>
#include<QVBoxLayout>

SongInfoShow::SongInfoShow(QWidget *parent, SongInfo &m_song_info) :
    QWidget(parent)
{
    this->resize(500,parent->height()-100);
    //整体垂直布局
    vLayout = new QVBoxLayout(this);
    titleShow = new QLabel(this);
    artistShow = new QLabel(this);
    disk = new diskWidget(this,this->width()/2-80);
    titleShow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    artistShow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    //disk->rotateStop();
    //disk->rotateStart();

    //QFont myfont("Microsoft YaHei", 10, 75);
    titleShow->setFont(QFont("Microsoft YaHei", 15, 75));
    artistShow->setFont(QFont("Microsoft YaHei", 10, 55));

    //更新歌曲信息
    changeSong(m_song_info);

    vLayout->addWidget(titleShow);
    vLayout->addWidget(artistShow);
    vLayout->addWidget(disk);
    vLayout->setSpacing(20);
    vLayout->setContentsMargins(20, 30, 0, 0);

    //下方按钮布局
    buttonBar = new QWidget(this);
    buttonBar->resize(this->width(),40);
    hLayout =new QHBoxLayout(buttonBar);
    vLayout->addWidget(buttonBar);

    //初始化喜欢按钮
    likeButton =new QPushButton("喜欢");
    likeButton->setFont(QFont("Microsoft YaHei", 10, 45));
    likeButton->setIcon(QIcon(":/icon/res/favourite_d.ico"));
    likeButton->setIconSize(QSize(25,25));
    likeButton->setFixedSize(QSize(80,35));
    //likeButton->setFlat(true);
    likeButton->setToolTip("喜欢");
    likeButton->setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    hLayout->addWidget(likeButton);

    //初始化收藏按钮
    starButton =new QPushButton("收藏");
    starButton->setFont(QFont("Microsoft YaHei", 10, 45));
    starButton->setIcon(QIcon(":/icon/res/addtolist.png"));
    starButton->setIconSize(QSize(25,25));
    starButton->setFixedSize(QSize(80,35));
    //starButton->setFlat(true);
    starButton->setToolTip("收藏");
    starButton->setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    hLayout->addWidget(starButton);

    //初始化更多按钮
    moreInfoButton =new QPushButton("更多");
    moreInfoButton->setFont(QFont("Microsoft YaHei", 10, 45));
    moreInfoButton->setIcon(QIcon(":/icon/res/more.png"));
    moreInfoButton->setIconSize(QSize(25,25));
    moreInfoButton->setFixedSize(QSize(80,35));
    moreInfoButton->setFlat(true);
    moreInfoButton->setToolTip("更多");
    moreInfoButton->setStyleSheet ("border:2px groove gray;border-radius:10px;padding:2px 4px;");
    hLayout->addWidget(moreInfoButton);

    vLayout->setStretchFactor(titleShow,1);
    vLayout->setStretchFactor(artistShow,1);
    vLayout->setStretchFactor(disk,6);
    vLayout->setStretchFactor(buttonBar,2);
    this->setLayout(vLayout);
    this->show();

}

SongInfoShow::SongInfoShow(QWidget *parent):
    QWidget(parent)
{

}

SongInfoShow::~SongInfoShow()
{
    delete titleShow;
    delete artistShow;
    delete disk;
}

void SongInfoShow::changeSong(SongInfo &m_song_info)
{
    titleShow->setText(m_song_info.title);
    artistShow->setText(m_song_info.artist);
    disk->changePic(m_song_info.album_cover);
}

void SongInfoShow::diskRotateStart()
{
    disk->rotateStart();
}

void SongInfoShow::diskRotateStop()
{
    disk->rotateStop();
}
