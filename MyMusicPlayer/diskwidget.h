#ifndef DISKWIDGET_H
#define DISKWIDGET_H

#include <QPixmap>
#include <QWidget>

class QTimer;
class diskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit diskWidget(QWidget *parent = nullptr,int r = 100);//构造函数，传入图片半径
    void setRadius(int radius);
    void setRate(double rate);
    void rotateStop();//旋转暂停
    void rotateStart();//旋转开始

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void rotate();
    void changePic(QString newPicPath);//切换图片

private:

    QPixmap coverPic;//封面图片
    QPixmap diskPic;    //唱片图
    int count;
    QTimer *timer;
    int i_radius;       //半径
    double d_rate;  //速率
    QString pixmapPath; //图片路径
    bool paintFlag;  //用于记录是否首次加载图片，判断是否需要设置蒙版切图
};

#endif // DISKWIDGET_H
