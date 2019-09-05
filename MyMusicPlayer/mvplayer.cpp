#include "mvplayer.h"
#include <QApplication>

MvPlayer::MvPlayer(QWidget *parent)
    : QWidget(parent)
    , mediaPlayer(0, QMediaPlayer::VideoSurface)
{
    this->resize(960, 540);
    this->setStyleSheet("background-color:rgb(0,0,0);");
    //视频窗口
    QVBoxLayout *layout=new QVBoxLayout();
    videoWidget = new QVideoWidget(this);
    layout->addWidget(videoWidget);
    layout->setMargin(0);
    this->setLayout(layout);
    videoWidget->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);

    //控制条初始化
    controlBar =new QWidget(this);
    controlBar->setSizePolicy(QSizePolicy::Preferred,QSizePolicy::Preferred);
    controlBar->setGeometry(0,this->height()-30,this->width(),30);
    controlBar->setStyleSheet("background-color:rgb(55,50,50);");

    //播放按钮
    playButton = new QPushButton;
    playButton->setIcon(QIcon(":/icon/res/play.png"));
    playButton->setIconSize(QSize(45,45));
    playButton->setFixedSize(QSize(50,50));
    playButton->setFlat(true);
    playButton->setToolTip("播放");
    playButton->setStyleSheet("border: none;");
    playButton->setAttribute(Qt::WA_Hover,true);
    playButton->installEventFilter(this);

    //初始化音乐播放的当前时间
    currentTimeLabel = new QLabel;
    currentTimeLabel->setStyleSheet("color:rgb(255,255,255);");
    currentTimeLabel->setText("0:00");

    //初始化总时间
    totalTimeLabel = new QLabel;
    totalTimeLabel->setStyleSheet("color:rgb(255,255,255);");
    totalTimeLabel->setText("0:00");

    //进度条
    positionSlider = new QSlider(Qt::Horizontal);
    positionSlider->setRange(0, 0);
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

            positionSlider->setStyleSheet(sliderStyleSheet);

    //画质调节
    qualityCom =new QComboBox(controlBar);
    QString comboStyle="QComboBox \
    {\
            border: 0px;\
            width: 35px;\
            padding: 1px 2px 1px 3px;\
            font: normal bold 15px \"Microsoft YaHei\";\
            color: #ffffff;\
            background: transparent;\
}\
            QComboBox::drop-down\
    {\
            border:0px;\
            border-style: none;}\
            QComboBox QAbstractItemView\
    {\
            height:25px;\
            background:rgb(50,50,50);\
            color:#ffcccc;\
            border:0px;\
}";

            qualityCom->setStyleSheet(comboStyle);
    qualityCom->setToolTip("切换画质");

    //全屏按钮
    fullScreenBtn = new QPushButton;
    fullScreenBtn->setIcon(QIcon(":/icon/res/fullscreen.png"));
    fullScreenBtn->setIconSize(QSize(20,20));
    fullScreenBtn->setFixedSize(QSize(30,30));
    fullScreenBtn->setFlat(true);
    fullScreenBtn->setToolTip("全屏");
    fullScreenBtn->setStyleSheet("border: none;");
    fullScreenBtn->setFocusPolicy(Qt::NoFocus);
    fullScreenBtn->setAttribute(Qt::WA_Hover,true);

    //水平布局
    QHBoxLayout *controlLayout = new QHBoxLayout();
    controlLayout->setMargin(0);
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(currentTimeLabel);
    controlLayout->addWidget(totalTimeLabel);
    controlLayout->addWidget(positionSlider);
    controlLayout->addWidget(qualityCom);
    controlLayout->addWidget(fullScreenBtn);
    controlBar->setLayout(controlLayout);
    controlBar->show();

    mediaPlayer.setVideoOutput(videoWidget);//设置视频播放窗口
    //this->setLayout(vlayout);

    //状态显示标签
    statusLabel =new QLabel(this);
    statusLabel->setStyleSheet("QLabel\
    {\
                                   color:white;\
                                   background-color:rgb(50,50,50);\
                                   border: 0px;\
                                   font: 18px \"Microsoft YaHei\";\
     }");

          statusLabel->hide();

    this->setWindowTitle("暂无视频播放");//默认标题
    playButton->setEnabled(true);

    timer =new QTimer(this);
    timer->start(10000);

    this->setMouseTracking(true);
    videoWidget->setMouseTracking(true);//设置鼠标追踪
    this->setFocus();
    this->setFocusPolicy(Qt::StrongFocus);

    initSignalsAndSlots();//连接信号
    this->show();
}

