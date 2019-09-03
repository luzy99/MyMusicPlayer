//在线播放的url:"http://music.163.com/song/media/outer/url?id=xxxxx.mp3"
#include "musicplaybar.h"
#include "lyricdownload.h"
#include "lyricwidget.h"
#include<QDebug>
#include <QPalette>
#include <QApplication>

MusicPlayBar::MusicPlayBar(QWidget *parent)
    : QWidget(parent)
{
    //初始化窗体信息
    this->setAutoFillBackground(true);
    QPalette backPalette;
    backPalette.setColor(QPalette::Window,QColor(255,255,255));
    this->setPalette(backPalette);
    this->setFixedHeight(60);

    layout = new QHBoxLayout; //采用水平布局
    layout->setContentsMargins(0,0,0,0);

    //初始化上一首按钮
    previousBtn = new QPushButton;
    previousBtn->setObjectName("previousBtn");
    previousBtn->setIcon(QIcon(":/icon/res/previous.png"));
    previousBtn->setIconSize(QSize(35,35));
    previousBtn->setFixedSize(QSize(50,50));
    previousBtn->setFlat(true);
    previousBtn->setToolTip("上一首");
    previousBtn->setStyleSheet("border: none;");
    previousBtn->setAttribute(Qt::WA_Hover,true);
    previousBtn->installEventFilter(this);
    layout->addWidget(previousBtn);

    //初始化播放按钮
    playBtn = new QPushButton;
    playBtn->setObjectName("playBtn");
    playBtn->setIcon(QIcon(":/icon/res/play.png"));
    playBtn->setIconSize(QSize(45,45));
    playBtn->setFixedSize(QSize(50,50));
    playBtn->setFlat(true);
    playBtn->setToolTip("播放");
    playBtn->setStyleSheet("border: none;");
    playBtn->setAttribute(Qt::WA_Hover,true);
    playBtn->installEventFilter(this);
    layout->addWidget(playBtn);

    //初始化下一首按钮
    nextBtn = new QPushButton;
    nextBtn->setObjectName("nextBtn");
    nextBtn->setIcon(QIcon(":/icon/res/next.png"));
    nextBtn->setIconSize(QSize(35,35));
    nextBtn->setFixedSize(QSize(50,50));
    nextBtn->setFlat(true);
    nextBtn->setToolTip("下一首");
    nextBtn->setStyleSheet("border: none;");
    nextBtn->setAttribute(Qt::WA_Hover,true);
    nextBtn->installEventFilter(this);
    layout->addWidget(nextBtn);

    //初始化音乐播放的当前时间
    currentTimeLabel = new QLabel;
    currentTimeLabel->setText("0:00");
    layout->addWidget(currentTimeLabel);

    //进度条的样式表
    QString sliderStyleSheet = "\
            QSlider::add-page:Horizontal\
            {     \
               background-color: rgb(87, 97, 106);\
               height:4px;\
               border-radius: 1px;\
            }\
            QSlider::sub-page:Horizontal \
           {\
               background-color:qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:0, stop:0 rgba(255,205,217,255), stop:1 rgba(110,179,217,255));\
               height:4px;\
               border-radius: 2px;\
            }\
           QSlider::groove:Horizontal \
           {\
               background:transparent;\
               height:6px;\
                border: 1px solid #4A708B;\
                border-radius: 2px;\
           }\
           QSlider::handle:Horizontal \
           {\
               height: 30px;\
               width: 30px;\
               border-image: url(:/icon/res/playSlider.png);\
               margin: -10 -8px; \
           }" ;

    //初始化进度条
    playSlider = new QSlider;
    playSlider->setOrientation(Qt::Horizontal);
    playSlider->setEnabled(true);
    playSlider->setStyleSheet(sliderStyleSheet);
    layout->addWidget(playSlider);

    //初始化总时间
    totalTimeLabel = new QLabel;
    totalTimeLabel->setText("0:00");
    layout->addWidget(totalTimeLabel);

    //初始化静音按钮
    muteBtn = new QPushButton;
    muteBtn->setObjectName("muteBtn");
    muteBtn->setIcon(QIcon(":/icon/res/soundFull.png"));
    muteBtn->setIconSize(QSize(25,25));
    muteBtn->setFixedSize(QSize(25,25));
    muteBtn->setFlat(true);
    muteBtn->setToolTip("点击开启/关闭声音");
    muteBtn->setToolTip("静音");
    muteBtn->setStyleSheet("border: none;");
    muteBtn->setAttribute(Qt::WA_Hover,true);
    muteBtn->installEventFilter(this);
    layout->addWidget(muteBtn);

    //初始化声音滚动条
    soundSlider = new QSlider;
    soundSlider->setOrientation(Qt::Horizontal);
    soundSlider->setValue(99);
    soundSlider->setEnabled(true);
    soundSlider->setMaximumWidth(150);
    soundSlider->setStyleSheet(sliderStyleSheet);
    soundSlider->setToolTip("调整音量");
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
    showLyricsBtn->setObjectName("showLyricsBtn");
    showLyricsBtn->setIcon(QIcon(":/icon/res/lyrics.png"));
    showLyricsBtn->setIconSize(QSize(35,35));
    showLyricsBtn->setFixedSize(QSize(35,35));
    showLyricsBtn->setFlat(true);
    showLyricsBtn->setToolTip("点击显示/隐藏歌词");
    showLyricsBtn->setStyleSheet("border: none;");
    showLyricsBtn->setCheckable(true);
    showLyricsBtn->setChecked(false);
    showLyricsBtn->setAttribute(Qt::WA_Hover,true);
    showLyricsBtn->installEventFilter(this);
    layout->addWidget(showLyricsBtn);

    //初始化翻译歌词按钮
    translateBtn = new QPushButton;
    translateBtn->setObjectName("translateBtn");
    translateBtn->setIcon(QIcon(":/icon/res/translate.png"));
    translateBtn->setIconSize(QSize(35,35));
    translateBtn->setFixedSize(QSize(35,35));
    translateBtn->setFlat(true);
    translateBtn->setToolTip("点击翻译/恢复外文歌词");
    translateBtn->setStyleSheet("border: none;");
    translateBtn->setCheckable(true);
    translateBtn->setChecked(false);
    translateBtn->setAttribute(Qt::WA_Hover,true);
    translateBtn->installEventFilter(this);
    layout->addWidget(translateBtn);

    //显示倍速选择
    playSpeedBtn = new QPushButton;
    playSpeedBtn->setObjectName("playSpeedBtn");
    playSpeedBtn->setIcon(QIcon(":/icon/res/rate100.png"));
    playSpeedBtn->setIconSize(QSize(35,35));
    playSpeedBtn->setFixedSize(QSize(35,35));
    playSpeedBtn->setFlat(true);
    playSpeedBtn->setToolTip("点击切换播放速度");
    playSpeedBtn->setStyleSheet("border: none;");
    playSpeedBtn->setAttribute(Qt::WA_Hover,true);
    playSpeedBtn->installEventFilter(this);
    layout->addWidget(playSpeedBtn);
    //默认倍数为正常倍数1.0x
    currentSpeed = 1.0;

    dragLabel = new QLabel;
    dragLabel->setObjectName("dragLabel");
    QPixmap dragIcon = QPixmap(":/icon/res/dragRegion.png").scaled(20,60);
    dragLabel->setPixmap(dragIcon);
    dragLabel->setAttribute(Qt::WA_TranslucentBackground);
    dragLabel->setAttribute(Qt::WA_Hover,true);
    dragLabel->installEventFilter(this);
    dragLabel->setFixedHeight(60);
    layout->addWidget(dragLabel);

    //将写的布局添加进当前栏
    this->setLayout(layout);
    //初始化播放器和播放列表
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist(this);
    playlist->setPlaybackMode(QMediaPlaylist::Loop); //循环模式
    player->setPlaylist(playlist);

    //初始化进度条阻塞状态
    //默认刚开始时不阻塞，进度条随着position的改变而改变
    block = false;

    //连接信号与槽
    initSignalsAndSlots();
}

