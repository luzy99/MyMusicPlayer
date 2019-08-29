#include "songinfoshow.h"
#include<QLabel>
#include<QFont>
#include<QVBoxLayout>

SongInfoShow::SongInfoShow(QWidget *parent, SongInfo &m_song_info) :
    QWidget(parent)
{
    QVBoxLayout *infoShow = new QVBoxLayout(this);
    titleShow = new QLabel(this);
    artistShow = new QLabel(this);
    disk = new diskWidget(this,this->width());
    titleShow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    artistShow->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Fixed);
    //disk->rotateStop();
    //disk->rotateStart();

    //QFont myfont("Microsoft YaHei", 10, 75);
    titleShow->setFont(QFont("Microsoft YaHei", 15, 75));
    artistShow->setFont(QFont("Microsoft YaHei", 10, 55));

    changeSong(m_song_info);

    infoShow->addWidget(titleShow);
    infoShow->addWidget(artistShow);
    infoShow->addWidget(disk);
    infoShow->setSpacing(20);
    infoShow->setContentsMargins(0, 30, 0, 0);
    this->setLayout(infoShow);
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
