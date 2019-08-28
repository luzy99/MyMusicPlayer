#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include<QGroupBox>
#include <QScrollArea>
#include<QLabel>
#include<QPushButton>
class SongList : public QWidget
{
    Q_OBJECT
public:
    explicit SongList(QWidget *parent = nullptr);

signals:
    void playMusic(int SongIndex); //触发播放音乐

public slots:
    void resetGeometry();
    //void onListSongsDoubleClicked(const QModelIndex &index); //双击歌曲项时的槽函数
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
};

#endif // SONGLIST_H