void MusicPlayBar::initSignalsAndSlots()
{
    //播放状态改变
    connect(player,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(onStateChanged(QMediaPlayer::State)));
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
    connect(playSlider,SIGNAL(sliderPressed()),
            this,SLOT(on_playSlider_sliderPressed()));
    connect(playSlider,SIGNAL(sliderReleased()),
            this,SLOT(on_playSlider_sliderReleased()));
    //"静音"按钮
    connect(muteBtn,SIGNAL(clicked()),
            this,SLOT(on_muteBtn_clicked()));
    //拖动音量条:
    connect(soundSlider,SIGNAL(sliderPressed()),
            this,SLOT(on_soundSlider_sliderPressed()));
    connect(soundSlider,SIGNAL(sliderReleased()),
            this,SLOT(on_soundSlider_sliderReleased()));
    connect(soundSlider,SIGNAL(valueChanged(int)),
            this,SLOT(on_soundSlider_valueChanged(int)));
    //"播放模式"按钮
    connect(playModeBtn,SIGNAL(clicked()),
            this,SLOT(on_playModeBtn_clicked()));
    //"显示底部歌词"按钮
    connect(showLyricsBtn,SIGNAL(clicked()),
            this,SLOT(on_showLyricsBtn_clicked()));
    //"翻译歌词按钮"
    connect(translateBtn,SIGNAL(clicked()),
            this,SLOT(on_translateBtn_clicked()));
    //"倍速"按钮
    connect(playSpeedBtn,SIGNAL(clicked()),
            this,SLOT(on_playSpeedBtn_clicked()));
}

