#include "mainwidget.h"
#include <QApplication>
#include "createsonglistdialog.h"



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWidget w;
    w.show();
//    CreateSongListDialog c;
//    c.show();
    return a.exec();
}
