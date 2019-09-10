#include "mainwindow.h"
#include "songlist.h"
#include "renamesonglistdialog.h"
#include "errorwindow.h"
#include <opencv2/opencv.hpp>
#include <QDir>
#include <QVBoxLayout>
#include <QSplitter>
#include <QApplication>
#include <QBitmap>
#include <QPainter>
#include <QMimeData>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QPropertyAnimation>
#include<QDebug>
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      clickedType(No),
      userId("")
{
    //初始化主窗体属性
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("Rainbow Music Player");
    this->setWindowIcon(QIcon(":/icon/res/suspensionWindow.png"));
    this->setMinimumSize(1200,800);
    this->setStyleSheet("QToolTip{border:none; "
                        "background: rgba(255,255,255,90%);"
                        "color: rgb(25,25,25);}");
    this->setAcceptDrops(true);

    //初始化保存文件夹
    initDirectory();

    //初始化当前歌曲的信息
    currentSongInfo = new SongInfo;
    currentSongInfo->title = "暂无歌曲在播放";
    currentSongInfo->artist = "暂无相关信息";
    currentSongInfo->album_cover = QDir::currentPath()+"/CoverImages/defaultCover.png";
    //初始化歌词下载 ,默认不进行翻译
    lyricsDownloader = new LyricDownload;
    translate = false;

    //初始化搜索框的逻辑部件
    searcher = new SearchData;

    //初始化窗口布局
    QVBoxLayout *outerLayout = new QVBoxLayout;
    outerLayout->setSpacing(0);
    outerLayout->setContentsMargins(0, 0, 0, 0);//设置边距

    //初始化标题栏
    titleBar  = new TitleBar;
    titleBar->setObjectName("titleBar");
    titleBar->setWindowOpacity(0.9);
    outerLayout->addWidget(titleBar,0,Qt::AlignTop);

    //初始化主界面装载多页面切换的容器
    mainPageContainer = new AnimationStackedWidget;
    mainPageContainer->setWindowFlags(Qt::FramelessWindowHint);
    mainPageContainer->setObjectName("mainPageContainer");

    //初始化首页
    songListPage = new QWidget;
    songListPage->setWindowFlags(Qt::FramelessWindowHint);
    songListPage->setStyleSheet("background-color: rgb(250,250,250)");
    mainPageContainer->insertWidget(0,songListPage);
    QHBoxLayout *songListPageLayout = new QHBoxLayout;
    songListPageLayout->setSpacing(0);
    songListPageLayout->setContentsMargins(0,0,0,0);

    QWidget *leftWidget = new QWidget;
    QVBoxLayout *leftLayout = new QVBoxLayout;
    QSplitter *middleSplitter = new QSplitter(Qt::Vertical);
    middleSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(200, 200, 200); }");
    middleSplitter->setHandleWidth(1); //分割线的宽度
    leftLayout->setSpacing(0);
    leftLayout->setContentsMargins(0,0,0,0);
    //初始化歌单界面
    songList = new SongList;
    songList->setStyleSheet("background: rgb(245,245,245)");
    middleSplitter->addWidget(songList);
    //初始化小的歌曲显示栏
    littleSongBar = new LittleSongBar;
    middleSplitter->addWidget(littleSongBar);
    leftLayout->addWidget(middleSplitter);
    leftWidget->setLayout(leftLayout);

    QWidget *rightWidget = new QWidget;
    QVBoxLayout *rightLayout = new QVBoxLayout;
    rightLayout->setSpacing(20);
    rightLayout->setContentsMargins(15,10,10,10);
    //初始化动画滚轮
    adsWall = new AnimatedWallWG;
    adsWall->setWindowFlags(Qt::FramelessWindowHint);
    adsWall->setAttribute(Qt::WA_TranslucentBackground);
    rightLayout->addWidget(adsWall);
    //初始化本地歌曲展示
    songListsShower = new ShowLocal;
    songListsShower->setWindowFlags(Qt::FramelessWindowHint);
    songListsShower->setAttribute(Qt::WA_TranslucentBackground);
    rightLayout->addWidget(songListsShower);
    rightWidget->setLayout(rightLayout);

    //首页的分割线操作
    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal);
    mainSplitter->setStyleSheet("QSplitter::handle { background-color: rgb(200, 200, 200); }");
    mainSplitter->setHandleWidth(1); //分割线的宽度
    mainSplitter->setChildrenCollapsible(false);
    mainSplitter->addWidget(leftWidget);
    mainSplitter->addWidget(rightWidget);
    songListPageLayout->addWidget(mainSplitter);
    songListPage->setLayout(songListPageLayout);

    //初始化展示歌曲信息的页面
    songInfoPage = new QWidget;
    songInfoPage->setWindowFlags(Qt::FramelessWindowHint);
    songInfoPage->setAutoFillBackground(true);
    mainPageContainer->insertWidget(1,songInfoPage);
    QHBoxLayout *songInfoPageLayout = new QHBoxLayout;
    //初始化歌曲信息显示
    infoShow = new SongInfoShow(*currentSongInfo,this);
    infoShow->setObjectName("infoShow");
    songInfoPageLayout->addWidget(infoShow,1);
    //初始化歌词界面
    lyricsShower = new LyricWidget;
    lyricsShower->setObjectName("lyricsShower");
    lyricsShower->setWindowFlags(Qt::FramelessWindowHint);
    lyricsShower->setAttribute(Qt::WA_TranslucentBackground);
    songInfoPageLayout->addWidget(lyricsShower,1);
    songInfoPage->setLayout(songInfoPageLayout);

    //将多页容器添加至主界面
    mainPageContainer->setCurrentIndex(0);
    outerLayout->addWidget(mainPageContainer);

    //初始化自定义音乐播放栏
    musicPlayBar = new MusicPlayBar;
    musicPlayBar->setObjectName("musicPlayBar");
    outerLayout->addWidget(musicPlayBar,Qt::AlignBottom);

    //初始化底部歌词弹幕
    lyricsBarrage = new miniLyrics;
    lyricsBarrage->setObjectName("lyricsBarrage");

    //初始化悬浮小窗
    suspensionWindow = new SuspensionWindow;
    //设置窗口布局
    this->setLayout(outerLayout);

    //更新拖动区域
    updateDragRegion();
    //关联信号与槽
    initSignalsAndSlots();
}

