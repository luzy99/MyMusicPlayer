#include "mainwindow.h"
//***************************测试用：
#include "skinchange.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SkinChange w;
    w.show();

//     MainWindow w;
//     w.show();

    return a.exec();
}
