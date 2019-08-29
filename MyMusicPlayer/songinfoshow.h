#ifndef SONGINFOSHOW_H
#define SONGINFOSHOW_H

#include <QWidget>
#include"songinfo.h"
#include<QLabel>
#include"diskwidget.h"

class SongInfoShow : public QWidget
{
    Q_OBJECT

public:
    explicit SongInfoShow(QWidget *parent,SongInfo &m_song_info);
    SongInfoShow(QWidget *parent = 0);
    ~SongInfoShow();
public slots:
    void changeSong(SongInfo &m_song_info);
private:
    QLabel *titleShow;
    QLabel *artistShow;
    diskWidget *disk;
};

#endif // SONGINFOSHOW_H
