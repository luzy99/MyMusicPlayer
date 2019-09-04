#include "mainwindow.h"
//***************************测试用：
#include "errorwindow.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     MainWindow w;
     w.show();
//    ErrorWindow w("测试");
//    w.show();
//    w.showInstantly();

    return a.exec();
}
