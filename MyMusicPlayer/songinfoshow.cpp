#include "songinfoshow.h"
#include "downloadwindow.h"
#include "lyricspost.h"
#include "addintosonglist.h"
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
    QHBoxLayout *titleLayout = new QHBoxLayout;
    titleLayout->setSpacing(10);
    titleLayout->setContentsMargins(0,0,0,0);
    titleShow = new QLabel;
    titleShow->setObjectName("titleShow");
    titleShow->setFont(QFont("Microsoft YaHei", 15, 75));
    titleShow->setStyleSheet("text-align: left;");
    titleShow->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
    titleLayout->addWidget(titleShow);
    //初始化显示MV按钮
    mvButton = new QPushButton;
    mvButton->setObjectName("mvButton");
    mvButton->setFont(QFont("Microsoft YaHei", 10, 45));
    mvButton->setText("MV");
    mvButton->setFixedSize(35,20);
    mvButton->setStyleSheet ("border:none;" \
                               "padding:2px 4px;" \
                               "color: rgb(25,25,25);" \
                               "background-color: rgba(255,255,255,0.3);");
    mvButton->setAttribute(Qt::WA_Hover,true);
    mvButton->installEventFilter(this);
    mvButton->setToolTip("这首歌没有MV信息");
    mvButton->setEnabled(false);
    titleLayout->addWidget(mvButton,0,Qt::AlignLeft);
    spacingLabel = new QLabel;
    spacingLabel->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    titleLayout->addWidget(spacingLabel);

    outerLayout->addLayout(titleLayout);

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

    //初始化收藏按钮
    starButton =new QPushButton("收藏");
    starButton->setObjectName("starButton");
    starButton->setFont(QFont("Microsoft YaHei", 10, 45));
    starButton->setIcon(QIcon(":/icon/res/addtolist.png"));
    starButton->setIconSize(QSize(25,25));
    starButton->setFixedSize(QSize(85,35));
    //starButton->setFlat(true);
    starButton->setToolTip("点击添加至歌单");
    starButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                               "border-radius:10px;" \
                               "padding:2px 4px;" \
                               "color: rgb(0,0,0);" \
                               "background-color: rgba(255,255,255,0.5);");
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
    downloadButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                   "border-radius:10px;" \
                                   "padding:2px 4px;" \
                                   "color: rgb(0,0,0);" \
                                   "background-color: rgba(255,255,255,0.5);");
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
    shareButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                "border-radius:10px;" \
                                "padding:2px 4px;" \
                                "color: rgb(0,0,0);" \
                                "background-color: rgba(255,255,255,0.5);");
    shareButton->setAttribute(Qt::WA_Hover,true);
    shareButton->installEventFilter(this);
    buttonLayout->addWidget(shareButton);
    outerLayout->addLayout(buttonLayout);

    //初始化返回首页按钮
    pageBackButton =new QPushButton("返回");
    pageBackButton->setObjectName("pageBackButton");
    pageBackButton->setFont(QFont("Microsoft YaHei", 10, 45));
    pageBackButton->setIcon(QIcon(":/icon/res/exitSongInfo.png"));
    pageBackButton->setIconSize(QSize(25,25));
    pageBackButton->setFixedSize(QSize(85,35));
    //pageBackButton->setFlat(true);
    pageBackButton->setToolTip("点击返回首页");
    pageBackButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                               "border-radius:10px;" \
                               "padding:2px 4px;" \
                               "color: rgb(0,0,0);" \
                               "background-color: rgba(255,255,255,0.5);");
    pageBackButton->setAttribute(Qt::WA_Hover,true);
    pageBackButton->installEventFilter(this);
    buttonLayout->addWidget(pageBackButton);

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
    //有MV时播放MV
    connect(mvButton,SIGNAL(clicked()),
            this,SIGNAL(playMV()));
    //点击收藏按钮添加至歌单
    connect(starButton,SIGNAL(clicked()),
            this,SLOT(on_starButton_clicked()));
    //点击下载按钮开始当前歌曲的下载
    connect(downloadButton,SIGNAL(clicked()),
            this,SLOT(on_downloadButton_clicked()));
    //点击进行歌词海报的分享
    connect(shareButton,SIGNAL(clicked()),
            this,SLOT(on_shareButton_clicked()));
    //点击返回按钮进行页面切换
    connect(pageBackButton,SIGNAL(clicked()),
            this,SLOT(on_pageBackButton_clicked()));
}

