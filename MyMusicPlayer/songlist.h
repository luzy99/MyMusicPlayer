#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>
#include <QWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QSqlDatabase>
#include <QUrl>
#include "mylistlistwidget.h"
#include "mylistsongwidget.h"
#include "songinfo.h"

class SongList : public QWidget
{
    Q_OBJECT
public:
    explicit SongList(QWidget *parent = nullptr);

    void showSongsOfList(QString listName);

    void initSonglist();

    void refuseChangeList(int);

    void initSignalsAndSlots(); //初始化设定信号与槽

    int maxNumInSongList(QString);//返回数据表中num最大值

    QPushButton *getCreateMusicListButton() const;

signals:
    void playMusic(int SongIndex); //触发播放音乐
    void clearMusic();
    void changePlaylist(QUrl url,int behaviorIndex);  //改变播放列表的信号，第二个参数代表操作类型，1:增加
    void sendSongInfo(SongInfo *);

public slots:
    void resetGeometry();
    //void onListSongsDoubleClicked(const QModelIndex &index); //双击歌曲项时的槽函数
    //void onChangeListSongs(QString fileName, int behaviorIndex); //处理changeListSongs信号的槽函数

//歌单的槽函数
    void onListListItemClicked(QListWidgetItem* item);
    void onListListItemEntered(QListWidgetItem* item);
    void createSongListInDatabase(QString);
    void clearSongListInDatabase(QString);
    void deleteSongListInDatabase(QString);
    void renameSongListInDatabase(QString);
    void on_ListSongs_doubleClicked(const QModelIndex &index); //双击歌曲项时的槽函数
    void on_addSongBtn_clicked(); //增加歌曲时触发

//播放列表槽函数
    void onListSongItemEntered(QListWidgetItem *);
    //set开头的槽函数是与右击菜单栏有关的数据库修改操作
    void setSongLiked(QString);
    void setSongDisliked(QString);
    void setSongRemove(QString);
    void setSongAddInto(QString);
    void setSongInfoShowed(QString);
    //拖拽或者添加文件的槽函数
    void addNewSong(QString);
    void onUpdateAudioTagInMainWindow(QString);

private:
    QSqlDatabase db;

    QGroupBox *g_container;
    QScrollArea *scrollLists;
    QScrollArea *scrollSongs;
    QWidget *scrollListsWidget;
    QWidget *scrollSongsWidget;
    MyListListWidget *listList;
    MyListSongWidget *listSongs;
    QLabel *labelLists;
    QLabel *labelSongs;
    QPushButton *createMusicListButton;
    QPushButton *addSongBtn;
    QString playingSongList;
    QString actingSongListName;

    SongInfo *tempInfo;
};

#endif // SONGLIST_H

