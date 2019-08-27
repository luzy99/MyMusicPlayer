#include "musicplaybar.h"
#include <QHBoxLayout>
#include <QPalette>

MusicPlayBar::MusicPlayBar(QWidget *parent)
    : QWidget(parent)
    ,themeColor(205,10,10)
{
    QHBoxLayout *layout = new QHBoxLayout; //采用水平布局

    //初始化上一首按钮
    previousBtn = new QPushButton;
    previousBtn->setIcon(QIcon(":/icon/res/previous.png"));
    previousBtn->setIconSize(QSize(50,50));
    previousBtn->setFlat(true);
    layout->addWidget(previousBtn);

    //初始化播放按钮
    playBtn = new QPushButton;
    playBtn->setIcon(QIcon(":/icon/res/play.png"));
    playBtn->setIconSize(QSize(50,50));
    playBtn->setFlat(true);
    layout->addWidget(playBtn);

    //初始化下一首按钮
    nextBtn = new QPushButton;
    nextBtn->setIcon(QIcon(":/icon/res/next.png"));
    nextBtn->setIconSize(QSize(50,50));
    nextBtn->setFlat(true);
    layout->addWidget(nextBtn);

    //初始化音乐播放的当前时间
    currentTimeLabel = new QLabel;
    currentTimeLabel->setText("0:00");
    layout->addWidget(currentTimeLabel);

    //初始化进度条
    playSlider = new QSlider;
    playSlider->setOrientation(Qt::Horizontal);
    playSlider->setEnabled(true);
    layout->addWidget(playSlider);

    //初始化总时间
    totalTimeLabel = new QLabel;
    totalTimeLabel->setText("0:00");
    layout->addWidget(totalTimeLabel);

    //初始化静音按钮
    muteBtn = new QPushButton;
    muteBtn->setIcon(QIcon(":/icon/res/soundFull.png"));
    muteBtn->setIconSize(QSize(30,30));
    muteBtn->setFlat(true);
    layout->addWidget(muteBtn);

    //初始化声音滚动条
    soundSlider = new QSlider;
    soundSlider->setOrientation(Qt::Horizontal);
    soundSlider->setValue(99);
    soundSlider->setEnabled(true);
    soundSlider->setMaximumWidth(150);
    layout->addWidget(soundSlider);

    //初始化播放模式按钮
    playModeBtn = new QPushButton;
    playModeBtn->setIcon(QIcon(":/icon/res/modeLoop.png"));
    playModeBtn->setIconSize(QSize(50,50));
    playModeBtn->setFlat(true);
    layout->addWidget(playModeBtn);

    //显示歌词按钮
    showLyricsBtn = new QPushButton;
    showLyricsBtn->setIcon(QIcon(":/icon/res/lyrics.png"));
    showLyricsBtn->setIconSize(QSize(50,50));
    showLyricsBtn->setFlat(true);
    layout->addWidget(showLyricsBtn);

    //显示倍速选择
    playSpeed = new QPushButton;
    playSpeed->setIcon(QIcon(":/icon/res/rate100.png"));
    playSpeed->setIconSize(QSize(50,50));
    playSpeed->setFlat(true);
    layout->addWidget(playSpeed);

    //将写的布局添加进当前栏
    this->setLayout(layout);

    //修改当前组件的颜色
    QPalette pt = this->palette();
    pt.setColor(QPalette::Window,themeColor);
    this->setPalette(pt);

    //初始化播放器和播放列表
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop); //循环模式
    player->setPlaylist(playlist);

    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(onStateChanged(QMediaPlayer::State)));
    connect(player,SIGNAL(positionChanged(qint64)),
            this,SLOT(onPositionChanged(qint64)));
    connect(player,SIGNAL(durationChanged(qint64)),
            this,SLOT(onDurationChanged(qint64)));
    connect(playlist,SIGNAL(currentIndexChanged(int)),
            this,SLOT(onPlaylistChanged(int)));
}

//在播放器播放状态变化时发射，以更新界面上按钮的使能状态
void MusicPlayBar::onStateChanged(QMediaPlayer::State state)
{
    //播放器状态变化,更新按钮状态
   bool isPlaying = (state == QMediaPlayer::PlayingState);
   if(isPlaying)
   {
       playBtn->setIcon(QIcon(":/icon/res/play.png"));
   }
   else {
       playBtn->setIcon(QIcon(":/icon/res/pause.png"));
   }
}

//在文件时长变化时发射，用于跟新用户界面上文件时间长度的显示
void MusicPlayBar::onDurationChanged(qint64 duration)
{
    //文件时长变化，更新进度显示
       playSlider->setMaximum(int(duration));
       int secs = int(duration)/1000; //秒
       int mins =  secs/60; //分钟
       secs = secs%60; //余秒数
       QString minStr(mins);
       QString secStr;
       secStr = QString(secs);
       if(secs>10) ;
       else
       {
           secStr = "0"+secStr;
       }

       durationTime = minStr+":"+secStr;
       totalTimeLabel->setText(durationTime);
}

//当前文件播放位置变化时发射，用于跟新界面上的进度条
void MusicPlayBar::onPositionChanged(qint64 position)
{
    //当前文件播放进度变化，更新进度显示
       if(playSlider->isSliderDown())
           return; //手动状态，不处理
       playSlider->setSliderPosition(int(position));
       int secs = int(position)/1000; //秒
       int mins =  secs/60; //分钟
       secs = secs%60; //余秒数
       QString minStr(mins);
       QString secStr;
       secStr = QString(secs);
       if(secs>10) ;
       else
       {
           secStr = "0"+secStr;
       }

       positionTime = minStr+":"+secStr;
       currentTimeLabel->setText(positionTime);
}

void MusicPlayBar::changeThemeColor(QColor)
{
    //改更各个组件的颜色
}

