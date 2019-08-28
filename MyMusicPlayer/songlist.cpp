#include "songlist.h"
#include "titlebar.h"
#include<QDebug>
#include <QDir>
#include <QFileDialog>

SongList::SongList(QWidget *parent) : QWidget(parent)
{
    //数据成员初始化，各个窗口父子关系确认
    g_container = new QGroupBox (this);
    scrollSongs = new QScrollArea (g_container);
    scrollLists = new QScrollArea (g_container);
    scrollListsWidget = new QWidget(scrollLists);
    scrollSongsWidget = new QWidget (scrollSongs);
    listList = new QListWidget(scrollListsWidget);
    listSongs = new QListWidget(scrollSongsWidget);
    labelLists = new QLabel(scrollListsWidget);
    labelSongs = new QLabel(scrollSongsWidget);
    lovedMusiclist = new QListWidgetItem(listList);
    createMusicListButton = new QPushButton(scrollListsWidget);
    addSongBtn = new QPushButton(scrollSongsWidget);

    //滚轮属性
    scrollLists->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollSongs->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    //各个窗口位置确认
    g_container->setGeometry(0, 0, 240 , 870);
    scrollLists->setGeometry(0, 0, 240, 290);
    scrollSongs->setGeometry(0, 290, 240, 580);

    scrollListsWidget->setGeometry(0, 0, 230, 290);
    scrollListsWidget->setMinimumSize(230, 290);

    scrollSongsWidget->setGeometry(0, 0, 230 , 580);
    scrollSongsWidget->setMinimumSize(230, 580);

    labelLists->setGeometry(0, 0 , 230 ,30);
    labelSongs->setGeometry(0, 0, 230, 30);

    listList->setGeometry(0, 30, 230, 260);
    listSongs->setGeometry(0, 30, 230, 550);

    createMusicListButton->setGeometry(200, 0, 30, 30);
    addSongBtn->setGeometry(200, 0, 30, 30);
    //设置两个滚轮区无边框
    scrollLists->setFrameShape(QFrame::NoFrame);
    scrollSongs->setFrameShape(QFrame::NoFrame);

    //消除滚轮区内部的窗口边框
    listList->setFrameShape(QFrame::NoFrame);
    listSongs->setFrameShape(QFrame::NoFrame);

    //标签文字
    labelLists->setText(" 我的歌单");
    labelSongs->setText(" 播放列表");
    labelLists->setStyleSheet("color:rgb(139, 139, 139);");
    labelSongs->setStyleSheet("color:rgb(139, 139, 139);");

    //列表项的定义，样式
    lovedMusiclist->setIcon(QIcon(":/icon/res/favourite_d.ico"));
    lovedMusiclist->setText("我喜欢的音乐");
    QFont font = QFont("宋体",10,QFont::Bold);
    lovedMusiclist->setFont(font);

    //新建歌单按钮样式
    createMusicListButton->setFlat(true);
    createMusicListButton->setIcon(QIcon(":/icon/res/add.png"));
//    createMusicListButton->setStyleSheet("QPushButton {"
//                                             "border: 0px solid #8f8f91;"
//                                             "border-radius:15px;"
//                                             "background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,"
//                                             "stop: 0 #f6f7fa, stop: 1 #dadbde);"
//                                             "min-width: 10px;"
//                                         "}");

    //添加歌曲按钮
    addSongBtn->setFlat(true);
    addSongBtn->setIcon(QIcon(":/icon/res/add.png"));

    //设置滚轮样式
    scrollLists->setWidget(scrollListsWidget);
    scrollSongs->setWidget(scrollSongsWidget);

    QString scrollStyleSheet = "QScrollBar:vertical"
                               "{   width:8px;  "
                               "   background:rgba(0,0,0,0%);"
                               "    margin:0px,0px,0px,0px;"
                                "   padding-top:9px;"
                                "   padding-bottom:9px;"
                              " }"
                               "QScrollBar::handle:vertical"
                               "{"
                                   "width:8px;"
                                  " background:rgba(0,0,0,25%);"
                                  " border-radius:4px;"
                                   "min-height:20;"
                               "}"
                               "QScrollBar::handle:vertical:hover"
                               "{"
                                  " width:8px;"
                                  " background:rgba(0,0,0,50%);"
                                  " border-radius:4px;"
                                  " min-height:20;"
                               "}"
                               "QScrollBar::add-line:vertical"
                               "{"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/3.png);"
                                   "subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:vertical"
                               "{"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/1.png);"
                                   "subcontrol-position:top;"
                               "}"
                               "QScrollBar::add-line:vertical:hover"
                               "{"
                                  " height:9px;width:8px;"
                                   "border-image:url(:/images/a/4.png);"
                                   "subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:vertical:hover"
                               "{"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/2.png);"
                                   "subcontrol-position:top;"
                               "}"
                               "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical"
                               "{"
                                   "background:rgba(0,0,0,10%);"
                                   "border-radius:4px;"
                               "}"

                               "QScrollBar:horizontal"
                               "{"
                                   "width:8px;"
                                   "background:rgba(0,0,0,0%);"
                                   "margin:0px,0px,0px,0px;"
                                   "padding-top:9px;"
                                   "padding-bottom:9px;"
                               "}"
                               "QScrollBar::handle:horizontal"
                               "{"
                                  " width:8px;"
                                  " background:rgba(0,0,0,25%);"
                                  " border-radius:4px;"
                                   "min-height:20;"
                               "}"
                               "QScrollBar::handle:horizontal:hover"
                               "{"
                                   "width:8px;"
                                  " background:rgba(0,0,0,50%);"
                                   "border-radius:4px;"
                                  " min-height:20;"
                               "}"
                               "QScrollBar::add-line:horizontal"
                               "{"
                                  " height:9px;width:8px;"
                                  " border-image:url(:/images/a/3.png);"
                                  " subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:horizontal"
                               "{"
                                  " height:9px;width:8px;"
                                   "border-image:url(:/images/a/1.png);"
                                   "subcontrol-position:top;"
                               "}"
                               "QScrollBar::add-line:horizontal:hover"
                              " {"
                                   "height:9px;width:8px;"
                                   "border-image:url(:/images/a/4.png);"
                                   "subcontrol-position:bottom;"
                               "}"
                               "QScrollBar::sub-line:horizontal:hover"
                               "{"
                                  " height:9px;width:8px;"
                                   "border-image:url(:/images/a/2.png);"
                                   "subcontrol-position:left;"
                              " }"
                               "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal "
                               "{"
                                   "background:rgba(0,0,0,10%);"
                                   "border-radius:4px;"
                               "}";

    scrollLists->setStyleSheet(scrollStyleSheet);

    scrollSongs->setStyleSheet(scrollStyleSheet);

    //让部件显示
    g_container->show();
    scrollLists->show();
    scrollSongs->show();
    scrollListsWidget->show();
    scrollSongsWidget->show();
    listList->show();
    listSongs->show();

    //链接信号与槽
    initSignalsAndSlots();
}

