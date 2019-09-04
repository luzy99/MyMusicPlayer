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
    void initSignalsAndSlots();
    ~SongInfoShow();
public slots:
    void changeSong(SongInfo &m_song_info);
    void diskRotateStart();
    void diskRotateStop();

private slots:
    void on_downloadButton_clicked();

private:
    QString songId;
    QString songName;

    QLabel *titleShow;
    QLabel *artistShow;
    diskWidget *disk;
    QVBoxLayout *outerLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *likeButton;
    QPushButton *starButton;
    QPushButton *downloadButton;
    QPushButton *shareButton;
};

#endif // SONGINFOSHOW_H
