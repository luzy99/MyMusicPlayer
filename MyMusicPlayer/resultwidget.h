#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include "songinfo.h"

class ResultWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ResultWidget(QWidget *parent = nullptr);

    //获取在线图片
    void getOnlinePic(QString);

    void addsonginfoItems();

signals:

public slots:
    void on_replyFinished(QNetworkReply *reply);
    void on_searchReply(QMap<QString,SongInfo> searchResult);
private:
    QMap<QString,SongInfo> m_searchResult;
    QLabel *m_tipLabel;
    QPushButton *m_btnClose;
    QListWidget *m_resultWidget;
    QWidget *m_songInfoWidget;
    QLabel *m_titleLabel;
    QLabel *m_artistLabel;
    QLabel *m_pictureLabel;
    QPixmap *m_songPicture;
};

#endif // RESULTWIDGET_H
