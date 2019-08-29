#include "lyricwidget.h"
#include "lyricdownload.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LyricWidget w;


    LyricDownload *ly1;
    QString result;
    ly1->DownloadLyric("1366903039",true);
    w.analyzeLrcContent("1366903039");
    w.show();
    //qDebug()<<result;
    //ly1->DisposeLryic(result);
    //qDebug()<<result;
    //ly1->AddLyricTag(result,L"21",L"hhh",L"srt",L"album");
    //qDebug()<<result;

    return a.exec();
}
