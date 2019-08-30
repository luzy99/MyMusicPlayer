#include "musicplaybar.h"
#include<QDebug>
#include <QPalette>
MusicPlayBar::MusicPlayBar(QWidget *parent)
    : QWidget(parent)
{
    layout = new QHBoxLayout(parent); //采用水平布局

    //初始化上一首按钮
    previousBtn = new QPushButton;
    previousBtn->setIcon(QIcon(":/icon/res/previous.png"));
    previousBtn->setIconSize(QSize(35,35));
    previousBtn->setFixedSize(QSize(50,50));
    previousBtn->setFlat(true);
    previousBtn->setToolTip("上一首");
    layout->addWidget(previousBtn);

    //初始化播放按钮
    playBtn = new QPushButton;
    playBtn->setIcon(QIcon(":/icon/res/play.png"));
    playBtn->setIconSize(QSize(45,45));
    playBtn->setFixedSize(QSize(50,50));
    playBtn->setFlat(true);
    playBtn->setToolTip("播放");
    layout->addWidget(playBtn);

    //初始化下一首按钮
    nextBtn = new QPushButton;
    nextBtn->setIcon(QIcon(":/icon/res/next.png"));
    nextBtn->setIconSize(QSize(35,35));
    nextBtn->setFixedSize(QSize(50,50));
    nextBtn->setFlat(true);
    nextBtn->setToolTip("下一首");
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
    muteBtn->setIconSize(QSize(25,25));
    muteBtn->setFixedSize(QSize(25,25));
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
    playModeBtn->setIcon(QIcon(":/icon/res/modeSequence.png"));
    playModeBtn->setIconSize(QSize(25,25));
    playModeBtn->setFixedSize(QSize(25,25));
    playModeBtn->setFlat(true);
    playModeBtn->setToolTip("列表循环");
    layout->addWidget(playModeBtn);

    //显示歌词按钮
    showLyricsBtn = new QPushButton;
    showLyricsBtn->setIcon(QIcon(":/icon/res/lyrics.png"));
    showLyricsBtn->setIconSize(QSize(35,35));
    showLyricsBtn->setFixedSize(QSize(35,35));
    showLyricsBtn->setFlat(true);
    showLyricsBtn->setToolTip("点击显示/隐藏歌词");
    layout->addWidget(showLyricsBtn);

    //显示倍速选择
    playSpeedBtn = new QPushButton;
    playSpeedBtn->setIcon(QIcon(":/icon/res/rate100.png"));
    playSpeedBtn->setIconSize(QSize(35,35));
    playSpeedBtn->setFixedSize(QSize(35,35));
    playSpeedBtn->setFlat(true);
    playSpeedBtn->setToolTip("点击切换播放速度");
    layout->addWidget(playSpeedBtn);
    currentSpeed = 1.0;

    //将写的布局添加进当前栏
    this->setLayout(layout);
    //初始化播放器和播放列表
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop); //循环模式
    player->setPlaylist(playlist);

    //连接信号与槽
    initSignalsAndSlots();


}

void MusicPlayBar::initSignalsAndSlots()
{
    //播放位置改变
    connect(player,SIGNAL(positionChanged(qint64)),
            this,SLOT(onPositionChanged(qint64)));
    //切歌
    connect(player,SIGNAL(durationChanged(qint64)),
            this,SLOT(onDurationChanged(qint64)));
    //"上一首"按钮
    connect(previousBtn,SIGNAL(clicked()),
            this,SLOT(on_previousBtn_clicked()));
    //”播放&暂停”按钮
    connect(playBtn,SIGNAL(clicked()),
            this,SLOT(on_playBtn_clicked()));
    //"下一首"按钮
    connect(nextBtn,SIGNAL(clicked()),
           this,SLOT(on_nextBtn_clicked()));
    //拖动播放进度条:
    connect(playSlider,SIGNAL(sliderReleased()),
            this,SLOT(on_playSlider_sliderReleased()));
    //"静音"按钮
    connect(muteBtn,SIGNAL(clicked()),
            this,SLOT(on_muteBtn_clicked()));
    //拖动音量条:
    connect(soundSlider,SIGNAL(valueChanged(int)),
            this,SLOT(on_soundSlider_valueChanged(int)));
    //"播放模式"按钮
    connect(playModeBtn,SIGNAL(clicked()),
            this,SLOT(on_playModeBtn_clicked()));
    //"倍速"按钮
    connect(playSpeedBtn,SIGNAL(clicked()),
            this,SLOT(on_playSpeedBtn_clicked()));
}




