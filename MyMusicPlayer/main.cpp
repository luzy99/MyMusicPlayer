#include "mainwindow.h"
//***************************测试用：

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     MainWindow w;
     w.show();

    return a.exec();
}
