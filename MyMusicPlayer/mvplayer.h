#ifndef MVPLAYER_H
#define MVPLAYER_H

#include <QWidget>
#include <QDialog>
#include<QAction>
#include<QSlider>
#include<QLabel>
#include<QPushButton>
#include <QMediaPlayer>
#include <qvideowidget.h>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QComboBox>
#include <QtNetwork/QNetworkRequest>//网络请求
#include <QtNetwork/QNetworkReply>//网络响应
#include <QtNetwork/QNetworkAccessManager>//网络管理
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include<QRegExp>//正则
#include<QPainter>
#include<QResizeEvent>
#include<QDateTime>
#include<QTimer>

class MvPlayer : public QDialog
{
    Q_OBJECT
public:
    explicit MvPlayer(QWidget *parent = nullptr);
    bool getMvUrls(QString mv_id);

signals:
    void MvWidgetClose();

public slots:
    void play();
    void setPosition(int position);
    void mediaStateChanged(QMediaPlayer::State state);//播放状态改变
    void positionChanged(qint64 position);//位置变化
    void durationChanged(qint64 duration);//时长变化
    void resizeEvent(QResizeEvent *ev);//重置进度条位置
    void onComboBoxChanged(const QString &key);//画质切换时调用
    void on_fullScreenBtn_clicked();//全屏按钮
    void hideBar();//隐藏进度条
    void handleError();//错误处理

    void mouseMoveEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent  *event);
    void wheelEvent(QWheelEvent * event);
    void closeEvent(QCloseEvent *event);

private:
    void initSignalsAndSlots(); //初始化信号与槽
    QMediaPlayer mediaPlayer;//播放器
    QVideoWidget *videoWidget;//视频窗口
    QPushButton *playButton;//播放按钮
    QPushButton *fullScreenBtn;//全屏按钮
    QSlider *positionSlider;//进度条
    QWidget *controlBar;//底部控制条
    QHBoxLayout *controlLayout;
    QLabel *currentTimeLabel;//当前时间
    QLabel *totalTimeLabel;//总时间
    QComboBox *qualityCom;//画质下拉条
    QPushButton *existBtn; //退出按钮
    QTimer *timer;//定时器控制隐藏
    QLabel *statusLabel;

    QString mvId;
    QString mvTitle;//mv标题
    QString mvArtist;//歌手
    QMap<QString, QString> urlMap;//记录不同清晰度的视频url 格式：<清晰度，url>
};

#endif // MVPLAYER_H