//事件过滤器->用于产生按钮覆盖效果
bool MusicPlayBar::eventFilter(QObject *obj, QEvent *event)
{
    if(obj->objectName() == "previousBtn")
    {
        //过滤“上一首“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            previousBtn->setIcon(QIcon(":/icon/res/previousHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            previousBtn->setIcon(QIcon(":/icon/res/previous.png"));
        }
    }
    else if(obj->objectName() == "playBtn")
    {
        //过滤“播放&暂停“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            if(player->state() == QMediaPlayer::PlayingState)
            {
                playBtn->setIcon(QIcon(":/icon/res/pauseHover.png"));
            }
            else
            {
                playBtn->setIcon(QIcon(":/icon/res/playHover.png"));
            }
        }
        if(event->type() == QEvent::HoverLeave)
        {
            if(player->state() == QMediaPlayer::PlayingState)
            {
                playBtn->setIcon(QIcon(":/icon/res/pause.png"));
            }
            else
            {
                playBtn->setIcon(QIcon(":/icon/res/play.png"));
            }
        }
    }
    else if(obj->objectName() == "nextBtn")
    {
        //过滤“下一首“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            nextBtn->setIcon(QIcon(":/icon/res/nextHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            nextBtn->setIcon(QIcon(":/icon/res/next.png"));
        }
    }
    else if(obj->objectName() == "translateBtn")
    {
        //过滤“翻译“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            translateBtn->setIcon(QIcon(":/icon/res/translateHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            translateBtn->setIcon(QIcon(":/icon/res/translate.png"));
        }
    }
    else if(obj->objectName() == "showLyricsBtn")
    {
        //过滤“显示底部歌词“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            showLyricsBtn->setIcon(QIcon(":/icon/res/lyricsHover.png"));
        }
        if(event->type() == QEvent::HoverLeave)
        {
            showLyricsBtn->setIcon(QIcon(":/icon/res/lyrics.png"));
        }
    }
    else if(obj->objectName() == "playSpeedBtn")
    {
        //过滤“倍数“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            if(currentSpeed == 1.0)
            {
                playSpeedBtn->setIcon(QIcon(":/icon/res/rate100Hover.png"));
            }
            else if(currentSpeed == 1.5)
            {
                playSpeedBtn->setIcon(QIcon(":/icon/res/rate150Hover.png"));
            }
            else if(currentSpeed == 2.0)
            {
                playSpeedBtn->setIcon(QIcon(":/icon/res/rate200Hover.png"));
            }
            else
            {
                //currentSpeed == 0.5
                playSpeedBtn->setIcon(QIcon(":/icon/res/rate050Hover.png"));
            }
        }
        if(event->type() == QEvent::HoverLeave)
        {
                if(currentSpeed == 1.0)
                {
                    playSpeedBtn->setIcon(QIcon(":/icon/res/rate100.png"));
                }
                else if(currentSpeed == 1.5)
                {
                    playSpeedBtn->setIcon(QIcon(":/icon/res/rate150.png"));
                }
                else if(currentSpeed == 2.0)
                {
                    playSpeedBtn->setIcon(QIcon(":/icon/res/rate200.png"));
                }
                else
                {
                    //currentSpeed == 0.5
                    playSpeedBtn->setIcon(QIcon(":/icon/res/rate050.png"));
                }
         }
     }
    else if(obj->objectName() == "muteBtn")
    {
        //过滤“静音“按钮被覆盖事件
        if(event->type() == QEvent::HoverEnter)
        {
            if(!player->isMuted())
            {
                if(soundSlider->value() != 99)
                {
                    muteBtn->setIcon(QIcon(":/icon/res/soundNormalHover.png"));
                }
                else
                {
                    muteBtn->setIcon(QIcon(":/icon/res/soundFullHover.png"));
                }
            }
            else
            {
                muteBtn->setIcon(QIcon(":/icon/res/soundMuteHover.png"));
            }
        }
        if(event->type() == QEvent::HoverLeave)
        {
            if(!player->isMuted())
            {
                if(soundSlider->value() != 99)
                {
                    muteBtn->setIcon(QIcon(":/icon/res/soundNormal.png"));
                }
                else
                {
                    muteBtn->setIcon(QIcon(":/icon/res/soundFull.png"));
                }
            }
            else
            {
                muteBtn->setIcon(QIcon(":/icon/res/soundMute.png"));
            }
        }
    }
    else if(obj->objectName() == "dragLabel")
    {
        //过滤尺寸更改事件
        if(event->type() == QEvent::HoverEnter)
        {
            QCursor cur(Qt::SizeFDiagCursor);
            QApplication::setOverrideCursor(cur);
        }
        if(event->type() == QEvent::HoverLeave)
        {
            QApplication::restoreOverrideCursor();
        }
     }
    else {}

    return QWidget::eventFilter(obj,event);
}