bool MvPlayer::getMvUrls(QString mv_id)
{
    if(mv_id.isEmpty())//若id不存在
    {
        qDebug()<<"id不存在!";
        return 0;
    }
    else
    {
        mvId=mv_id;
        QString url = "http://music.163.com/api/mv/detail?id="
                +mvId;
        //构造请求
        QNetworkRequest request;
        request.setUrl(QUrl(url));
        //构造网络管理
        QNetworkAccessManager* manager = new QNetworkAccessManager;
        // 发送请求
        QNetworkReply *pReply = manager->get(request);
        //设置事件循环，等待资源下载完毕
        QEventLoop eventLoop;
        QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
        eventLoop.exec();

        //检测http请求的状态码
        int nHttpCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
        qDebug()<<"mv播放地址解析："<<nHttpCode;

        QByteArray response = pReply->readAll();
        QString responseStr=response;
        //qDebug()<<responseStr;

        QJsonParseError error;
        QJsonDocument jsonResult =QJsonDocument::fromJson(responseStr.toUtf8(),&error);

        if (error.error == QJsonParseError::NoError) // 解析未发生错误
        {
            if(jsonResult.isObject())//是对象
            {
                qDebug()<<"MV info toJson SUCCESS";
                QJsonObject data =jsonResult.object().value("data").toObject();

                mvTitle =data.value("name").toString();             //标题
                mvArtist=data.value("artistName").toString();   //歌手

                QJsonObject urls =data.value("brs").toObject();
                QString key;
                foreach (key, urls.keys())//将json数据搬入QMap
                {
                    urlMap[key]=urls.value(key).toString();
                }
                qDebug()<<urlMap;
                qualityCom->addItems(urlMap.keys());//设置菜单信息
                QString title=mvTitle+" - "+mvArtist;//设置窗口标题
                this->setWindowTitle(title);
                return 1;
            }
        }
        return 0;
    }

}

void MvPlayer::play()
{
    switch(mediaPlayer.state())
    {
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}

void MvPlayer::setPosition(int position)
{
    mediaPlayer.setPosition(position);
}

void MvPlayer::mediaStateChanged(QMediaPlayer::State state)
{
    switch(state) {
    case QMediaPlayer::PlayingState:
        playButton->setIcon(QIcon(":/icon/res/pause.png"));
        break;
    default:
        playButton->setIcon(QIcon(":/icon/res/play.png"));
        break;
    }
}

void MvPlayer::positionChanged(qint64 position)
{
    positionSlider->setValue(position);
    QDateTime time =QDateTime::fromMSecsSinceEpoch(position);
    QString positionTime = time.toString("mm:ss");
    currentTimeLabel->setText(positionTime);
}

void MvPlayer::durationChanged(qint64 duration)
{
    positionSlider->setRange(0, duration);
    QDateTime time =QDateTime::fromMSecsSinceEpoch(duration);
    QString durationTime = time.toString("/mm:ss");
    totalTimeLabel->setText(durationTime);

}

void MvPlayer::resizeEvent(QResizeEvent *ev)
{
    controlBar->setGeometry(0,this->height()-50,this->width(),50);
    statusLabel->setGeometry(this->width()/2-25,this->height()/2-20,0,0);
    statusLabel->adjustSize();
    return QWidget::resizeEvent(ev);
}

void MvPlayer::onComboBoxChanged(const QString &key)
{
    QString url = urlMap.value(key);
    qDebug()<<QUrl(url).isValid();
    mediaPlayer.setMedia(QUrl(url));

}

void MvPlayer::mouseMoveEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    //qDebug()<<"move";
    timer->start(3000);
    controlBar->show();
    this->setCursor(Qt::ArrowCursor);//显示鼠标
}

void MvPlayer::mouseDoubleClickEvent(QMouseEvent *event)
{
    if(!isFullScreen())
    {
        fullScreenBtn->setIcon(QIcon(":/icon/res/exitfullscreen.png"));
        fullScreenBtn->setToolTip("全屏");
        this->showFullScreen();
    }
    else
    {
        fullScreenBtn->setIcon(QIcon(":/icon/res/fullscreen.png"));
        fullScreenBtn->setToolTip("退出全屏");
        this->showNormal();
    }
    event->accept();
}

void MvPlayer::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)//鼠标左键单击播放
    {
        if(event->pos().y()<this->height()-controlBar->height())//判断鼠标不在进度条上
        {
            play();
        }
    }
}

