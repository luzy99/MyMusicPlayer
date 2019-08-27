#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include<QGroupBox>
#include <QScrollArea>
class SongList : public QWidget
{
    Q_OBJECT
public:
    explicit SongList(QWidget *parent = nullptr);


public slots:
    void resetGeometry();

private:
    QGroupBox *g_container;
    QScrollArea *scrollLists;
    QScrollArea *scrollSongs;
    QWidget *scrollListsWidget;
    QWidget *scrollSongsWidget;
    QListWidget *listList;
    QListWidget *listSongs;
};

#endif // SONGLIST_H
