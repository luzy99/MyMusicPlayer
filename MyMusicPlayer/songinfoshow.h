#ifndef SONGINFOSHOW_H
#define SONGINFOSHOW_H

#include <QWidget>
#include"songinfo.h"
#include<QLabel>
#include"diskwidget.h"
#include <QEvent>
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
    QPushButton *getMvButton() const;
    void setMvButton(QPushButton *value);

    void setUser(const QString &value);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void changePage();
    void sendAddIntoSongListCommand(QString,QString);
    void playMV(); //通知首页播放MV

public slots:
    void changeSong(SongInfo &m_song_info);
    void diskRotateStart();
    void diskRotateStop();

private slots:
    void on_downloadButton_clicked();
    void on_shareButton_clicked();
    void on_pageBackButton_clicked();
    void on_starButton_clicked();

private:
    QString songId;
    QString songName;
    QString User;

    SongInfo tempSongInfo;

    QLabel *titleShow;
    QPushButton *mvButton;
    QLabel *spacingLabel;
    QLabel *artistShow;
    diskWidget *disk;
    QVBoxLayout *outerLayout;
    QHBoxLayout *buttonLayout;
    QPushButton *starButton;
    QPushButton *downloadButton;
    QPushButton *shareButton;
    QPushButton *pageBackButton;
};

#endif // SONGINFOSHOW_H
