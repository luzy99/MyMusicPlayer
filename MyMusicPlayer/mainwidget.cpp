#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "titlebar.h" //包含“自定义标题栏”头文件
#include <QMimeData>
#include <QFileInfo>
#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>

#include<QDebug>
//调用WIN API需要用到的头文件与库 [实现缩放]
#ifdef Q_OS_WIN
#include <qt_windows.h>
#include <Windowsx.h>
#endif
/*
 * 205 10 10 深红
 * 240 12 16 选中红色
 * 255 255 255 白色
 * 219 208 208 深灰
 * 244 239 239 浅灰
 */


/*测试结果
 * 1.当在随机播放模式时，下一首按钮不能有随机效果
 * 2.拖入新歌曲的时候，会自动添加到播放列表的最后，并从列表第一首播放
*/
MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWidget)
{
    ui->setupUi(this);

    //Qt::FramelessWindowHint设置窗口标志为无边框，而Qt::WindowStaysOnTopHint使窗口位于所有界面之上
    this->setWindowFlags(Qt::FramelessWindowHint);
    //背景透明
    //setAttribute(Qt::WA_TranslucentBackground, true);

    //设置窗口允许拖拽东西上来(默认不可以)
    this->setAcceptDrops(true);

    //使用调色板设置窗口的背景色
    QPalette pal_windows(palette());
    pal_windows.setColor(QPalette::Background, QColor(255, 255, 255));
    setAutoFillBackground(false);    
    resize(1200,900);
    //设置窗口名称，会发生窗口标题栏改变事件，随之自定义标题栏的标题会更新
    setWindowTitle("My Music Player"); 
    //设置窗口图标，会发生窗口图标改变事件，随之自定义标题栏的图标会更新    
    setWindowIcon(QIcon(":/icon/res/icon.png")); 
    setPalette(pal_windows);
    setMinimumSize(400 , 300);
    //定义自定义标题栏对象
    pTitleBar = new titleBar(this);
    installEventFilter(pTitleBar);
    
    
    //窗口布局中加标题栏
    QVBoxLayout *pLayout1 = new QVBoxLayout();
    pLayout1->addWidget(pTitleBar);
    pLayout1->addStretch();
    pLayout1->setSpacing(0);
    pLayout1->setContentsMargins(0, 0, 0, 0);//设置边距
    this->setLayout(pLayout1);

    //歌单布局
    pSongList = new SongList (this);
    installEventFilter(pSongList);
    pSongList->setGeometry(0,30,240,this->height()-30);

    //初始化歌词下载
    //默认不进行翻译
    lyricsDownloader = new LyricDownload;
    translate = false;

    //初始化展示歌词的组件
    lyricsShower = new LyricWidget;
    lyricsShower->resize(200,400);
    lyricsShower->show();

    //初始化底部歌词弹幕
    lyricsBarrage = new miniLyrics;
        
    //m_nBorder表示鼠标位于边框缩放范围的宽度，可以设置为5
    m_nBorderWidth=5;

    //初始化当前歌曲的信息
    //默认第一次打开时还没有播放歌曲
    currentSongInfo = new SongInfo;
    currentSongInfo->title = "暂无歌曲在播放";
    currentSongInfo->artist = "暂无相关信息";
    currentSongInfo->album_cover = ":/icon/res/default_cover.png";

    //初始化显示
    infoShow = new SongInfoShow(this,*currentSongInfo);

    infoShow->setGeometry(250,30,300,this->height()-60);

    //初始化自定义音乐播放栏
    pMusicPlayBar = new MusicPlayBar(this);
    pMusicPlayBar->setGeometry(240, 840, 900, 60);

    //判断当前路径下有无下载封面和歌词需要的文件
    //若没有则重新创建
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
    if(coverFile.exists())
    {
        //如果已经存在则什么也不做
    }
    else
    {
        //如果不存在，则重新创建文件
        coverFile.mkdir(QDir::currentPath()+"/Lyrics");
    }

    //关联信号与槽
    initSignalsAndSlots();
}

MainWidget::~MainWidget()
{
    delete ui;
}

//nativeEvent主要用于进程间通信-消息传递，使用这种方式后来实现窗体的缩放 [加上了这函数，窗口也能移动了]
bool MainWidget::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)

    MSG *param = static_cast<MSG *>(message);

    switch (param->message)
    {
    case WM_NCHITTEST:
    {
        int nX = GET_X_LPARAM(param->lParam) - this->geometry().x();
        int nY = GET_Y_LPARAM(param->lParam) - this->geometry().y();

        // 如果鼠标位于子控件上，则不进行处理
        if (childAt(nX, nY) != nullptr)
            return QWidget::nativeEvent(eventType, message, result);

        *result = HTCAPTION;

        // 鼠标区域位于窗体边框，进行缩放
        if ((nX > 0) && (nX < m_nBorderWidth))
            *result = HTLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width()))
            *result = HTRIGHT;

        if ((nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOP;

        if ((nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOM;

        if ((nX > 0) && (nX < m_nBorderWidth) && (nY > 0)
                && (nY < m_nBorderWidth))
            *result = HTTOPLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > 0) && (nY < m_nBorderWidth))
            *result = HTTOPRIGHT;

        if ((nX > 0) && (nX < m_nBorderWidth)
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMLEFT;

        if ((nX > this->width() - m_nBorderWidth) && (nX < this->width())
                && (nY > this->height() - m_nBorderWidth) && (nY < this->height()))
            *result = HTBOTTOMRIGHT;

        return true;
    }
    }
    emit windowChange();
    return QWidget::nativeEvent(eventType, message, result);
}

