#ifndef MUSICPLAYBAR_H
#define MUSICPLAYBAR_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer> //音乐播放器
#include <QMediaPlaylist> //播放列表

class MusicPlayBar : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlayBar(QWidget *parent = nullptr);
    QColor themeColor; //主题色

signals:

public slots:
    void changeThemeColor(QColor);
    void onStateChanged(QMediaPlayer::State state);
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);

private:
    QPushButton *previousBtn; //上一首歌
    QPushButton *playBtn; //播放&暂停，默认选中时在播放
    QPushButton *nextBtn; //下一首歌
    QLabel *currentTimeLabel; //已播放时长
    QSlider *playSlider; //歌曲进度条
    QLabel *totalTimeLabel; //总时长
    QPushButton *muteBtn; //静音&打开声音，默认选中时静音
    QSlider *soundSlider; //歌曲进度条
    QPushButton *playModeBtn; //切换播放模式
    QPushButton *showLyricsBtn; //显示歌词&隐藏歌词，默认在选中时显示歌词弹幕
    QPushButton *playSpeed; //倍数
    float currentSpeed; //当前播放速度
    QMediaPlayer *player; //播放器
    QMediaPlaylist *playlist; //播放列表
    QString durationTime; //总长度
    QString positionTime; //当前播放到的位置
};

#endif // MUSICPLAYBAR_H
