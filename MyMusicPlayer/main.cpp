#include "mainwindow.h"
//***************************测试用：
#include "lyricspost.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     MainWindow w;
     w.show();

//    LyricsPost w("28854853");
//    w.show();

    return a.exec();
}
