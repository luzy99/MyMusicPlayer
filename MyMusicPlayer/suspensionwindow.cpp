//这是一个悬浮框
//用来和主窗口互换视图
#include "suspensionwindow.h"

SuspensionWindow::SuspensionWindow(QWidget *parent) : QMainWindow(parent)
{
    this->setFixedSize(400,100); //悬浮窗大小不可改变
}