//判断当前路径下有无下载封面歌词和歌需要的文件
//若没有则重新创建
void MainWindow::initDirectory()
{
    QDir coverFile(QDir::currentPath()+"/CoverImages");
    if(coverFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        coverFile.mkdir(QDir::currentPath()+"/CoverImages");
    }

    QDir lyricsFile(QDir::currentPath()+"/Lyrics");
    if(lyricsFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        lyricsFile.mkdir(QDir::currentPath()+"/Lyrics");
    }

    QDir songsFile(QDir::currentPath()+"/Songs");
    if(songsFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        songsFile.mkdir(QDir::currentPath()+"/Songs");
    }

    QDir searchFile(QDir::currentPath()+"/searchPictures");
    if(searchFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        searchFile.mkdir(QDir::currentPath()+"/searchPictures");
    }

    QDir userHeadsFile(QDir::currentPath()+"/userHeads");
    if(userHeadsFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        userHeadsFile.mkdir(QDir::currentPath()+"/userHeads");
    }
}

//初始化信号与槽
void MainWindow::initSignalsAndSlots()
{
    //这里是主窗口与音乐播放栏之间的信号槽
    //主窗口触发播放音乐事件
    connect(this,SIGNAL(playMusic(int)),
            musicPlayBar,SLOT(onPlayMusic(int)));
    //歌词翻译&取消翻译
    connect(musicPlayBar,SIGNAL(translateChanged()),
            this,SLOT(onTranslateChanged()));
    //显示&隐藏底部弹幕
    connect(musicPlayBar,SIGNAL(showLyricsBarrage(bool)),
            this,SLOT(onShowLyricsBarrage(bool)));

    //这是主窗口和歌曲展示页间的信号槽，播放MV
    connect(infoShow,SIGNAL(playMV()),
            this,SLOT(onPlayMV()));

    //这是歌单与音乐播放栏之间的信号槽
    //歌单触发播放音乐事件
    connect(songList,SIGNAL(playMusic(int)),
            musicPlayBar,SLOT(onPlayMusic(int)));
    //歌单修改音乐播放器中的逻辑列表(playlist)
    connect(songList , SIGNAL(changePlaylist(QUrl,int)),
            musicPlayBar,SLOT(onChangePlaylist(QUrl,int)));
    //在歌单中清空播放列表事件
    connect(songList,SIGNAL(clearMusic()),
            musicPlayBar,SLOT(onClearMusic()));
    connect(musicPlayBar,SIGNAL(updateAudioTagInMainWindow(QString)),
            songList,SLOT(onUpdateAudioTagInMainWindow(QString)));

    //这是主窗口和歌单之间的信号槽
    //传递新歌单名，让数据库创建
    connect(this, SIGNAL(sendSongListName(QString)),
            songList, SLOT(createSongListInDatabase(QString)));
    //点击按钮，新建歌单
    connect(songList->getCreateMusicListButton(), SIGNAL(clicked()),
                this , SLOT(showCreateSongListDialog()));
    //添加到播放列表
    connect(this, SIGNAL(addIntoPlayList(QString)),
            songList, SLOT(addNewSong(QString)));
    connect(this,SIGNAL(onlineAddInto(SongInfo)),
            songList,SLOT(onlineAddNewSong(SongInfo)));
    connect(songList, SIGNAL(sendSongInfo(SongInfo*)),
            this, SLOT(onRecieveSongInfo(SongInfo*)));

    //这里是音乐播放栏和歌曲信息展示窗口之间的信号槽
    //播放位置改变时歌词变化
    connect(musicPlayBar,SIGNAL(positionChanged(qint64)),
            lyricsShower,SLOT(onPositionChanged(qint64)));
    connect(musicPlayBar,SIGNAL(positionChanged(qint64)),
            lyricsBarrage,SLOT(onPositionChanged(qint64)));
    //拖动歌词时阻塞进度条
    connect(lyricsShower,SIGNAL(blockSignals(bool)),
            musicPlayBar,SLOT(onBlockSignals(bool)));
    //把阻塞那一时刻的position传回给lyricShower
    connect(musicPlayBar,SIGNAL(positionStop(qint64)),
            lyricsShower,SLOT(on_positionStop(qint64)));
    //歌词拖动后播放器进度随之更新
    connect(lyricsShower,SIGNAL(positionDraggedTo(qint64)),
            musicPlayBar,SLOT(onPositionDraggedTo(qint64)));
    //同时底部弹幕也随之更新
    connect(lyricsShower,SIGNAL(positionDraggedTo(qint64)),
            lyricsBarrage,SLOT(onPositionChanged(qint64)));
    //链接播放状态和圆盘的转动
    connect(musicPlayBar,SIGNAL(becomePausing()),
            infoShow,SLOT(diskRotateStop()));
    connect(musicPlayBar,SIGNAL(becomePlaying()),
            infoShow,SLOT(diskRotateStart()));

    //这里是标题栏和主窗口的信号槽
    //主要是对窗体属性的一些操作
    connect(titleBar,SIGNAL(showSuspensionWindow()),
            this,SLOT(onShowSuspensionWindow()));
    connect(titleBar,SIGNAL(minimizeWindow()),
            this,SLOT(onMinimizeWindow()));
    connect(titleBar,SIGNAL(maximizeWindow()),
            this,SLOT(onMaximizeWindow()));
    connect(titleBar,SIGNAL(closeWindow()),
            this,SLOT(onCloseWindow()));
    //用于开启&关闭手势识别
    connect(titleBar,SIGNAL(beginGesture()),
            this,SLOT(onBeginGesture()));
    connect(titleBar,SIGNAL(closeGesture()),
            this,SLOT(onCloseGesture()));

    //用户登录成功,向主页面和歌单分别抛出id
    connect(titleBar->loginForm,SIGNAL(loginSuccess(QString)),
            this,SLOT(onLoginSuccess(QString)));
    connect(titleBar->loginForm,SIGNAL(loginSuccess(QString)),
            songList,SLOT(onRecieveUserId(QString)));

    //这是标题栏(搜索框)和搜索器之间的信号槽
    //完成一个搜索的逻辑
    connect(titleBar,SIGNAL(beginSearchOnline(QString)),
            searcher,SLOT(searchSongsOnline(QString)));
    connect(titleBar,SIGNAL(beginSearchLocal(QString)),
            searcher,SLOT(searchLocal(QString)));
    connect(titleBar,SIGNAL(beginSearchMv(QString)),
            searcher,SLOT(searchMv(QString)));
    connect(searcher,SIGNAL(searchFinished(QMap<QString,SongInfo>)),
            this,SLOT(onSearch(QMap<QString,SongInfo>)));
    connect(searcher,SIGNAL(searchLocalFinished(QMap<QString,QString>)),
            this,SLOT(onSearchlocal(QMap<QString,QString>)));
    connect(searcher,SIGNAL(searchMvFinished(QMap<QString,QMap<QString,QString>>)),
            this,SLOT(onSearchMV(QMap<QString,QMap<QString,QString>>)));

    //这里是改变主题色的槽函数
    connect(titleBar->skinWidget,SIGNAL(colorChanged(QColor)),
            musicPlayBar,SLOT(changeThemeColor(QColor)));
    connect(titleBar->skinWidget,SIGNAL(colorChanged(QColor)),
            titleBar,SLOT(changeThemeColor(QColor)));

    //这是歌曲播放栏和歌词弹幕之间的信号槽
    //点击时模拟歌曲播放栏的按钮被按下的效果
    connect(lyricsBarrage->m_previousBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(on_previousBtn_clicked()));
    connect(lyricsBarrage->m_playBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(onRemotePlay()));
    connect(lyricsBarrage->m_nextBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(on_nextBtn_clicked()));
    connect(lyricsBarrage->m_closeBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(onCloseLyrics()));
    connect(musicPlayBar,SIGNAL(becomePlaying()),
            lyricsBarrage,SLOT(onBecomePlaying()));
    connect(musicPlayBar,SIGNAL(becomePausing()),
            lyricsBarrage,SLOT(onBecomePausing()));

    //这是歌曲展示页和歌单之前的信号槽
    //添加至歌单
    connect(infoShow, SIGNAL(sendAddIntoSongListCommand(QString,QString)),
            songList,SLOT(remoteSetSongAddInto(QString,QString)));

    //这是负责处理主窗口中央StackedWidget的页面切换
    connect(littleSongBar,SIGNAL(changePage()),
            this,SLOT(onChangePage()));
    connect(infoShow,SIGNAL(changePage()),
            this,SLOT(onChangePage()));

    //这是悬浮窗之间的一些信号槽,主要用于窗体的切换
    //和远程超控音乐播放器的状态
    connect(suspensionWindow,SIGNAL(hideWindow()),
            this,SLOT(onHideSuspensionWindow()));
    connect(suspensionWindow->previousBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(on_previousBtn_clicked()));
    connect(suspensionWindow->playBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(onRemotePlay()));
    connect(suspensionWindow->nextBtn,SIGNAL(clicked()),
            musicPlayBar,SLOT(on_nextBtn_clicked()));
    connect(musicPlayBar,SIGNAL(becomePlaying()),
            suspensionWindow,SLOT(onBecomePlaying()));
    connect(musicPlayBar,SIGNAL(becomePausing()),
            suspensionWindow,SLOT(onBecomePausing()));

    //点击播放历史播放歌曲
    connect(songListsShower,SIGNAL(songUrl(QString)),
            songList,SLOT(on_playHistory_clicked(QString)));

}

