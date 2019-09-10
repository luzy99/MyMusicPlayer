#pragma once

#include <QObject>
#include<QSplashScreen>
#include<QProgressBar>
#include<QPaintEvent>

class SplashScreen:public QSplashScreen
{
    Q_OBJECT
public:
     SplashScreen(QPixmap pixmap,QWidget*parent=nullptr);
    ~SplashScreen();
       //设置进度区间
       void setRange(int min, int max);
public slots:
       //更新进度
       void updateProgress(int num);
       void showProgressMessage(int total, const QString& msg);

private:
       QProgressBar *bar; //进度条
};


