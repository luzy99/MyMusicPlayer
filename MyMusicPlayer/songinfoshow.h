#ifndef SONGINFOSHOW_H
#define SONGINFOSHOW_H

#include"diskwidget.h"
#include"songinfo.h"
#include <QWidget>
#include<QLabel>


class SongInfoShow : public QWidget
{
    Q_OBJECT

public:
    explicit SongInfoShow(QWidget *parent,SongInfo &m_song_info);
    SongInfoShow(QWidget *parent = nullptr);
    ~SongInfoShow();

public slots:
    void changeSong(SongInfo &m_song_info);
    void Stop();
    void Start();

private:
    QLabel *titleShow;
    QLabel *artistShow;
    diskWidget *disk;
};

#endif // SONGINFOSHOW_H