//更新拖动区域
void MainWindow::updateDragRegion()
{
    QPoint rightBottomPoint(x()+width(),y()+height());
    dragRegion = QRect(rightBottomPoint.x()-25,rightBottomPoint.y()-25,25,25);
}

//高斯模糊并将其结果跟新至背景图
void MainWindow::GaussianBlurUpdate()
{
    //将当前封面另存为同一目录下的tempCover文件
    qDebug()<<currentSongInfo->album_cover;
    QFile coverFile(currentSongInfo->album_cover);
    if(!coverFile.open(QIODevice::ReadOnly))
    {
        qDebug()<<"封面文件读取失败!";
        return;
    }
    QString tempCoverPath = QDir::currentPath() + "/CoverImages/tempCover.png";
    QFile::remove(tempCoverPath);
    coverFile.copy(tempCoverPath);
    coverFile.close();
    //高斯模糊
    Mat srcImage = imread(tempCoverPath.toStdString());
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    Mat dstImage;
    int g_nGaussianBlurValue = 20;
    GaussianBlur(srcImage, dstImage, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);
    //自动调节亮度与对比度
    int height = dstImage.rows;
    int width = dstImage.cols;
    int channels = dstImage.channels();
    Mat finalImage = Mat::zeros(dstImage.size(),dstImage.type());
    float alpha = 0.8; //控制对比度的参数，小于1则降低
    float beta = -10; //控制亮度的参数，小于0则降低
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < width; col++)
        {
            if (channels == 3)
            {
                //获取RGB各通道像素值
                float b = dstImage.at<Vec3b>(row, col)[0];
                float g = dstImage.at<Vec3b>(row, col)[1];
                float r = dstImage.at<Vec3b>(row, col)[2];
                //对各通道像素值进行计算，实现对比度和亮度变换
                finalImage.at<Vec3b>(row, col)[0] = saturate_cast<uchar>(b*alpha + beta);//saturate_cast<uchar>()——确保新的像素值在0-255之间
                finalImage.at<Vec3b>(row, col)[1] = saturate_cast<uchar>(g*alpha + beta);
                finalImage.at<Vec3b>(row, col)[2] = saturate_cast<uchar>(r*alpha + beta);
            }
            else if (channels == 1)
            {
                //获取灰度图像像素值
                float v = dstImage.at<uchar>(row, col);
                finalImage.at<uchar>(row, col) = saturate_cast<uchar>(v*alpha + beta);
            }
        }
    }
    //储存openCV处理完毕后的图片到指定路径
    imwrite(tempCoverPath.toStdString(), finalImage);
    //将高斯模糊后的图片作为歌曲展示页的背景
    QPalette backPalette;
    QPixmap backImage = QPixmap(tempCoverPath).scaledToWidth(songInfoPage->width(),Qt::SmoothTransformation);
    backPalette.setBrush(backgroundRole(),backImage);
    songInfoPage->setPalette(backPalette);
}