void SongList::initSignalsAndSlots()
{
    //双击时发出播放信号
    connect(listSongs,SIGNAL(doubleClicked(QModelIndex)),
            this,SLOT(on_ListSongs_doubleClicked(QModelIndex)));
    //双击添加歌曲时添加文件
    connect(addSongBtn,SIGNAL(clicked()),
            this,SLOT(on_addSongBtn_clicked()));
}

//当窗口发生改变的时候，歌单部分按照比例缩放
void SongList::resetGeometry()
{
    QWidget *pWindow = this->window();
    QSize pWindowSize = pWindow->size();
    int sizeY = pWindowSize.height();
    int sizeX = pWindowSize.width();
    this->resize(sizeX , sizeY);
    g_container->setGeometry(0, 0, 240 , sizeY-30);
    scrollLists->setGeometry(0, 0, 240,  ((sizeY-30 ))/3);
    scrollSongs->setGeometry(0, ((sizeY-30 ))/3, 240, (2*(sizeY-30))/3);
    scrollListsWidget->setGeometry(0, 0, 230, 290);
    scrollListsWidget->setMinimumSize(230, 290);

    scrollSongsWidget->setGeometry(0, 0, 230 , 580);
    scrollSongsWidget->setMinimumSize(230, 580);
}

void SongList::on_ListSongs_doubleClicked(const QModelIndex &index)
{
    int songIndex = index.row();
    emit playMusic(songIndex);
}

void SongList::on_addSongBtn_clicked()
{
    //添加文件
    QString curPath = QDir::homePath(); //获取用户目录
    //限制打开文件的类型
    QString dlgTitle = "选择音频文件(mp3/wav/wma)";
    QString filter = "音频文件(*.mp3 *.wav *.wma);;mp3文件(*.mp3);;wav文件(*.wav);;wma文件(*.wma);;所有文件(*.*)";
    QStringList fileList = QFileDialog::getOpenFileNames(this,dlgTitle,curPath,filter);

    if (fileList.count()<1)
        return;
    for(int i=0;i<fileList.count();i++)
    {
        QString file = fileList.at(i);
        QFileInfo fileInfo(file);
        listSongs->addItem(fileInfo.fileName()); //添加到界面文件夹
        QUrl songUrl = QUrl::fromLocalFile(file);
        emit changePlaylist(songUrl,1);
    }
}

void SongList::onChangeListSongs(QString fileName, int behaviorIndex)
{
    if(behaviorIndex == 1)
    {
        listSongs->addItem(fileName);
    }
}

