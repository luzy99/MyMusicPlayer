#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include <QObject>
#include<QFile>
#include<QMap>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include<QEventLoop>
#include"songinfo.h"

class SearchData : public QObject
{
    Q_OBJECT
public:
    explicit SearchData(QObject *parent = nullptr);

signals:
    void searchFinished(QMap<QString,SongInfo> searchResults);
    void searchMvFinished(QMap<QString,QMap<QString,QString>> mvResults);
public slots:
    bool searchSongsOnline(QString songName);
    void searchLocal(QString songName);
    bool searchMv(QString songName);

private:
    QMap<QString,SongInfo> searchResults;//<id,Songinfo结构体>
    QMap<QString,QMap<QString,QString>> mvResults;
    /**QMap结构
     * <id,<"mv_id",...>>
     *      <"title",...>
     *      <"pic_url",...>
     *      <"artist",...>
     * */
};

#endif // SEARCHDATA_H