//鼠标按下事件的三个过程
void MainWindow::mousePressEvent(QMouseEvent *event)
{
    //如果点在标题栏范围内->移动事件
    if(titleBar->rect().contains(event->pos()))
    {
        clickedType = Move;
        mouseStartPoint = event->globalPos(); //记录鼠标的初始位置
        windowsStartPoint = this->pos(); //记录窗口的初始位置
    }
    else if (dragRegion.contains(event->globalPos()))
    {
        clickedType = Resize;
    }
}
void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if(clickedType == No)
    {
        return;
    }
    else if(clickedType == Move)
    {
        QPoint offset = event->globalPos() - mouseStartPoint ;
        QPoint p = windowsStartPoint + offset;
        this->move(p);
    }
    else
    {
        QPoint newRightButtom = event->globalPos();
        int newWidth = newRightButtom.x() - this->x();
        int newHeight = newRightButtom.y() - this->y();
        this->setGeometry(this->x(),this->y(),newWidth,newHeight);

        QPalette backPalette;
        QPixmap Image(QDir::currentPath() + "/CoverImages/tempCover.png");
        QPixmap backImage = Image.scaledToWidth(songInfoPage->width(),Qt::FastTransformation);
        backPalette.setBrush(backgroundRole(),backImage);
        songInfoPage->setPalette(backPalette);
    }

   QString tempCoverPath = QDir::currentPath() + "/CoverImages/tempCover.png";
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);

    if(clickedType == No)
    {
        return;
    }
    else
    {
        clickedType = No;
        updateDragRegion();

        QPalette backPalette;
        QPixmap Image(QDir::currentPath() + "/CoverImages/tempCover.png");
        QPixmap backImage = Image.scaledToWidth(songInfoPage->width(),Qt::SmoothTransformation);
        backPalette.setBrush(backgroundRole(),backImage);
        songInfoPage->setPalette(backPalette);
    }
}

