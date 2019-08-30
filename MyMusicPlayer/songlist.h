#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>
#include <QWidget>
//#include <QListWidget>
#include<QGroupBox>
#include <QScrollArea>
#include<QLabel>
#include<QPushButton>
#include<QSqlDatabase>
#include"mylistlistwidget.h"
#include "mylistsongwidget.h"

class SongList : public QWidget
{
    Q_OBJECT
public:
    explicit SongList(QWidget *parent = nullptr);

    void showSongsOfList(QString listName);

    void initSonglist();

    void refuseChangeList(int);


    QPushButton *getCreateMusicListButton() const;

signals:
    void playMusic(int SongIndex); //触发播放音乐



public slots:
    void resetGeometry();
    //void onListSongsDoubleClicked(const QModelIndex &index); //双击歌曲项时的槽函数
    void onChangeListSongs(QString fileName, int behaviorIndex); //处理changeListSongs信号的槽函数

    //歌单的槽函数
    void onListListItemClicked(QListWidgetItem* item);

    void onListListItemEntered(QListWidgetItem* item);

    void createSongListInDatabase(QString);

    void clearSongListInDatabase(QString);

    void deleteSongListInDatabase(QString);

    void renameSongListInDatabase(QString);


    //播放列表槽函数
    void onListSongItemEntered(QListWidgetItem *);

    void setSongLiked(QString);

    void setSongDisliked(QString);

    void setSongRemove(QString);

    void setSongAddInto(QString);

    void setSongInfoShowed(QString);


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

    QListWidgetItem playingSongList;
    QString actingSongListName;
};

#endif // SONGLIST_H
