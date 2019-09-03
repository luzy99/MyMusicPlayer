#include "songlist.h"
#include "titlebar.h"
#include "createsonglistdialog.h"
#include "renamesonglistdialog.h"
#include "audiotag.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QDebug>
#include <QColor>
#include <QDir>
#include <QFileDialog>


SongList::SongList(QWidget *parent) : QWidget(parent)
{
    //建立联系，数据库的打开
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("127.0.0.1");
    db.setDatabaseName("Rainbow");
    db.setUserName("root");
    db.setPassword("20000908q");
    if(!db.open())
    {
        qDebug()<<"数据库打开失败";
    }

    //数据成员初始化，各个窗口父子关系确认
    g_container = new QGroupBox (this);
    scrollSongs = new QScrollArea (g_container);
    scrollLists = new QScrollArea (g_container);
    scrollListsWidget = new QWidget(scrollLists);
    scrollSongsWidget = new QWidget (scrollSongs);
    listList = new MyListListWidget(scrollListsWidget);
    listSongs = new MyListSongWidget(scrollSongsWidget);
    labelLists = new QLabel(scrollListsWidget);
    labelSongs = new QLabel(scrollSongsWidget);

    addSongBtn = new QPushButton(scrollSongsWidget);
    createMusicListButton = new QPushButton(scrollListsWidget);


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

    //滚轮属性
    scrollLists->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollSongs->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);


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



    //初始化歌单
    initSonglist();
    showSongsOfList("我喜欢的音乐");


    listList->setMouseTracking(true); //此处极为关键，若不设置的话，itemEnter()函数将异常

    listSongs->setMouseTracking(true);

    tempInfo = new SongInfo;
    tempInfo->title = "暂无歌曲在播放";
    tempInfo->artist = "暂无相关信息";

    //让部件显示
    g_container->show();
    scrollLists->show();
    scrollSongs->show();
    scrollListsWidget->show();
    scrollSongsWidget->show();
    listList->show();
    listSongs->show();

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

    //播放列表项鼠标事件
    connect(listSongs, SIGNAL(itemEntered(QListWidgetItem *)),this , SLOT(onListSongItemEntered(QListWidgetItem *)));
    connect(listSongs, SIGNAL(sendLikeCommand(QString)), this, SLOT(setSongLiked(QString)));
    connect(listSongs, SIGNAL(sendDislikeCommand(QString)), this ,SLOT(setSongDisliked(QString)));
    connect(listSongs, SIGNAL(sendRemoveCommand(QString)), this,SLOT(setSongRemove(QString)));
    connect(listSongs, SIGNAL(sendAddIntoSongListCommand(QString)),this, SLOT(setSongAddInto(QString)));
    connect(listSongs, SIGNAL(sendShowSongListInfoCommand(QString)), this ,SLOT(setSongInfoShowed(QString)));

    //歌单项鼠标事件
    connect(listList, SIGNAL(itemClicked(QListWidgetItem *)), this , SLOT(onListListItemClicked(QListWidgetItem *)));
    connect(listList, SIGNAL(itemEntered(QListWidgetItem *)), this , SLOT(onListListItemEntered(QListWidgetItem *)));
    connect(listList, SIGNAL(sendClearListCommand(QString)), this, SLOT(clearSongListInDatabase(QString)));
    connect(listList, SIGNAL(sendDeleteListCommand(QString)), this , SLOT(deleteSongListInDatabase(QString)));
    connect(listList, SIGNAL(sendRenameListCommand(QString)), this, SLOT(renameSongListInDatabase(QString)));


}

int SongList::maxNumInSongList(QString tablename)
{
    QSqlQuery query(db);
    query.exec(QString("select num from %1 as t order by t.num desc limit 0,1;").arg(tablename));
    query.next();
    return query.value(0).toInt();
}

//展示歌单的歌
void SongList::showSongsOfList(QString listName)
{
    listSongs->clear();
    actingSongListName = listName;
    QFont font = QFont("微软雅黑",10,QFont::Normal);
    QSqlQuery query(db);
    //从数据库读取数据
    QString tempcommand = QString("select songName, LikeOrNot from %1 as t order by t.num desc;");
    QString command = tempcommand.arg(listName);
    query.exec(command);
    while(query.next())//每次执行完exec(),指针会调回第一条数据之前
    {
        QString songName= QString(query.value(0).toString());
        QListWidgetItem *item;
        if(query.value(1) == 0)
            item = new QListWidgetItem(QIcon(":/icon/res/favourite_d.ico"),songName);
        else item = new QListWidgetItem(QIcon(":/icon/res/heart.ico"),songName);
        item->setFont(font);
        item->setSizeHint(QSize(230,30));
        listSongs->addItem(item);
    }
}

