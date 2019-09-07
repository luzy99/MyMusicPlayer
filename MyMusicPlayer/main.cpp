#include "mainwindow.h"
//***************************测试用：
#include "showlocal.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

     MainWindow w;
     w.show();
//    SongInfo *newInfo = new SongInfo;
//    newInfo->song_id = "101126";
//    AudioTag tag("",newInfo);
//    tag.mvIdMatch();
//    qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();

    return a.exec();
}
