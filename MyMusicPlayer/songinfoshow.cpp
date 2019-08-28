#include "songinfoshow.h"
#include "ui_songinfoshow.h"
#include<QLabel>
#include<QFont>

SongInfoShow::SongInfoShow(QWidget *parent, SongInfo &m_song_info) :
    QWidget(parent),
    ui(new Ui::SongInfoShow)
{
    ui->setupUi(this);
    QVBoxLayout *infoShow = new QVBoxLayout(this);
    titleShow = new QLabel(this);
    artistShow = new QLabel(this);
    disk = new diskWidget(this,this->width()/2-60);

    titleShow->setGeometry(0,30,200,30);
    artistShow->setGeometry(0,60,200,30);
    disk->setGeometry(0,100,250,250);

    //disk->rotateStop();
    //disk->rotateStart();

    //QFont myfont("Microsoft YaHei", 10, 75);
    titleShow->setFont(QFont("Microsoft YaHei", 15, 75));
    artistShow->setFont(QFont("Microsoft YaHei", 10, 55));

    changeSong(m_song_info);

    infoShow->addWidget(titleShow);
    infoShow->addWidget(artistShow);
    infoShow->addWidget(disk);
    infoShow->addStretch();
    infoShow->setSpacing(0);
    infoShow->setContentsMargins(0, 0, 0, 0);
    this->setLayout(infoShow);
    this->show();

}

SongInfoShow::SongInfoShow(QWidget *parent):
    QWidget(parent),
    ui(new Ui::SongInfoShow)
{
    ui->setupUi(this);
}

SongInfoShow::~SongInfoShow()
{
    delete ui;
}

SongInfoShow::changeSong(SongInfo &m_song_info)
{
    titleShow->setText(QString::fromStdWString(m_song_info.title));
    artistShow->setText(QString::fromStdWString(m_song_info.artist));
    disk->changePic(QString::fromStdWString(m_song_info.album_cover));
}