//在播放器播放状态变化时发射，以更新界面上按钮的使能状态
void MusicPlayBar::onStateChanged(QMediaPlayer::State state)
{
    //播放器状态变化,更新按钮状态
   bool isPlaying = (state == QMediaPlayer::PlayingState);
   if(isPlaying)
   {
       playBtn->setIcon(QIcon(":/icon/res/pauseHover.png"));
       playBtn->setToolTip("播放");
   }
   else
   {
       playBtn->setIcon(QIcon(":/icon/res/playHover.png"));
       playBtn->setToolTip("暂停");
   }
}

//在文件时长变化时发射，用于跟新用户界面上文件时间长度的显示
//同时切歌时需要调用爬虫更新作者和封面的信息
void MusicPlayBar::onDurationChanged(qint64 duration)
{
    //文件时长变化，更新进度显示
       playSlider->setMaximum(int(duration));
       QDateTime time = QDateTime::fromMSecsSinceEpoch(duration);
       durationTime = time.toString("mm:ss");
       totalTimeLabel->setText(durationTime);

       QString currentFilePath = playlist->currentMedia().canonicalUrl().toString();
       //qDebug()<< playlist->mediaCount()<<playlist->currentIndex()<<playlist->currentMedia().canonicalUrl();
       emit updateAudioTagInMainWindow(currentFilePath);
}

