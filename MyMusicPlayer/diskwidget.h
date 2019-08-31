#ifndef DISKWIDGET_H
#define DISKWIDGET_H

#include <QPixmap>
#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QBitmap>
#include<QPixmap>
#include <QTimer>
#include <QDebug>
#include<QImage>
#include<QLabel>

class QTimer;
class diskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit diskWidget(QWidget *parent = nullptr,int r = 100);
    void setRadius(int radius);
    void setRate(double rate);
    void rotateStop();//旋转暂停
    void rotateStart();//旋转开始
    void nextSong();//切换下一曲

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void rotate();
    void changePic(QString newPicPath);//切换图片

private:

    QPixmap coverPic;
    QPixmap diskPic;    //唱片图
    QPixmap needlePic;//唱针图
    int count;
    QTimer *timer;  //旋转动画计时器
    QTimer *stopTimer;  //暂停动画计时器
    int i_radius;       //半径
    double d_rate;  //速率
    QString pixmapPath; //图片路径
    bool paintFlag;  //用于记录是否首次加载图片，判断是否需要设置蒙版切图
    int needleCount;
    int animiMode;  //动画模式，1：播放，2：暂停，3：切歌
    bool needleFlag;//切歌时动画标志
};

#endif // DISKWIDGET_H