//初始化，从数据库里读取歌单
void SongList::initSonglist()
{
    QFont font = QFont("微软雅黑",10,QFont::Normal);
    QString cmd = "show tables;";
    QSqlQuery query(cmd);
    while (query.next())
    {
        QString tableName = QString(query.value(0).toString());
        if(tableName == QString("我喜欢的音乐"))
        {
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/res/favourite_d.ico"),tableName);
            item->setFont(font);
            item->setSizeHint(QSize(230,40));
            listList->insertItem(0, item);
        }
        else if(tableName == QString("播放历史"))
        {
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/res/otherlist.ico"),tableName);
            item->setFont(font);
            item->setSizeHint(QSize(230,40));
            listList->insertItem(1, item);
        }
        else
        {
            QListWidgetItem *item = new QListWidgetItem(QIcon(":/icon/res/otherlist.ico"),tableName);
            item->setIcon(QIcon(":/icon/res/otherlist.ico"));
            item->setText(tableName);
            item->setFont(font);
            item->setSizeHint(QSize(40,40));
            listList->addItem(item);
        }
    }
}

void SongList::refuseChangeList(int status)
{
    QFont font = QFont("微软雅黑",10,QFont::Normal);
    QString information;
    if(status == 1)
        information= "此歌单不能删除哦！" ;
    else if(status == 2) information = "此歌单不支持重命名哦！";
    else information = "播放历史不能加歌哦！";


    QMessageBox message(QMessageBox::Warning, "警告", information);
    message.setWindowIcon(QIcon(":/icon/res/icon.png"));
    message.setFont(font);
    message.exec();
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

//    scrollListsWidget->setGeometry(0, 0, 230, ((sizeY-30 ))/3);
//    scrollListsWidget->setMinimumSize(230, ((sizeY-30))/3);

//    scrollSongsWidget->setGeometry(0, 0, 230 , (2*(sizeY-30))/3);
//    scrollSongsWidget->setMinimumSize(230, (2*(sizeY-30))/3);


//    listList->setGeometry(0, 30, 230, ((sizeY-30 ))/3 - 30);
//    listList->setMinimumSize(230 , 260);
//    listSongs->setGeometry(0, 30, 230, (2*(sizeY-30))/3 - 30);
//    listSongs->setMinimumSize(230 , 550);
}

//void SongList::onListSongsDoubleClicked(const QModelIndex &index)
//{
//    int curRow = index.row();
//    emit playMusic(curRow);
//}


//当歌单里的项被点击时
void SongList::onListListItemClicked(QListWidgetItem *item)
{
    showSongsOfList(item->text());
    actingSongListName = item->text();
}

//鼠标移到项上时
void SongList::onListListItemEntered(QListWidgetItem *item)
{
    listList->setTempItem(*item);
}

//根据输入的表明，在数据库建立表格
void SongList::createSongListInDatabase(QString name)
{
    QSqlQuery query(db);
    QString command = "create table %1 (songName varchar(255) ,"
                      "songUrl varchar(255) primary key, likeOrNot int(1), "
                      "artist varchar(255), album varchar(255), cover_image varchar(255),num int(9));";
    query.exec(command.arg(name));
    listList->clear();
    initSonglist();
    showSongsOfList(name);
}

//清空选中歌单的歌曲(存在漏洞：清空我喜欢的音乐的时候，注意取消这些歌的我喜欢)
void SongList::clearSongListInDatabase(QString name)
{
    qDebug()<<"数据库清空操作";
    qDebug()<<name;
    QSqlQuery query(db);


    if(name == QString("我喜欢的音乐"))
    {
        query.exec("select songUrl from 我喜欢的音乐;");
        while(query.next())
        {
            QSqlQuery query1(db);
            QString cmd = "show tables;";
            QSqlQuery query2(cmd);
            while (query2.next())
            {
                QString songUrl = query.value(0).toString();
                QString tableName = QString(query2.value(0).toString());
                QString command = "update %1 set likeOrNot = 0 where songUrl = '%2' ;";
                query1.exec(command.arg(tableName, songUrl));
            }
        }
    }
    QString command = "truncate table %1 ;";
    query.exec(command.arg(name));
    listList->clear();
    initSonglist();
    showSongsOfList(name);
}

