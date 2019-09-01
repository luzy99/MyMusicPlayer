#ifndef AUDIOTAG_H
#define AUDIOTAG_H
#include "songinfo.h"
#include "stdio.h"
#include <QFile>
#include <QtNetwork/QNetworkRequest>//网络请求
#include <QtNetwork/QNetworkReply>//网络响应
#include <QtNetwork/QNetworkAccessManager>//网络管理
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QEventLoop>
#include<QRegExp>//正则

class AudioTag
{
public:
    AudioTag(QString f_url, SongInfo *&si);
    ~AudioTag();
    //解析mp3中的封面图片，tag_content表示原始标签信息，tag_index表示图片信息起始地址，tag_size表示标签大小
    bool GetAlbumCover(string& tag_content,int tag_index,int tag_size);
    bool getAllinfo();//获取mp3中标题，艺术家，年份等信息，存入成员对象m_song_info中
    bool idMatch();//根据m_song_info中的标题，艺术家信息匹配网易云的歌曲id
    bool downloadPic();//通过id下载封面图片
    bool mvIdMatch();//通过song_id匹配mv_id
private:
    SongInfo* m_song_info;
    FILE* fp;
};

#endif // AUDIOTAG_H
