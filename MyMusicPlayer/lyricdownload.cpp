#include "lyricdownload.h"
#include <QFile>
#include <QIODevice>
#include <stdio.h>
#include <fstream>
#include <iostream>
using namespace std;

LyricDownload::LyricDownload(QObject *parent)
    : QObject(parent)
{

}

bool LyricDownload::DownloadLyric(const QString& song_id,QString &result, bool download_translate)
{
    QString lyric_url;
    if(!download_translate)
    {//下载不带翻译的歌词文
        lyric_url = "http://music.163.com/api/song/media?id=" + song_id;
    }
    else
    {//获取带翻译的歌词文本
        lyric_url = "http://music.163.com/api/song/lyric?os=osx&id=" + song_id + "&lv=-1&kv=-1&tv=-1";
    }

    //将wstring形式的url转换为QString类型
    qDebug() << lyric_url;

    //构造请求
    QNetworkRequest request;
    request.setUrl(QUrl(lyric_url));
    //构造网络管理
    QNetworkAccessManager* manager=new QNetworkAccessManager;
    // 发送请求
    QNetworkReply *pReply = manager->get(request);

    //设置事件循环，等待歌词资源下载完毕
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    //检测http请求的状态码
    int nHttpCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    qDebug()<<nHttpCode;

    QByteArray bytes = pReply->readAll();
    QString Qresult=bytes;
    //qDebug()<<Qresult;
    result=Qresult;
    //result=Qresult.toStdWString();
    QString path="D:/Git/clone/Lyric/Lyric"+song_id+ ".rlc";
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(result.toUtf8());
    file.close();
    if(!(200==nHttpCode))
    {
        return true;
    }
    else
    {
        return false;
    }


}

//bool LyricDownload::DisposeLryic(QString &lyric_str)
//{
//    size_t index1 = lyric_str.find('[');	//查找第1个左中括号，即为歌词开始的位置下标
//    if (index1 == string::npos)
//    {
//        return false;
//    }
//    lyric_str = lyric_str.substr(index1, lyric_str.size() - index1 - 13);
//    if (!lyric_str.empty() && lyric_str.back() == L'\"')
//        lyric_str.pop_back();

//    for (int i{}; i < static_cast<int>(lyric_str.size() - 1); i++)
//    {
//        //如果歌词中含有字符串“\r\n”或“\n\n”，则将其转换成为两个转义字符\r\n，删掉两个多余的字符
//        if (i < static_cast<int>(lyric_str.size() - 3))
//        {
//            if ((lyric_str[i] == '\\' && lyric_str[i + 1] == 'r' && lyric_str[i + 2] == '\\' && lyric_str[i + 3] == 'n')
//                || (lyric_str[i] == '\\' && lyric_str[i + 1] == 'n' && lyric_str[i + 2] == '\\' && lyric_str[i + 3] == 'n'))
//            {
//                lyric_str[i] = '\r';
//                lyric_str[i + 1] = '\n';
//                lyric_str.erase(i + 2, 2);
//            }
//        }
//        //如果歌词中含有字符串“\r”，则将其转换成为两个转义字符\r\n
//        if (lyric_str[i] == '\\' && lyric_str[i + 1] == 'r')
//        {
//            lyric_str[i] = '\r';
//            lyric_str[i + 1] = '\n';
//        }
//        //如果歌词中含有字符串“\n”，则将其转换成为两个转义字符\r\n
//        if (lyric_str[i] == '\\' && lyric_str[i + 1] == 'n')	//将歌词文本中的“\n”转换成回车符\r\n
//        {
//            lyric_str[i] = '\r';
//            lyric_str[i + 1] = '\n';
//        }
//        //如果歌词中含有字符串“\"”，则删除反斜杠“\”
//        if (lyric_str[i] == '\\' && lyric_str[i + 1] == '\"')
//        {
//            lyric_str.erase(i, 1);
//        }
//    }
//    return true;
//}

//void LyricDownload::AddLyricTag(wstring &lyric_str, const wstring &song_id, const wstring &title, const wstring &artist, const wstring &album)
//{
//    wstring tag_info{};

//    QString Qtmp=QString("[id:%1]\r\n").arg(song_id);
//    wstring tmp=Qtmp.toStdWString();
//    tag_info += tmp;
//    if (lyric_str.find(L"[ti:") == wstring::npos)
//    {
//        QString Qtmp=QString("[ti:%1]\r\n").arg(title);
//        wstring tmp=Qtmp.toStdWString();
//        tag_info += tmp;

//    }
//    if (lyric_str.find(L"[ar:") == wstring::npos)
//    {
//        QString Qtmp=QString("[ar:%1]\r\n").arg(artist);
//        wstring tmp=Qtmp.toStdWString();
//        tag_info += tmp;
//    }
//    if (lyric_str.find(L"[al:") == wstring::npos)
//    {
//        QString Qtmp=QString("[al:%1]\r\n").arg(album);
//        wstring tmp=Qtmp.toStdWString();
//        tag_info += tmp;
//    }
//    lyric_str = tag_info + lyric_str;

//    wstring path=L"D://Git//clone//Lyric"+song_id;
//    FILE *fp=_wfopen(path.c_str(),L"w");
//    fwrite(lyric_str.c_str(),1,lyric_str.length(),fp);
//    fclose(fp);
//}
