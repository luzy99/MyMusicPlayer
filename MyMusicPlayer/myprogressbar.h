//这是我自定义的圆形进度条
#ifndef MYPROGRESSBAR_H
#define MYPROGRESSBAR_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>

class MyProgressBar : public QWidget
{
    Q_OBJECT
public:
    explicit MyProgressBar(QWidget *parent = nullptr);
    void setPersent(int value);

protected:
    void paintEvent(QPaintEvent *);

signals:

public slots:

private:
    int rotateAngle; //旋转角度
    int persent; //完成进度
};

#endif // MYPROGRESSBAR_H