//覆写dragEnterEvent,允许歌曲被拖入
void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    //不允许未登录用户拖入
    if(userId == "")
    {
        ErrorWindow *notLoginError = new ErrorWindow("请先注册或登录");
        notLoginError->show();
        notLoginError->showInstantly();
        return;
    }

    //定义一个QStringList记录可以接受的类型
    QStringList acceptedFileTypes;
    acceptedFileTypes.append("mp3");
    acceptedFileTypes.append("wav");
    acceptedFileTypes.append("wma");
    //mineDate()为拖入文件的内容
    if(event->mimeData()->hasUrls())
    {  //遍历拖入的文件进行类型判断，如果是音乐文件则接受
        int totalFileNumber = event->mimeData()->urls().count();
        bool formatValid = true; //拖入文件是否格式都正确
        for(int count = 0 ; count<totalFileNumber ; ++count)
        {
             QFileInfo file(event->mimeData()->urls().at(count).toLocalFile());
             if(acceptedFileTypes.contains(file.suffix().toLower())) {}             //是否包含后缀名
             else
             {
                 formatValid = false;
             }
        }

        if(formatValid)
        {
            event->acceptProposedAction();
        }
        else
        {
            ErrorWindow *typeError = new ErrorWindow("拖入的文件格式错误");
            typeError->show();
            typeError->showInstantly();
        }
    }
}
//拖入后进行加载处理
void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> songUrls = event->mimeData()->urls(); //拖入文件的Url
    //遍历QList,将这些歌曲都添加进播放列表
    //发射信号，清理player里面之前的歌
    emit clearMusic();
    foreach(QUrl url,songUrls)
    {
        //emit changePlaylist(url,1);
        QFileInfo fileInfo = url.toLocalFile();
        //emit changeListSongs(fileInfo.fileName(),1);
        emit addIntoPlayList(fileInfo.filePath());
    }

    //播放最新拖入的歌
    emit playMusic(0);
}

