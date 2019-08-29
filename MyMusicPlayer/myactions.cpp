#include "myactions.h"

MyActions::MyActions(QObject *parent) : QObject(parent)
{
    //初始化关闭窗口的
    actClose = new QAction;
    actClose->setIcon(QIcon(":/icon/res/close.png"));
    actClose->setText("关闭");
    connect(actClose,SIGNAL(triggered()),
            this->parent(),SLOT(close()));
}
