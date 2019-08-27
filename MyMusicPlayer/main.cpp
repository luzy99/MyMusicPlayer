#include "mainwidget.h"
#include "suspensionwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget w;
    w.show();
//    SuspensionWindow w;
//    w.show();

    return a.exec();
}
