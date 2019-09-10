#include "searchdata.h"
#include <QSqlQuery>

SearchData::SearchData(QObject *parent) : QObject(parent)
{
    searchResults.clear();
    mvResults.clear();
    localResults.clear();
}

bool SearchData::searchSongsOnline(QString songName)
{
    searchResults.clear();
    QString url ="http://music.163.com/api/search/pc/?s="
            +songName
            +"&limit=20&type=1&offset=0";
    //构造请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);

    //构造网络管理
    QNetworkAccessManager* manager=new QNetworkAccessManager;
    // 发送请求
    QNetworkReply *pReply = manager->get(request);
    //设置事件循环，等待资源下载完毕
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
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
            //qDebug()<<songsArray;
            int count =songsArray.count();
            for(int i=0;i<count;i++)
            {
                SongInfo songinfo;
                QJsonObject tempObject =songsArray[i].toObject();
                songinfo.title=tempObject.value("name").toString();                         //标题
                songinfo.song_id=QString::number(tempObject.value("id").toInt());   //ID
                songinfo.artist=tempObject.value("artists").toArray()[0]                    //艺术家
                        .toObject().value("name").toString();
                songinfo.album=tempObject.value("album").toObject().value("name").toString();//专辑名
                songinfo.pic_url=tempObject.value("album").toObject().value("picUrl").toString();//专辑封面url
                searchResults.insert(songinfo.song_id,songinfo);//加入map
            }

            emit searchFinished(searchResults);
            return 1;
        }
    }
    qDebug()<<"searchinfo ERROR";
    return 0;
}

void SearchData::searchLocal(QString songName)
{
    localResults.clear();

    //与数据库建立联系
    QSqlQuery query;
    QString cmd = "show tables;";
    QSqlQuery query1(cmd);
    while (query1.next())
    {
        QString tableName = QString(query1.value(0).toString());
        if(tableName == QString("%1_播放历史").arg(m_userid))
        {
            continue;
        }
        if(tableName.section("_",0,0)!=m_userid)
        {
            continue;
        }

        QString sql=QString("select songName,songUrl,artist,cover_image from %1").arg(tableName);
        query.exec(sql);
        while(query.next())
        {
            QString songname=query.value(0).toString();
            QString songUrl=query.value(1).toString();
            QString artist=query.value(2).toString();
            QString cover_image=query.value(3).toString();

            if(songName==songname||songName==artist)
            {
                QString resultinfo= songname+":"+artist+":"+cover_image;
                QString songUrl2;
                //查重,防止显示重复的搜索结果
                if(songUrl.at(0).isLower())
                {
                    songUrl2 = songUrl.at(0).toUpper() + songUrl.mid(1);
                }
                else
                {
                    songUrl2 = songUrl.at(0).toLower() + songUrl.mid(1);
                }

                if(!localResults.contains(songUrl) && !localResults.contains(songUrl2))
                {
                    localResults.insert(songUrl,resultinfo);
                }
            }
        }
    }
    emit searchLocalFinished(localResults);
}

bool SearchData::searchMv(QString songName)
{
    mvResults.clear();
    QString url ="http://music.163.com/api/search/pc/?s="
            +songName
            +"&limit=20&type=1004&offset=0";
    //构造请求
    QNetworkRequest request;
    request.setUrl(QUrl(url));
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute,true);

    //构造网络管理
    QNetworkAccessManager* manager=new QNetworkAccessManager;
    // 发送请求
    QNetworkReply *pReply = manager->get(request);
    //设置事件循环，等待资源下载完毕
    QEventLoop eventLoop;
    QObject::connect(manager, &QNetworkAccessManager::finished, &eventLoop, &QEventLoop::quit);
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
            QJsonArray songsArray =temp["mvs"].toArray();
            //qDebug()<<songsArray;
            int count =songsArray.count();
            QMap<QString,QString> mvInfo;

            for(int i=0;i<count;i++)
            {
                QJsonObject tempObject =songsArray[i].toObject();
                QString id=QString::number(tempObject.value("id").toInt());//ID

                mvInfo.insert("mv_id",id);                                                      //ID
                mvInfo.insert("title",tempObject.value("name").toString());                //标题
                mvInfo.insert("artist",tempObject.value("artistName").toString()); //歌手
                mvInfo.insert("pic_url",tempObject.value("cover").toString());//封面图片url
                mvResults.insert(id,mvInfo);//加入map
            }
            qDebug()<<"mvResults"<<mvResults.keys();
            qDebug()<<mvResults.values();
            emit searchMvFinished(mvResults);
            return 1;
        }
    }
    qDebug()<<"searchMv ERROR";
    return 0;
}

void SearchData::setUserid(const QString &userid)
{
    m_userid = userid;
}