//当前文件播放位置变化时发射，用于跟新界面上的进度条
void MusicPlayBar::onPositionChanged(qint64 position)
{
    //当前文件播放进度变化，更新进度显示
    //如果当前不是阻塞状态，进度条随位置的改变而改变
    if(!block)
    {
      //qDebug()<<"position changed";
       playSlider->setSliderPosition(int(position));
       emit positionChanged(position);

       QDateTime time = QDateTime::fromMSecsSinceEpoch(position);
       positionTime = time.toString("mm:ss");
       currentTimeLabel->setText(positionTime);
     }
}

//更新逻辑播放列表
void MusicPlayBar::onChangePlaylist(QUrl url, int behaviorIndex)
{
    if(behaviorIndex == 1)
    {
        playlist->addMedia(url);
    }
}

//外界触发放歌
void MusicPlayBar::onPlayMusic(int SongIndex)
{
    //播放对应序列的歌曲
        playlist->setCurrentIndex(SongIndex);
        player->play();
        playBtn->setIcon(QIcon(":/icon/res/pause.png"));
}

//清空播放列表的槽函数
void MusicPlayBar::onClearMusic()
{
    playlist->clear();
}

//拖动歌词时触发的槽函数
void MusicPlayBar::onBlockSignals(bool block)
{
    //更新阻塞状态
    this->block = block;
    qint64 currentPosition = qint64(playSlider->value());
    //qDebug()<<block;
    //playSlider->blockSignals(block);
    emit positionStop(currentPosition);
}

//歌词拖动之后更新位置
void MusicPlayBar::onPositionDraggedTo(qint64 newPosition)
{
    //qDebug()<<"enter positionDraggedTo";
    //将播放的歌曲位置同步到拖动的那句歌词上
    player->setPosition(newPosition);
}

//点击"上一首"触发的槽函数
void MusicPlayBar::on_previousBtn_clicked()
{
    //如果歌单为空->不执行
    if(playlist->isEmpty())
    {
        return;
     }

    //如果状态时随机播放,则随机出上一首
    int currentIndex;
    if(playlist->playbackMode()==QMediaPlaylist::Random)
    {
        int row=playlist->mediaCount();
        currentIndex=qrand()%row;
        playlist->setCurrentIndex(currentIndex);
    }
    //如果不是随机播放，则依照歌单的顺序进行
    else
    {
        currentIndex = playlist->currentIndex()-1;
         playlist->setCurrentIndex(currentIndex);
    }

    player->play();
}

//点击”播放&暂停”触发的槽函数
void MusicPlayBar::on_playBtn_clicked()
{
    //播放
    if(player->state() == QMediaPlayer::PlayingState )
    {
        player->pause();
        emit becomePausing();
    }
    else
    {
        if(playlist->currentIndex()<0)
        {
            playlist->setCurrentIndex(0);
        }
        player->play();
        emit becomePlaying();
    }
}

//点击"下一首"触发的槽函数
void MusicPlayBar::on_nextBtn_clicked()
{
    //如果歌单为空->不执行
    if(playlist->isEmpty())
     {
        return;
     }

    //如果状态时随机播放,则随机出下一首
    int currentIndex;
    if(playlist->playbackMode()==QMediaPlaylist::Random)
    {
        int row=playlist->mediaCount();
        currentIndex=qrand()%row;
        playlist->setCurrentIndex(currentIndex);
    }
    //如果不是随机播放，则依照歌单的顺序进行
    else
    {
        currentIndex = playlist->currentIndex()+1;
         playlist->setCurrentIndex(currentIndex);
    }

    player->play();
}

//用户开始拖动播放进度条时触发的槽函数
void MusicPlayBar::on_playSlider_sliderPressed()
{
    //修改样式表，制造选中的效果
    QString oldStyleSheet = playSlider->styleSheet();
    QString newStyleSheet = oldStyleSheet.replace("playSlider.png","playSliderClicked.png");
    playSlider->setStyleSheet(newStyleSheet);
}

