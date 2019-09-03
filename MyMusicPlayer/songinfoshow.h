#ifndef SONGINFOSHOW_H
#define SONGINFOSHOW_H

#include <QWidget>
#include"songinfo.h"
#include<QLabel>
#include"diskwidget.h"
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QPushButton>

class SongInfoShow : public QWidget
{
    Q_OBJECT

public:
    explicit SongInfoShow(SongInfo &m_song_info,QWidget *parent = nullptr);
    SongInfoShow(QWidget *parent = nullptr);
    ~SongInfoShow();
public slots:
    void changeSong(SongInfo &m_song_info);
    void diskRotateStart();
    void diskRotateStop();

private:
    QLabel *titleShow;
    QLabel *artistShow;
    diskWidget *disk;
    QVBoxLayout *vLayout;
    QHBoxLayout *hLayout;
    QPushButton *likeButton;
    QPushButton *starButton;
    QPushButton *moreInfoButton;
    QWidget *buttonBar;
};

#endif // SONGINFOSHOW_H