SongInfoShow::~SongInfoShow()
{
    delete titleShow;
    delete artistShow;
    delete disk;
}

//事件过滤器
bool SongInfoShow::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->objectName() == "starButton")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            starButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.7);");
        }
        if(event->type() == QEvent::HoverLeave)
        {
            starButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.5);");
        }
    }
    else if(obj->objectName() == "downloadButton")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            downloadButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.7);");
        }
        if(event->type() == QEvent::HoverLeave)
        {
            downloadButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.5);");
        }
    }
    else if(obj->objectName() == "shareButton")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            shareButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.7);");
        }
        if(event->type() == QEvent::HoverLeave)
        {
            shareButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.5);");
        }
    }
    else if(obj->objectName() == "pageBackButton")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            pageBackButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.7);");
        }
        if(event->type() == QEvent::HoverLeave)
        {
            pageBackButton->setStyleSheet ("border:2px groove rgb(0,0,0);" \
                                       "border-radius:10px;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(0,0,0);" \
                                       "background-color: rgba(255,255,255,0.5);");
        }
    }
    else if(obj->objectName() == "mvButton")
    {
        if(event->type() == QEvent::HoverEnter)
        {
            mvButton->setStyleSheet ("border:none;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(25,25,25);" \
                                       "background-color: rgba(255,255,255,0.5);");
        }
        if(event->type() == QEvent::HoverLeave)
        {
            mvButton->setStyleSheet ("border:none;" \
                                       "padding:2px 4px;" \
                                       "color: rgb(25,25,25);" \
                                       "background-color: rgba(255,255,255,0.3);");
        }
    }
    else
    {

    }

    return QWidget::eventFilter(obj,event);
}

void SongInfoShow::changeSong(SongInfo &m_song_info)
{
    titleShow->setText(m_song_info.title);
    artistShow->setText(m_song_info.artist);
    disk->changePic(m_song_info.album_cover);
    songId = m_song_info.song_id;
    songName=m_song_info.title;
    if(m_song_info.mv_id == "")
    {
        mvButton->setToolTip("这首歌没有MV信息");
        mvButton->setEnabled(false);
    }
    else
    {
        mvButton->setToolTip("点击播放MV");
        mvButton->setEnabled(true);
    }
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
    //qDebug()<<songId<<songName;
    DownloadWindow *newDownload = new DownloadWindow(songId,songName);
    newDownload->show();
}

//点击开始分享歌词海报
void SongInfoShow::on_shareButton_clicked()
{
    LyricsPost *newLyricsPost = new LyricsPost(songId);
    newLyricsPost->show();
}

//点击进行页面切换
void SongInfoShow::on_pageBackButton_clicked()
{
    emit changePage();
}

//点击添加至歌单
void SongInfoShow::on_starButton_clicked()
{
    QString listName;
    AddIntoSongList window(User);
    int status = window.exec();
    qDebug()<<status;
    if(status)
    {
        listName = window.getAddedSongList();
        qDebug()<< listName;
        emit(sendAddIntoSongListCommand(songName, listName));
    }
}

void SongInfoShow::setUser(const QString &value)
{
    User = value;
}

QPushButton *SongInfoShow::getMvButton() const
{
    return mvButton;
}

void SongInfoShow::setMvButton(QPushButton *value)
{
    mvButton = value;
}
