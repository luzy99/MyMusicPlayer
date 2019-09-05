#include "mainwindow.h"
#include "songlist.h"
#include "renamesonglistdialog.h"
#include <opencv2/opencv.hpp>
#include <QDir>
#include <QVBoxLayout>
#include <QApplication>
#include <QBitmap>
#include <QPainter>
#include <QMimeData>
#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include<QDebug>
using namespace cv;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      clickedType(No)
{
    //初始化主窗体属性
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("Rainbow Music Player");
    this->setWindowIcon(QIcon(":/icon/res/suspensionWindow.png"));
    this->setMinimumSize(1200,800);
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
    mainPageContainer = new QStackedWidget;
    mainPageContainer->setObjectName("mainPageContainer");

    //初始化展示歌单信息的界面
    songListPage = new QWidget;
    mainPageContainer->insertWidget(0,songListPage);
    QHBoxLayout *songListPageLayout = new QHBoxLayout;
    songListPageLayout->setSpacing(0);
    songListPageLayout->setContentsMargins(0,0,0,0);
    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->setSpacing(0);
    leftLayout->setContentsMargins(0,0,0,0);
    //初始化歌单界面
    songList = new SongList;
    leftLayout->addWidget(songList);
    //初始化小的歌曲显示栏
    littleSongBar = new LittleSongBar;
    leftLayout->addWidget(littleSongBar);
    songListPageLayout->addLayout(leftLayout,2);
    songListPage->setLayout(songListPageLayout);
    //初始化动画滚轮
    QVBoxLayout *adsLayout = new QVBoxLayout;
    adsLayout->setSpacing(10);
    adsLayout->setContentsMargins(10,10,10,10);
    adsWall = new AnimatedWallWG;
    adsWall->setWindowFlags(Qt::FramelessWindowHint);
    adsWall->setAttribute(Qt::WA_TranslucentBackground);
    adsLayout->addWidget(adsWall,Qt::AlignTop);
    songListPageLayout->addLayout(adsLayout,7);

    //初始化展示歌曲信息的页面
    songInfoPage = new QWidget;
    songInfoPage->setAutoFillBackground(true);
    mainPageContainer->insertWidget(1,songInfoPage);
    QHBoxLayout *songInfoPageLayout = new QHBoxLayout;
    //初始化歌曲信息显示
    infoShow = new SongInfoShow(*currentSongInfo,this);
    infoShow->setObjectName("infoShow");
    songInfoPageLayout->addWidget(infoShow);
    //初始化歌词界面
    lyricsShower = new LyricWidget;
    lyricsShower->setObjectName("lyricsShower");
    lyricsShower->setWindowFlags(Qt::FramelessWindowHint);
    lyricsShower->setAttribute(Qt::WA_TranslucentBackground);
    songInfoPageLayout->addWidget(lyricsShower);
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
        coverFile.mkdir(QDir::currentPath()+"/Lyrics");
    }

    QDir songsFile(QDir::currentPath()+"/Songs");
    if(songsFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        coverFile.mkdir(QDir::currentPath()+"/Songs");
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
            songList,SLOT(setSongAddInto(QString,QString)));

    //这是负责处理主窗口中央StackedWidget的页面切换
    connect(littleSongBar,SIGNAL(changePage(int)),
            this,SLOT(onChangePage(int)));
    connect(infoShow,SIGNAL(changePage(int)),
            this,SLOT(onChangePage(int)));
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
            QMessageBox::warning(this,"警告","拖入的文件存在格式错误.",QMessageBox::Yes);
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
    suspensionWindow = new SuspensionWindow;
    //小窗在关闭时delete
    suspensionWindow->setAttribute(Qt::WA_DeleteOnClose);
    //这是主窗口和悬浮窗之间的信号槽,主要用于窗体的切换
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
    this->close();
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

    littleSongBar->changeSong(*currentSongInfo);
    infoShow->changeSong(*currentSongInfo);
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
void MainWindow::onChangePage(int index)
{
    mainPageContainer->setCurrentIndex(index);
}