//在播放器播放状态变化时发射，以更新界面上按钮的使能状态
void MusicPlayBar::onStateChanged(QMediaPlayer::State state)
{
    //播放器状态变化,更新按钮状态
   bool isPlaying = (state == QMediaPlayer::PlayingState);
   if(isPlaying)
   {
       playBtn->setIcon(QIcon(":/icon/res/play.png"));
       playBtn->setToolTip("播放");
   }
   else
   {
       playBtn->setIcon(QIcon(":/icon/res/pause.png"));
       playBtn->setToolTip("暂停");
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
       durationTime = QString::asprintf("%d:%d",mins,secs);
       totalTimeLabel->setText(durationTime);
}

//当前文件播放位置变化时发射，用于跟新界面上的进度条
void MusicPlayBar::onPositionChanged(qint64 position)
{
    //当前文件播放进度变化，更新进度显示
       playSlider->setSliderPosition(int(position));
       int secs = int(position)/1000; //秒
       int mins =  secs/60; //分钟
       secs = secs%60; //余秒数
       positionTime = QString::asprintf("%d:%d",mins,secs);
       currentTimeLabel->setText(positionTime);
}

void MusicPlayBar::onChangePlaylist(QUrl url, int behaviorIndex)
{
    if(behaviorIndex == 1)
    {
        playlist->addMedia(url);
    }
}

void MusicPlayBar::onPlayMusic(int SongIndex)
{
        playlist->setCurrentIndex(SongIndex);
        player->play();
        playBtn->setIcon(QIcon(":/icon/res/pause.png"));
}

//点击"上一首"触发的槽函数
void MusicPlayBar::on_previousBtn_clicked()
{
    int curIndex = playlist->currentIndex();
    playlist->setCurrentIndex(curIndex-1);
    player->play();
}

//点击”播放&暂停”触发的槽函数
void MusicPlayBar::on_playBtn_clicked()
{
    //播放
    if(player->state() == QMediaPlayer::PlayingState )
    {
        player->pause();
        playBtn->setIcon(QIcon(":/icon/res/play.png"));
        playBtn->setToolTip("播放");
    }
    else
    {
        if(playlist->currentIndex()<0)
        {
            playlist->setCurrentIndex(0);
        }
        player->play();
        playBtn->setIcon(QIcon(":/icon/res/pause.png"));
        playBtn->setToolTip("暂停");
    }
}

//点击"下一首"触发的槽函数
void MusicPlayBar::on_nextBtn_clicked()
{
    int curIndex = playlist->currentIndex();
    playlist->setCurrentIndex(curIndex+1);
    player->play();
}

//用户拖动进度条之后的槽函数
void MusicPlayBar::on_playSlider_sliderReleased()
{
    int newValue = playSlider->value();
    player->setPosition(newValue);
}

//点击"静音"触发的槽函数
void MusicPlayBar::on_muteBtn_clicked()
{
    //静音控制
    bool mute = player->isMuted();
    player->setMuted(!mute);
    if(mute)
    {
        muteBtn->setIcon(QIcon(":/icon/res/soundNormal.png"));
    }
    else
    {
        muteBtn->setIcon(QIcon(":/icon/res/soundMute.png"));
    }
}

//歌曲滑条被拖动时触发的槽函数
void MusicPlayBar::on_soundSlider_valueChanged(int value)
{
    //更改图标
    if(value == 0)
    {
        muteBtn->setIcon(QIcon(":/icon/res/soundMute.png"));
    }
    else if(value == 99)
    {
        muteBtn->setIcon(QIcon(":/icon/res/soundFull.png"));
    }
    else
    {
        muteBtn->setIcon(QIcon(":/icon/res/soundNormal.png"));
    }

    player->setVolume(value);
}

//切换播放模式时触发的槽函数
void MusicPlayBar::on_playModeBtn_clicked()
{
    int curMode = playlist->playbackMode();
    switch (curMode)
    {
    //如果当前是单曲循环->列表循环
    case 1:
        playlist->setPlaybackMode(QMediaPlaylist::Loop);
        playModeBtn->setIcon(QIcon(":/icon/res/modeSequence.png"));
        playModeBtn->setToolTip("列表循环");
        break;
    //如果当前是列表循环->列表随机
    case 3:
        playlist->setPlaybackMode(QMediaPlaylist::Random);
        playModeBtn->setIcon(QIcon(":/icon/res/modeRandom.png"));
        playModeBtn->setToolTip("列表随机");
        break;
    case 4:
    //如果当前是列表随机->单曲循环
        playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        playModeBtn->setIcon(QIcon(":/icon/res/modeLoop.png"));
        playModeBtn->setToolTip("单曲循环");
        break;
    }
}

//切换倍数时触发的槽函数
void MusicPlayBar::on_playSpeedBtn_clicked()
{
    if(currentSpeed == 1.0)
    {
        //如果当前是1.0x->1.5x
        currentSpeed = 1.5;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate150.png"));
    }
    else if(currentSpeed == 1.5)
    {
        //如果当前是1.5x->2.0x
        currentSpeed = 2.0;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate200.png"));
    }
    else if(currentSpeed == 2.0)
    {
        //如果当前是2.0x->0.5x
        currentSpeed = 0.5;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate050.png"));
    }
    else if(currentSpeed == 0.5)
    {
        //如果当前是0.5x->1.0x
        currentSpeed = 1.0;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate100.png"));
    }
    else {}
}


void MusicPlayBar::changeThemeColor(QColor)
{
    //改更各个组件的颜色
}