//删除选中歌单
void SongList::deleteSongListInDatabase(QString name)
{
    if(name == QString("我喜欢的音乐") || name ==QString("播放历史"))
    {
        refuseChangeList(1);
    }
    else
    {
        qDebug()<<"数据库删除操作";
        qDebug()<<name;
        QSqlQuery query(db);
        QString command = "drop table %1;";
        query.exec(command.arg(name));
        listList->clear();
        initSonglist();
        showSongsOfList("我喜欢的音乐");
    }
}

//重命名歌单
void SongList::renameSongListInDatabase(QString name)
{
    if(name == QString("我喜欢的音乐") || name ==QString("播放历史"))
    {
        refuseChangeList(2);
    }
    else
    {
        qDebug()<<"数据库重命名操作";
        qDebug()<<name;
        QSqlQuery query(db);
        QString newName;
        RenameSongListDialog dlg;
        int status = dlg.exec();
        if(status == 0)
            newName = dlg.getLineEdit()->text();
        else {}
        QString command = "alter table %1 rename to %2;";
        query.exec(command.arg(name, newName));
        listList->clear();
        initSonglist();
        showSongsOfList(newName);
    }
}

//歌曲项鼠标操作
void SongList::onListSongItemEntered(QListWidgetItem *item)
{
    QString songName = item->text();
    QString command = "select likeOrNot from %1 where songName = '%2' ;";
    QSqlQuery query(db);
    query.exec(command.arg(actingSongListName ,songName));
    query.next();
    if(query.value(0).toInt() == 0)
        listSongs->setCurrentSongLikedOrNot(false);
    else listSongs->setCurrentSongLikedOrNot(true);
    listSongs->setTempItem(*item);
}

//设置歌曲为我喜欢
void SongList::setSongLiked(QString name)
{
    QSqlQuery query1(db);
    QString cmd = "show tables;";
    QSqlQuery query2(cmd);
    while (query2.next())
    {
        QString tableName = QString(query2.value(0).toString());
        QString command = "update %1 set likeOrNot = 1 where songName = '%2' ;";
        query1.exec(command.arg(tableName, name));
    }

    QString getInfoCommand = "select * from %1 where songName = '%2';";
    query1.exec(getInfoCommand.arg(actingSongListName, name));
    query1.next();
    QString songUrl = QString(query1.value(1).toString());
    QString likeOrNot = QString(query1.value(2).toString());
    QString artist = QString(query1.value(3).toString());
    QString album = QString(query1.value(4).toString());
    QString album_cover = QString(query1.value(5).toString());

    QString addedSonglist("我喜欢的音乐");
    QString num = QString::number(maxNumInSongList(addedSonglist) + 1);
    QString insertCommand = "insert into %1 values('%2', '%3', '%4', '%5', '%6', '%7', '%8');";

    query1.exec(insertCommand.arg(addedSonglist , name, songUrl, likeOrNot , artist, album, album_cover, num));
    showSongsOfList(actingSongListName);
}

//设置歌曲为我不怎么喜欢
void SongList::setSongDisliked(QString name)
{
    QSqlQuery query1(db);
    QString cmd = "show tables;";
    QSqlQuery query2(cmd);
    while (query2.next())
    {
        QString tableName = QString(query2.value(0).toString());
        QString command = "update %1 set likeOrNot = 0 where songName = '%2' ;";
        query1.exec(command.arg(tableName, name));
    }

    QString command = "delete from 我喜欢的音乐 where songName = '%1' ;";
    query1.exec(command.arg(name));
    showSongsOfList(actingSongListName);
}

//移除歌曲
void SongList::setSongRemove(QString name)
{
    QSqlQuery query(db);
    if(actingSongListName == QString("我喜欢的音乐"))
    {
        QSqlQuery query1(db);
        QString cmd = "show tables;";
        QSqlQuery query2(cmd);
        while (query2.next())
        {
            QString tableName = QString(query2.value(0).toString());
            QString command = "update %1 set likeOrNot = 0 where songName = '%2' ;";
            query1.exec(command.arg(tableName, name));
        }

    }



    QString command = "delete from %1 where songName = '%2' ;";
    query.exec(command.arg(actingSongListName, name));
    showSongsOfList(actingSongListName);
}

