#ifndef MUSICPLAYBAR_H
#define MUSICPLAYBAR_H

#include <QWidget>
#include <QEvent>
#include <QPushButton>
#include<QGroupBox>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer> //音乐播放器
#include <QMediaPlaylist> //播放列表
#include <QHBoxLayout>
#include <QUrl>

class MusicPlayBar : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPlayBar(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化设定信号与槽
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void positionChanged(qint64 postion); //位置改变时发送的型号
    void songChanged(QString songId,bool translate); //切歌时发送的信号
    void updateAudioTag(QString currentFilePath); //更新歌曲信息
    void becomePlaying(); //表示接下来播放
    void becomePausing(); //表示接下来暂停
    void positionStop(qint64); //发送当前停止的位置
    void translateChanged(); //表示翻译模式发生变化
    void showLyricsBarrage(bool show); //表示显示&隐藏底部歌词弹幕

public slots:
    void changeThemeColor(QColor);
    void onStateChanged(QMediaPlayer::State state);
    void onDurationChanged(qint64 duration);
    void onPositionChanged(qint64 position);
    void onChangePlaylist(QUrl url,int behaviorIndex); //处理信号changePlaylist的函数
    void onPlayMusic(int SongIndex); //处理信号playMusic的函数
    void onClearMusic();
    void onBlockSignals(bool block); //拖动歌词时触发的函数
    void onPositionDraggedTo(qint64 newPosition); //歌曲拖动后触发的槽函数

    void on_previousBtn_clicked(); //上一首被点击时触发
    void on_playBtn_clicked(); //播放&暂停键点击触发
    void on_nextBtn_clicked(); //下一首被点击时触发
    void on_playSlider_sliderPressed(); //进度条被点击时触发
    void on_playSlider_sliderReleased(); //用户拖动进度条时更改位置，释放时触发
    void on_muteBtn_clicked(); //静音被点击时触发
    void on_soundSlider_sliderPressed();//进度条被点击时触发
    void on_soundSlider_sliderReleased(); //用户拖动进度条时更改位置，释放时触发
    void on_soundSlider_valueChanged(int value); //音量滑条被拖动时触发
    void on_playModeBtn_clicked(); //切换播放模式时触发
    void on_showLyricsBtn_clicked(); //显示&隐藏底部弹幕时触发
    void on_translateBtn_clicked(); //翻译歌词时触发
    void on_playSpeedBtn_clicked(); //切换倍速时触发

private:
    QHBoxLayout *layout;

    QMediaPlayer *player; //播放器
    QMediaPlaylist *playlist; //播放列表
    QPushButton *previousBtn; //上一首歌
    QPushButton *playBtn; //播放&暂停
    QPushButton *nextBtn; //下一首歌
    QLabel *currentTimeLabel; //已播放时长
    QSlider *playSlider; //歌曲进度条
    QLabel *totalTimeLabel; //总时长
    QPushButton *muteBtn; //静音&打开声音
    QSlider *soundSlider; //歌曲进度条
    QPushButton *playModeBtn; //切换播放模式
    QPushButton *showLyricsBtn; //显示歌词&隐藏歌词
    QPushButton *translateBtn; //翻译外语歌词
    QPushButton *playSpeedBtn; //倍数
    QLabel *dragLabel; //拖动区域

    double currentSpeed; //当前播放速度
    QString durationTime; //总长度
    QString positionTime; //当前播放到的位置

    bool block; //音乐播放位置改变时是否将它传递给进度条

};

#endif // MUSICPLAYBAR_H
