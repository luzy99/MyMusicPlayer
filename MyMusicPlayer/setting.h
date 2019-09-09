#ifndef SETTING_H
#define SETTING_H

#include<QWidget>
#include<QObject>
#include<stdio.h>
#include<QTimer>
#include"windowsx.h"
#include<QString>
#include<QTime>
#include<QSpinBox>
#include<QLineEdit>
#include<QLabel>
#include<QLayout>
#include<QGroupBox>
#include<QPushButton>
#include<QCheckBox>
#include<QMouseEvent>

class timerStop:public QWidget
{

    Q_OBJECT

public:
    timerStop(QWidget *parent=nullptr);
    ~timerStop();
    void createDoubleSpinBoxes();
    void createLineEditBoxes();
    qint64 getTimer();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

public slots:
    void on_ok_btn_click();

public:
        QTimer m_timer1;
private:
    qint64 timerOffset;
    QSpinBox *hour;

    QSpinBox *min;
    QGroupBox*delayedTime;
    QGroupBox*assignTime;
    QLineEdit*line;
    QCheckBox *delayed;
    QCheckBox *assigned;
    QPushButton *ok_btn;

    QPoint mouseStartPoint;
    QPoint windowsStartPoint;

};

#endif