//添加到歌单(还没有完善)
void SongList::setSongAddInto(QString name)
{
    QString addedSonglist("哎呦哎呦");
    QSqlQuery query(db);
    QString getInfoCommand = "select * from %1 where songName = '%2';";
    query.exec(getInfoCommand.arg(actingSongListName, name));
    query.next();
    QString songUrl = QString(query.value(1).toString());
    QString likeOrNot = QString(query.value(2).toString());
    QString artist = QString(query.value(3).toString());
    QString album = QString(query.value(4).toString());
    QString album_cover = QString(query.value(5).toString());
    QString num = QString::number(maxNumInSongList(actingSongListName) + 1);


    QString insertCommand = "insert into %1 values('%2', '%3', '%4', '%5', '%6', '%7', '%8');";

    query.exec(insertCommand.arg(addedSonglist , name, songUrl, likeOrNot , artist, album, album_cover, num));
    showSongsOfList(actingSongListName);
}

//弹窗展示歌单信息
void SongList::setSongInfoShowed(QString name)
{
    QFont font = QFont("微软雅黑",10,QFont::Normal);
    QSqlQuery query(db);
    QString getInfoCommand = "select * from %1 where songName = '%2';";
    query.exec(getInfoCommand.arg(actingSongListName, name));
    query.next();
    QString songUrl = QString(query.value(1).toString());
    QString artist = QString(query.value(3).toString());
    QString album = QString(query.value(4).toString());
    QString information = QString("歌曲名称：     %1 \n艺术家：        %2 \n"
                                  "专辑：           %3 \n路径：           %4 ").arg(name, artist, album, songUrl);
    QMessageBox message(QMessageBox::Information, "歌曲信息", information);
    message.setWindowIcon(QIcon(":/icon/res/icon.png"));
    message.setFont(font);
    message.exec();
}
//拖拽或者添加音乐文件

void SongList::addNewSong(QString Path)
{
    playingSongList = actingSongListName;
    SongInfo *info = new SongInfo();
    info->title = "暂无歌曲在播放";
    info->artist = "暂无相关信息";
    tempInfo = info;

    AudioTag tag(Path,tempInfo);
    tag.getAllinfo();
    QSqlQuery query(db);

    QString name = tempInfo->title;
    QString songUrl = Path;
    QString likeOrNot;

    if(actingSongListName == QString("我喜欢的音乐"))
    {
        likeOrNot = "1"; //问题：如果此歌曲原来就存在与别的歌单，而且设置为我喜欢
        QSqlQuery query1(db);
        QString cmd = "show tables;";
        QSqlQuery query2(cmd);
        while (query2.next())
        {
            QString tableName = QString(query2.value(0).toString());
            QString command = "update %1 set likeOrNot = 1 where songUrl = '%2' ;";
            query1.exec(command.arg(tableName, Path));
        }
    }
    else likeOrNot = "0";
    query.exec(QString("select likeOrNot from 我喜欢的音乐 where songUrl = '%2' ;").arg(Path));
    query.next();
    if(query.value(0).toInt() == 1)
        likeOrNot = "1";

    QString artist = tempInfo->artist;
    QString album = tempInfo->album;
    QString pic_url = "";
    if(tempInfo->has_cover)
        pic_url = tempInfo->album_cover;
    else  pic_url = "";
    QString addedSonglist(actingSongListName);
    QString num = QString::number(maxNumInSongList(addedSonglist) + 1);
    QString insertCommand = "insert into %1 values('%2', '%3', '%4', '%5', '%6', '%7', '%8');";
    query.exec(insertCommand.arg(addedSonglist , name, songUrl, likeOrNot , artist, album, pic_url, num));
    showSongsOfList(actingSongListName);

    //从数据库读取数据
    QString readcommand = QString("select songUrl from %1 as t order by t.num desc;");
    query.exec(readcommand.arg(addedSonglist));
    emit clearMusic();
    while(query.next())//每次执行完exec(),指针会调回第一条数据之前
    {
        qDebug()<<query.value(0).toUrl();
        emit changePlaylist(query.value(0).toUrl(), 1);
    }

    delete info;
}

