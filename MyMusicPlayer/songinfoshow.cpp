#include "songinfoshow.h"
#include "downloadwindow.h"
#include<QLabel>
#include<QFont>
#include<QVBoxLayout>

SongInfoShow::SongInfoShow(SongInfo &m_song_info,QWidget *parent)
    : QWidget(parent),
      songId(m_song_info.song_id),
      songName(m_song_info.title)
{
    //设置改窗体的属性
    this->resize(parent->width()/2,parent->height());

    //整体垂直布局
    outerLayout = new QVBoxLayout;
    outerLayout->setSpacing(10);
    outerLayout->setContentsMargins(30, 30, 0, 60);

    //初始化音乐标题标签
    //QFont myfont("Microsoft YaHei", 10, 75);
    titleShow = new QLabel;
    titleShow->setObjectName("titleShow");
    titleShow->setFont(QFont("Microsoft YaHei", 15, 75));
    titleShow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    outerLayout->addWidget(titleShow);

    //初始化音乐作者标签
    artistShow = new QLabel;
    artistShow->setObjectName("artistShow");
    artistShow->setFont(QFont("Microsoft YaHei", 10, 55));
    artistShow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    outerLayout->addWidget(artistShow);

    //初始化专辑转盘
    disk = new diskWidget(this,this->width()/2-100);
    outerLayout->addWidget(disk);

    //更新三者的信息
    changeSong(m_song_info);

    //下方按钮布局
    buttonLayout = new QHBoxLayout;
    buttonLayout->setMargin(0);
    buttonLayout->setSpacing(15);
    buttonLayout->setAlignment(Qt::AlignLeft);
    //初始化喜欢按钮
    likeButton =new QPushButton("喜欢");
    likeButton->setObjectName("likeButton");
    likeButton->setFont(QFont("Microsoft YaHei", 10, 45));
    likeButton->setIcon(QIcon(":/icon/res/favourite_d.ico"));
    likeButton->setIconSize(QSize(25,25));
    likeButton->setFixedSize(QSize(85,35));
    //likeButton->setFlat(true);
    likeButton->setToolTip("点击添加为喜欢的歌");
    likeButton->setStyleSheet ("border:2px groove gray;" \
                               "border-radius:10px;" \
                               "padding:2px 4px;" \
                               "color: #dbdbdb;");
    likeButton->setAttribute(Qt::WA_Hover,true);
    likeButton->installEventFilter(this);
    buttonLayout->addWidget(likeButton);
    //初始化收藏按钮
    starButton =new QPushButton("收藏");
    starButton->setObjectName("starButton");
    starButton->setFont(QFont("Microsoft YaHei", 10, 45));
    starButton->setIcon(QIcon(":/icon/res/addtolist.png"));
    starButton->setIconSize(QSize(25,25));
    starButton->setFixedSize(QSize(85,35));
    //starButton->setFlat(true);
    starButton->setToolTip("点击添加至歌单");
    starButton->setStyleSheet ("border:2px groove gray;" \
                               "border-radius:10px;" \
                               "padding:2px 4px;" \
                               "color: #dbdbdb;");
    starButton->setAttribute(Qt::WA_Hover,true);
    starButton->installEventFilter(this);
    buttonLayout->addWidget(starButton);
    //初始化下载按钮
    downloadButton =new QPushButton("下载");
    downloadButton->setObjectName("downloadButton");
    downloadButton->setFont(QFont("Microsoft YaHei", 10, 45));
    downloadButton->setIcon(QIcon(":/icon/res/saveSong.png"));
    downloadButton->setIconSize(QSize(25,25));
    downloadButton->setFixedSize(QSize(85,35));
    //starButton->setFlat(true);
    downloadButton->setToolTip("点击下载歌曲");
    downloadButton->setStyleSheet ("border:2px groove gray;" \
                               "border-radius:10px;" \
                               "padding:2px 4px;" \
                               "color: #dbdbdb;");
    downloadButton->setAttribute(Qt::WA_Hover,true);
    downloadButton->installEventFilter(this);
    buttonLayout->addWidget(downloadButton);
    //初始化分享按钮
    shareButton =new QPushButton("分享");
    shareButton->setObjectName("shareButton");
    shareButton->setFont(QFont("Microsoft YaHei", 10, 45));
    shareButton->setIcon(QIcon(":/icon/res/share.png"));
    shareButton->setIconSize(QSize(25,25));
    shareButton->setFixedSize(QSize(85,35));
    //starButton->setFlat(true);
    shareButton->setToolTip("点击分享歌词海报");
    shareButton->setStyleSheet ("border:2px groove gray;" \
                               "border-radius:10px;" \
                               "padding:2px 4px;" \
                               "color: #dbdbdb;");
    shareButton->setAttribute(Qt::WA_Hover,true);
    shareButton->installEventFilter(this);
    buttonLayout->addWidget(shareButton);
    outerLayout->addLayout(buttonLayout);

//    vLayout->setStretchFactor(titleShow,1);
//    vLayout->setStretchFactor(artistShow,1);
//    vLayout->setStretchFactor(disk,6);
//    vLayout->setStretchFactor(buttonBar,2);
    this->setLayout(outerLayout);

    //关联信号与槽
    initSignalsAndSlots();
}

SongInfoShow::SongInfoShow(QWidget *parent):
    QWidget(parent)
{

}

//初始化信号与槽
void SongInfoShow::initSignalsAndSlots()
{
    //点击下载按钮开始当前歌曲的下载
    connect(downloadButton,SIGNAL(clicked()),
            this,SLOT(on_downloadButton_clicked()));
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
    songId = m_song_info.song_id;
    songName=m_song_info.title;
}

void SongInfoShow::diskRotateStart()
{
    disk->rotateStart();
}

void SongInfoShow::diskRotateStop()
{
    disk->rotateStop();
}

//点击开始下载歌曲
void SongInfoShow::on_downloadButton_clicked()
{
    qDebug()<<songId<<songName;
    DownloadWindow *newDownload = new DownloadWindow(songId,songName);
    newDownload->show();
}
