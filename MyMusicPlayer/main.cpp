#include "mainwidget.h"
//***************************测试用：
#include "suspensionwindow.h"
#include "downloadwindow.h"
#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//        MainWidget w;
//        w.show();
//    SuspensionWindow w;
//    w.show();
    DownloadWindow w("34218582","东南大学校歌");
    w.show();

    return a.exec();
}