void SongList::onUpdateAudioTagInMainWindow(QString filePath)
{
    if(filePath != "")
    {
        qDebug()<<filePath;
        //filePath = "F:/音乐/手机导入/Jim Brickman - Canon in D Major (Pachelbel’s Canon) [mqms2](1).mp3";
        QSqlQuery query(db);
        QString command("select songName,artist,cover_image from %1 where songUrl = '%2';");
        query.exec(command.arg(playingSongList, filePath));
        query.next();
        SongInfo *songInfo = new SongInfo();
        songInfo->title = query.value(0).toString();
        songInfo->artist = query.value(1).toString();
        qDebug()<<songInfo->title << songInfo->artist;
        AudioTag tag(filePath, songInfo);
        if(query.value(2).toString() != "")
        {
            songInfo->has_cover = 1;
            songInfo->album_cover = query.value(2).toString();
        }
        else
        {
            songInfo->has_cover = 0;
            tag.downloadPic();
            query.exec(QString("update %1 set cover_image = '%2' where songUrl = '%3' ").arg(playingSongList, songInfo->album_cover,filePath));
        }
        tag.idMatch();
        qDebug()<<songInfo->song_id;
        emit sendSongInfo(songInfo);
    delete songInfo;
    }
}

//双击item播放
void SongList::on_ListSongs_doubleClicked(const QModelIndex &index)
{
    playingSongList = actingSongListName;
    int songIndex = index.row();
    QSqlQuery query(db);
    emit clearMusic();
    //从数据库读取数据
    QString readcommand = QString("select songUrl from %1 as t order by t.num desc;");
    query.exec(readcommand.arg(actingSongListName));
    while(query.next())//每次执行完exec(),指针会调回第一条数据之前
    {
        qDebug()<<query.value(0).toUrl();
        emit changePlaylist(query.value(0).toUrl(), 1);
    }
    emit playMusic(songIndex);
}

void SongList::on_addSongBtn_clicked()
{
    SongInfo *info = new SongInfo();
    info->title = "暂无歌曲在播放";
    info->artist = "暂无相关信息";
    tempInfo = info;

    QSqlQuery query(db);
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
        QString direction = fileInfo.filePath();


        AudioTag tag(direction,tempInfo);
        tag.getAllinfo();
        QString name = tempInfo->title;
        QString songUrl = direction;
        QString likeOrNot;

        if(actingSongListName == QString("我喜欢的音乐"))
        {
            likeOrNot = "1"; //问题：如果此歌曲原来就存在与别的歌单，而且设置为我喜欢
            QSqlQuery query1(db);
            QString cmd = "show tables;";
            QSqlQuery query2(cmd);
            while (query2.next())
            {
                QString tableName = QString(query2.value(0).toString());
                QString command = "update %1 set likeOrNot = 1 where songUrl = '%2' ;";
                query1.exec(command.arg(tableName, direction));
            }
        }
        else likeOrNot = "0";
        query.exec(QString("select likeOrNot from 我喜欢的音乐 where songUrl = '%2' ;").arg(direction));
        query.next();
        if(query.value(0).toInt() == 1)
            likeOrNot = "1";


        QString artist = tempInfo->artist;
        QString album = tempInfo->album;
        QString pic_url = "";
        if(tempInfo->has_cover) //问题：如果此歌曲曾经在别的歌单播放过，重新下载是否会报错？如果歌曲重名了，那么图片的下载和导出可能出错
            pic_url = tempInfo->album_cover;
        else  pic_url = "";
        QString addedSonglist(actingSongListName);
        QString num = QString::number(maxNumInSongList(addedSonglist) + 1);
        QString insertCommand = "insert into %1 values('%2', '%3', '%4', '%5', '%6', '%7', '%8');";
        query.exec(insertCommand.arg(addedSonglist , name, songUrl, likeOrNot , artist, album, pic_url, num));
    }

    showSongsOfList(actingSongListName);
    emit clearMusic();
    //从数据库读取数据
    QString readcommand = QString("select songUrl from %1 as t order by t.num desc;");
    query.exec(readcommand.arg(actingSongListName));
    while(query.next())//每次执行完exec(),指针会调回第一条数据之前
    {
        qDebug()<<query.value(0).toUrl();
        emit changePlaylist(query.value(0).toUrl(), 1);
    }
    delete info;
}

//void SongList::onChangeListSongs(QString fileName, int behaviorIndex)
//{
//    if(behaviorIndex == 1)
//    {
//        listSongs->addItem(fileName);
//    }
//}

//指针类型函数，返回pushbutton指针
QPushButton *SongList::getCreateMusicListButton() const
{
    return createMusicListButton;
}

