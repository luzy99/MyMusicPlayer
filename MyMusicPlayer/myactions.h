//创建播放器使用过程中需要的Actions
//以方便程序的复用
#ifndef MYACTIONS_H
#define MYACTIONS_H

#include <QObject>
#include <QAction>

class MyActions : public QObject
{
    Q_OBJECT
public:
    explicit MyActions(QObject *parent = nullptr);
    QAction *actClose;

public slots:

};

#endif // MYACTIONS_H
