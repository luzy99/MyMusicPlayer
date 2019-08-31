#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include <QObject>
#include<QFile>
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

public slots:
    bool searchOnline(QString songName,SongInfo *&songInfos);
    void searchLocal(QString songName);
private:

};

#endif // SEARCHDATA_H