void MainWidget::initSignalsAndSlots()
{
    //重绘歌单大小
    connect(this , SIGNAL(windowChange()),
            pSongList,SLOT(resetGeometry()));
    //主窗口修改音乐播放器中的逻辑列表(playlist)
    connect(this , SIGNAL(changePlaylist(QUrl,int)),
            pMusicPlayBar,SLOT(onChangePlaylist(QUrl,int)));
    //修改显示给用户的歌单
    connect(this,SIGNAL(changeListSongs(QString,int)),
            pSongList,SLOT(onChangeListSongs(QString,int)));
    //歌单触发播放音乐事件
    connect(pSongList,SIGNAL(playMusic(int)),
            pMusicPlayBar,SLOT(onPlayMusic(int)));
    //主窗口触发播放音乐事件
    connect(this,SIGNAL(playMusic(int)),
            pMusicPlayBar,SLOT(onPlayMusic(int)));
    //歌单修改音乐播放器中的逻辑列表(playlist)
    connect(pSongList , SIGNAL(changePlaylist(QUrl,int)),
            pMusicPlayBar,SLOT(onChangePlaylist(QUrl,int)));
    //播放位置改变时歌词变化
    connect(pMusicPlayBar,SIGNAL(positionChanged(qint64)),
            lyricsShower,SLOT(onPositionChanged(qint64)));
    connect(pMusicPlayBar,SIGNAL(positionChanged(qint64)),
            lyricsBarrage,SLOT(onPositionChanged(qint64)));
    //拖动歌词时阻塞进度条
    connect(lyricsShower,SIGNAL(blockSignals(bool)),
            pMusicPlayBar,SLOT(onBlockSignals(bool)));
    //把阻塞那一时刻的position传回给lyricShower
    connect(pMusicPlayBar,SIGNAL(positionStop(qint64)),
            lyricsShower,SLOT(on_positionStop(qint64)));
    //歌词拖动后播放器进度随之更新
    connect(lyricsShower,SIGNAL(positionDraggedTo(qint64)),
            pMusicPlayBar,SLOT(onPositionDraggedTo(qint64)));
    //同时底部弹幕也随之更新
    connect(lyricsShower,SIGNAL(positionDraggedTo(qint64)),
            lyricsBarrage,SLOT(onPositionChanged(qint64)));
    //切歌时进行歌曲信息的变化
    //同时进行歌词下载
    connect(pMusicPlayBar,SIGNAL(updateAudioTag(QString)),
            this,SLOT(onUpdateAudioTag(QString)));
    //显示&隐藏底部弹幕
    connect(pMusicPlayBar,SIGNAL(showLyricsBarrage(bool)),
            this,SLOT(onShowLyricsBarrage(bool)));
    //歌词翻译&取消翻译
    connect(pMusicPlayBar,SIGNAL(translateChanged()),
            this,SLOT(onTranslateChanged()));
    //链接播放状态和圆盘的转动
    connect(pMusicPlayBar,SIGNAL(becomePausing()),
            infoShow,SLOT(Stop()));
    connect(pMusicPlayBar,SIGNAL(becomePlaying()),
            infoShow,SLOT(Start()));
    //重绘主窗口大小
    connect(this,SIGNAL(windowChange()),this,SLOT(resetGeometry()));
}

//覆写dragEnterEvent,允许歌曲被拖入
void MainWidget::dragEnterEvent(QDragEnterEvent *event)
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
void MainWidget::dropEvent(QDropEvent *event)
{
    QList<QUrl> songUrls = event->mimeData()->urls(); //拖入文件的Url
    //遍历QList,将这些歌曲都添加进播放列表
    foreach(QUrl url,songUrls)
    {
        emit changePlaylist(url,1);
        QFileInfo fileInfo = url.toLocalFile();
        emit changeListSongs(fileInfo.fileName(),1);
    }

    //**********************************************************************************
    //播放最新拖入的歌
    emit playMusic(-1);
}

void MainWidget::resetGeometry()
{
    QWidget *pWindow = this->window();
    QSize pWindowSize = pWindow->size();
    int sizeY = pWindowSize.height();
    int sizeX = pWindowSize.width();
    this->resize(sizeX , sizeY);
    this->pMusicPlayBar->setGeometry(240, sizeY-60, sizeX-300, 60);
}

//歌词翻译状态改变，重写爬取歌词
void MainWidget::onTranslateChanged()
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

//切歌时开始爬取各类信息
void MainWidget::onUpdateAudioTag(QString currentFileName)
{
    currentSongInfo->has_cover=false;
    AudioTag tag(currentFileName,currentSongInfo);
    tag.getAllinfo();
    tag.idMatch();
    tag.downloadPic();
    lyricsDownloader->DownloadLyric(currentSongInfo->song_id,translate);
    lyricsShower->analyzeLrcContent(currentSongInfo->song_id);

    //将主歌词的结果给底部弹幕赋值,防止重复爬取,提高程序效率
    lyricsBarrage->setLineMap(lyricsShower->getLineMap());
    lyricsBarrage->setWord_list(lyricsShower->getWord_list());
    lyricsBarrage->setInterval_list(lyricsShower->getInterval_list());

    infoShow->changeSong(*currentSongInfo);
}

//显示&隐藏底部歌词弹幕的槽函数
void MainWidget::onShowLyricsBarrage(bool show)
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
