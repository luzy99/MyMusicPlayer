#include "searchdata.h"

SearchData::SearchData(QObject *parent) : QObject(parent)
{

}

bool SearchData::searchOnline(QString songName,SongInfo *&songInfos)
{
    QString url ="http://music.163.com/api/search/pc/?s="
            +songName
            +"&limit=20&type=1&offset=0";
    //构造请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    //构造网络管理
    QNetworkAccessManager* manager=new QNetworkAccessManager;
    // 发送请求
    QNetworkReply *pReply = manager->get(request);
    //设置事件循环，等待资源下载完毕
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished,
                     &eventLoop, &QEventLoop::quit);
    eventLoop.exec();

    //检测http请求的状态码
    int nHttpCode = pReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http返回码
    qDebug()<<"搜索："<<songName<<nHttpCode;

    QByteArray bytes = pReply->readAll();
    QString Qresult=bytes;
    //qDebug()<<Qresult;
    QJsonParseError error;
    QJsonDocument jsonResult =QJsonDocument::fromJson(Qresult.toUtf8(),&error);


    if (error.error == QJsonParseError::NoError) // 解析未发生错误
    {
        if(jsonResult.isObject())
        {
            qDebug()<<"toJson SUCCESS";
            QJsonObject temp =jsonResult.object().value("result").toObject();
            QJsonArray songsArray =temp["songs"].toArray();
            qDebug()<<songsArray;
            int count =songsArray.count();
            if (count<=0)
            {
                return 0;
            }
            songInfos =new SongInfo[count];//创建SongInfo数组存储列表信息

            for(int i=0;i<count;i++)
            {
                QJsonObject tempObject =songsArray[i].toObject();
                songInfos[i].title=tempObject.value("name").toString();                         //标题
                songInfos[i].song_id=QString::number(tempObject.value("id").toInt());   //ID
                songInfos[i].artist=tempObject.value("artists").toArray()[0]                    //艺术家
                        .toObject().value("name").toString();
                songInfos[i].album=tempObject.value("album").toObject().value("name").toString();//专辑名
                songInfos[i].pic_url=tempObject.value("album").toObject().value("picUrl").toString();//专辑封面url
            }
            return 1;
        }
    }
    return 0;
}

void SearchData::searchLocal(QString songName)
{

}

