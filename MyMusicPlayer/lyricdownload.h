#ifndef LYRICDOWNLOAD_H
#define LYRICDOWNLOAD_H

#include <QObject>
#include <iostream>
#include <QtNetwork/QNetworkRequest>//网络请求
#include <QtNetwork/QNetworkReply>//网络响应
#include <QtNetwork/QNetworkAccessManager>//网络管理
#include <QFile>
#include <QUrl>
#include <QtDebug>
#include <QEventLoop>
using namespace std;


class LyricDownload : public QObject
{
    Q_OBJECT
public:
    explicit LyricDownload(QObject *parent = nullptr);
public:
    LyricDownload();
   // ~LyricDownload();


    //搜索框得到歌曲id
    //结果保存到result中
    //download_translate(从界面'译'按钮得到)参数指定是否下载带翻译的歌词
    static bool DownloadLyric(const wstring& song_id,wstring&result,bool download_translate);

    //对从网易云音乐下载的歌词进行处理，转换成正确的歌词文本
    static bool DisposeLryic(wstring& lyric_str);

    //在歌词前面加上标签信息
    static void AddLyricTag(wstring& lyric_str, const wstring& song_id, const wstring& title = (L""), const wstring& artist = (L""), const wstring& album = (L""));

public slots:
};

#endif // LYRICDOWNLOAD_H