//对窗口进行小窗化处理
void MainWindow::onShowSuspensionWindow()
{
    this->hide();
    suspensionWindow->show();
}
//对窗口进行最小化处理
void MainWindow::onMinimizeWindow()
{
    this->showMinimized();
}
//对窗口进行最大化&还原处理
void MainWindow::onMaximizeWindow()
{
    if(this->isMaximized())
    {
        this->showNormal();
    }
    else
    {
        this->showMaximized();
    }
}
//关闭窗口
void MainWindow::onCloseWindow()
{
    titleBar->skinWidget->close();
    this->close();
}

//用户登录成功
void MainWindow::onLoginSuccess(QString userId)
{
    this->userId = userId;
    songListsShower->onUserLogin(userId);
    searcher->setUserid(userId);
    infoShow->setUser(userId);
}

void MainWindow::onSearch(QMap<QString, SongInfo> results)
{
    ResultWidget *searchResult=new ResultWidget;
    searchResult->on_searchReply(results);
    connect(searchResult,SIGNAL(resendSongInfo(SongInfo &)),
            this,SLOT(onResendSongInfo(SongInfo &)));
    searchResult->show();
}
void MainWindow::onSearchlocal(QMap<QString, QString> localresults)
{
    SearchLocal *searchLocalResult=new SearchLocal(localresults);
    connect(searchLocalResult,SIGNAL(resendSongUrl(QString)),
            this,SLOT(onResendSongUrl(QString)));
    searchLocalResult->show();
}
void MainWindow::onSearchMV(QMap<QString, QMap<QString, QString> > mvResults)
{
    ResultWidget *searchResult=new ResultWidget;
    searchResult->on_searchReply1(mvResults);
    connect(searchResult,SIGNAL(resendnvId(QString)),
            this,SLOT(onResendMvId(QString)));
    searchResult->show();
}

//网络歌曲的播放
void MainWindow::onResendSongInfo(SongInfo &chosenSong)
{
    emit clearMusic();
    emit onlineAddInto(chosenSong);
    emit playMusic(0);
}
//本地歌曲搜索的播放
void MainWindow::onResendSongUrl(QString songUrl)
{
    qDebug()<<"reach here";
    emit clearMusic();
    emit addIntoPlayList(songUrl);
    emit playMusic(0);
}
//mv搜索的播放
void MainWindow::onResendMvId(QString mvId)
{
        MvPlayer *mvplayer = new MvPlayer;
        connect(mvplayer,SIGNAL(MvWidgetClose()),
                this,SLOT(onMvWidgetClose()));
        emit playMusic(-1);
        this->hide();
        mvplayer->getMvUrls(mvId);
        mvplayer->setModal(true);
        mvplayer->show();
}
//mv关闭后重新显示主界面
void MainWindow::onMvWidgetClose()
{
    this->show();
}

