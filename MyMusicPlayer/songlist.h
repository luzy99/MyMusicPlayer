#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include<QGroupBox>
#include <QScrollArea>
#include<QLabel>
#include<QPushButton>
#include <QUrl>

class SongList : public QWidget
{
    Q_OBJECT
public:
    explicit SongList(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化设定信号与槽

signals:
    void playMusic(int SongIndex); //触发播放音乐
    void changePlaylist(QUrl url,int behaviorIndex);  //改变播放列表的信号，第二个参数代表操作类型，1:增加

public slots:
    void resetGeometry();
    void on_ListSongs_doubleClicked(const QModelIndex &index); //双击歌曲项时的槽函数
    void on_addSongBtn_clicked(); //增加歌曲时触发
    void onChangeListSongs(QString fileName,int behaviorIndex); //处理changeListSongs信号的槽函数

private:
    QGroupBox *g_container;
    QScrollArea *scrollLists;
    QScrollArea *scrollSongs;
    QWidget *scrollListsWidget;
    QWidget *scrollSongsWidget;
    QListWidget *listList;
    QListWidget *listSongs;
    QLabel *labelLists;
    QLabel *labelSongs;
    QListWidgetItem *lovedMusiclist;
    QListWidgetItem *createdMusicList;
    QPushButton *createMusicListButton;
    QPushButton *addSongBtn;
};

#endif // SONGLIST_H
