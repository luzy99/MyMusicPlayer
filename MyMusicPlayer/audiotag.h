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
    //获取音频文件的专辑封面，并保存到临时目录
    //image_type：用来接收封面的格式 0:jpg, 1:png, 2:gif
    //file_name: 指定保存的专辑封面的文件名，如果为nullptr，则使用默认的文件名
    //返回值：专辑封面的保存路径
    ~AudioTag();
    bool GetAlbumCover(string& tag_content,int tag_index,int tag_size);
    bool getAllinfo();
    bool idMatch();
    bool downloadPic();
private:
    SongInfo* m_song_info;
    FILE* fp;
};

#endif // AUDIOTAG_H