//开始手势识别
void MainWindow::onBeginGesture()
{
    gestureControl = new GestureControl;
    connect(gestureControl,SIGNAL(gestureLeft()),
            musicPlayBar,SLOT(on_previousBtn_clicked()));
    connect(gestureControl,SIGNAL(gestureRight()),
            musicPlayBar,SLOT(on_nextBtn_clicked()));
    gestureControl->mainProcess();
}
//关闭手势识别
void MainWindow::onCloseGesture()
{
    gestureControl->setKeyboard('q');
}

//停止小窗化,显示大窗
void MainWindow::onHideSuspensionWindow()
{
    suspensionWindow->hide();
    this->show();
}

//弹出歌单的对话框
void MainWindow::showCreateSongListDialog()
{
    //不允许未登录用户创建歌单
    if(userId == "")
    {
        ErrorWindow *notLoginError = new ErrorWindow("请先注册或登录");
        notLoginError->show();
        notLoginError->showInstantly();
        return;
    }

    CreateSongListDialog dlg;
    int status = dlg.exec();
    if(status == 0)
    {
        emit(sendSongListName(dlg.getLineEdit()->text()));
    }
    else
    {

    }
}

//歌词翻译状态改变，重写爬取歌词
void MainWindow::onTranslateChanged()
{
    //改变翻译状态
    translate = !translate;
    //qDebug()<<translate;
    //重新爬取歌词
    lyricsDownloader->DownloadLyric(currentSongInfo->song_id,translate);
    lyricsShower->analyzeLrcContent(currentSongInfo->song_id);

    //同步更新至底部弹幕
    lyricsBarrage->setLineMap(lyricsShower->getLineMap());
    lyricsBarrage->setWord_list(lyricsShower->getWord_list());
    lyricsBarrage->setInterval_list(lyricsShower->getInterval_list());
}
//切歌时接受歌单爬取的歌曲信息并更新至主页面
void MainWindow::onRecieveSongInfo(SongInfo *info)
{
    currentSongInfo = new SongInfo(*info);
    //qDebug()<<currentSongInfo->song_id;

    //更新临时的背景图片
    GaussianBlurUpdate();

    //开始爬取歌词
    lyricsDownloader->DownloadLyric(currentSongInfo->song_id,translate);
    lyricsShower->analyzeLrcContent(currentSongInfo->song_id);

    //将主歌词的结果给底部弹幕赋值,防止重复爬取,提高程序效率
    lyricsBarrage->setLineMap(lyricsShower->getLineMap());
    lyricsBarrage->setWord_list(lyricsShower->getWord_list());
    lyricsBarrage->setInterval_list(lyricsShower->getInterval_list());

    //更新其他展示页面的歌曲信息
    littleSongBar->changeSong(*currentSongInfo);
    infoShow->changeSong(*currentSongInfo);
    suspensionWindow->changeSong(*currentSongInfo);
}

//显示&隐藏底部歌词弹幕的槽函数
void MainWindow::onShowLyricsBarrage(bool show)
{
    //qDebug()<<"reach change lyrics";
    if(show)
    {
        lyricsBarrage->show();
    }
    else
    {
        lyricsBarrage->hide();
    }
}

//切换视图中央的界面
void MainWindow::onChangePage()
{
    mainPageContainer->nextPage();
}

//播放MV
void MainWindow::onPlayMV()
{
    MvPlayer *mvplayer = new MvPlayer;
    connect(mvplayer,SIGNAL(MvWidgetClose()),
            this,SLOT(onMvWidgetClose()));
    emit playMusic(-1);
    this->hide();
    mvplayer->getMvUrls(currentSongInfo->mv_id);
    mvplayer->setModal(true);
    mvplayer->show();
}