void MvPlayer::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Escape)//esc退出全屏
    {
        this->showNormal();
    }
    else if(event->key()==Qt::Key_Space)//空格播放暂停
    {
        play();
    }
    else if(event->key()==Qt::Key_Right)//→快进5秒
    {
        qint64 position =mediaPlayer.position();
        mediaPlayer.setPosition(position+5000);
        timer->start(3000);
        controlBar->show();
    }
    else if(event->key()==Qt::Key_Left)//←快退5秒
    {
        qint64 position =mediaPlayer.position();
        mediaPlayer.setPosition(position-5000);
        timer->start(3000);
        controlBar->show();
    }
    else if(event->key()==Qt::Key_Up)//音量+10%
    {
        int volume=mediaPlayer.volume()+10;
        mediaPlayer.setVolume(volume);
        //qDebug()<<mediaPlayer.volume();
        QString icon;
        if(volume>=60)//音量图标
        {
            icon="🔊";
        }
        else if(volume<=0)
        {
            icon="🔈";
        }
        else
        {
            icon="🔉";
        }
        QString labelInfo=icon+" "+QString::number(mediaPlayer.volume())+"%";
        statusLabel->setText(labelInfo);
        statusLabel->setGeometry(this->width()/2-25,this->height()/2-20,0,0);
        statusLabel->adjustSize();
        statusLabel->show();
    }
    else if(event->key()==Qt::Key_Down)//音量-10%
    {
        int volume=mediaPlayer.volume()-10;
        mediaPlayer.setVolume(volume);
        //qDebug()<<mediaPlayer.volume();
        QString icon;
        if(volume>=60)
        {
            icon="🔊";
        }
        else if(volume<=0)
        {
            icon="🔈";
        }
        else
        {
            icon="🔉";
        }
        QString labelInfo=icon+" "+QString::number(mediaPlayer.volume())+"%";
        statusLabel->setText(labelInfo);
        statusLabel->setGeometry(this->width()/2-25,this->height()/2-20,0,0);
        statusLabel->adjustSize();
        statusLabel->show();
    }
}

void MvPlayer::wheelEvent(QWheelEvent *event)
{
    if(event->delta()>0)
    {
        QKeyEvent upKey(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        QCoreApplication::sendEvent(this, &upKey);
    }
    else
    {
        QKeyEvent downKey(QEvent::KeyPress, Qt::Key_Down, Qt::NoModifier);
        QCoreApplication::sendEvent(this, &downKey);
    }
}

void MvPlayer::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event);
    mediaPlayer.stop();
}

void MvPlayer::hideBar()
{
    controlBar->hide();
    statusLabel->hide();
    if(this->isFullScreen())//全屏时隐藏鼠标
    {
        this->setCursor(Qt::BlankCursor);
    }
}

void MvPlayer::handleError()//错误处理
{
    playButton->setEnabled(false);
    const QString errorString = mediaPlayer.errorString();
    QString message = "Error: ";
    if (errorString.isEmpty())
    {
        message += " #" + QString::number(int(mediaPlayer.error()));
    }
    else
        message += errorString;
    statusLabel->setText(message);
    statusLabel->setGeometry(this->width()/2-25,this->height()/2-20,0,0);
    statusLabel->adjustSize();
    timer->stop();
    statusLabel->show();
}

void MvPlayer::initSignalsAndSlots()
{
    connect(playButton, SIGNAL(clicked(bool)),this, SLOT(play()));//播放按钮
    connect(positionSlider, SIGNAL(sliderMoved(int)),this, SLOT(setPosition(int)));//滑动条
    connect(&mediaPlayer, SIGNAL(stateChanged(QMediaPlayer::State)),
            this, SLOT(mediaStateChanged(QMediaPlayer::State)));    //播放状态
    connect(&mediaPlayer, SIGNAL(positionChanged(qint64)),
            this, SLOT(positionChanged(qint64)));//进度条位置
    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)),
            this, SLOT(durationChanged(qint64)));//进度条时长
    connect(qualityCom,SIGNAL(currentIndexChanged(QString)),
            this,SLOT(onComboBoxChanged(QString)));//切换画质
    connect(timer ,SIGNAL(timeout()), this, SLOT(hideBar()));//定时隐藏进度条
    connect(fullScreenBtn,SIGNAL(clicked()),
            this,SLOT(on_fullScreenBtn_clicked()));//全屏
    typedef void (QMediaPlayer::*ErrorSignal)(QMediaPlayer::Error);
    connect(&mediaPlayer, static_cast<ErrorSignal>(&QMediaPlayer::error),
            this, &MvPlayer::handleError);//错误信息
}

void MvPlayer::on_fullScreenBtn_clicked()
{
    if(this->isFullScreen())//全屏状态
    {
        fullScreenBtn->setIcon(QIcon(":/icon/res/fullscreen.png"));
        fullScreenBtn->setToolTip("全屏");
        this->showNormal();
    }
    else//非全屏状态
    {
        fullScreenBtn->setIcon(QIcon(":/icon/res/exitfullscreen.png"));
        fullScreenBtn->setToolTip("退出全屏");
        this->showFullScreen();
    }
}