//用户拖动播放进度条之后的槽函数
void MusicPlayBar::on_playSlider_sliderReleased()
{
    int newValue = playSlider->value();
    player->setPosition(newValue);
    QString oldStyleSheet = playSlider->styleSheet();
    QString newStyleSheet = oldStyleSheet.replace("playSliderClicked.png","playSlider.png");
    playSlider->setStyleSheet(newStyleSheet);
}

//点击"静音"触发的槽函数
void MusicPlayBar::on_muteBtn_clicked()
{
    //静音控制
    bool mute = player->isMuted();
    player->setMuted(!mute);
    if(mute)
    {
        if(soundSlider->value() != 99)
        {
            muteBtn->setIcon(QIcon(":/icon/res/soundNormalHover.png"));
        }
        else
        {
            muteBtn->setIcon(QIcon(":/icon/res/soundFullHover.png"));
        }
    }
    else
    {
        muteBtn->setIcon(QIcon(":/icon/res/soundMuteHover.png"));
    }
}

//用户开始拖动音量进度条时触发的槽函数
void MusicPlayBar::on_soundSlider_sliderPressed()
{
    QString oldStyleSheet = soundSlider->styleSheet();
    QString newStyleSheet = oldStyleSheet.replace("playSlider.png","playSliderClicked.png");
    soundSlider->setStyleSheet(newStyleSheet);
}

//用户拖动音量进度条之后的槽函数
void MusicPlayBar::on_soundSlider_sliderReleased()
{
    QString oldStyleSheet = soundSlider->styleSheet();
    QString newStyleSheet = oldStyleSheet.replace("playSliderClicked.png","playSlider.png");
    soundSlider->setStyleSheet(newStyleSheet);
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

//显示&隐藏底部弹幕时触发的槽函数
void MusicPlayBar::on_showLyricsBtn_clicked()
{
    //如果当前没有歌播放，不允许开启底部弹幕
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        bool allowHover = !(showLyricsBtn->isChecked());
        showLyricsBtn->setAttribute(Qt::WA_Hover,allowHover);
        if(!allowHover)
        {
            showLyricsBtn->setIcon(QIcon(":/icon/res/lyricsHover.png"));
        }
        else
        {
            showLyricsBtn->setIcon(QIcon(":/icon/res/lyrics.png"));
        }

            //发送信号,决定底部弹幕的显示或隐藏
            emit showLyricsBarrage(!allowHover);
    }
    else
    {
        return;
    }
}

//翻译&恢复歌词语言时的槽函数
void MusicPlayBar::on_translateBtn_clicked()
{
    //如果当前没有歌播放，不允许开启歌词翻译
    if(player->mediaStatus() != QMediaPlayer::NoMedia)
    {
        //开启&关闭鼠标事件过滤器
        //使得被选中时持久性改变按钮图标的样式
        //qDebug()<<"enter here";
        bool allowHover = !(translateBtn->isChecked());
        translateBtn->setAttribute(Qt::WA_Hover,allowHover);
        if(!allowHover)
        {
            translateBtn->setIcon(QIcon(":/icon/res/translateHover.png"));
        }
        else
        {
            translateBtn->setIcon(QIcon(":/icon/res/translate.png"));
        }
        emit translateChanged();
    }
    else
    {
        return;
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
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate150Hover.png"));
    }
    else if(currentSpeed == 1.5)
    {
        //如果当前是1.5x->2.0x
        currentSpeed = 2.0;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate200Hover.png"));
    }
    else if(currentSpeed == 2.0)
    {
        //如果当前是2.0x->0.5x
        currentSpeed = 0.5;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate050Hover.png"));
    }
    else if(currentSpeed == 0.5)
    {
        //如果当前是0.5x->1.0x
        currentSpeed = 1.0;
        player->setPlaybackRate(currentSpeed);
        playSpeedBtn->setIcon(QIcon(":/icon/res/rate100Hover.png"));
    }
    else
    {

    }
}

void MusicPlayBar::changeThemeColor(QColor)
{
    //改更各个组件的颜色
}



