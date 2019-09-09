//这是在线搜索的弹出框
#ifndef RESULTWIDGET_H
#define RESULTWIDGET_H

#include <QWidget>
#include <QMap>
#include <QList>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>
#include <QMouseEvent>
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

    void addsonginfoItems(int cur);
    void addPicture(QString pid);
    QWidget *addNewItemWidget(int cur);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void resendSongInfo(SongInfo &);
    void resendnvId(QString);

public slots:
    void on_replyFinished(QNetworkReply *reply);
    void on_searchReply(QMap<QString,SongInfo> searchResult);
    void on_searchReply1(QMap<QString,QMap<QString,QString>> mvResults);
    void on_itemclicked(int cur);
    void closeEvent(QCloseEvent *event);//关闭

private:
    QMap<QString,SongInfo> m_searchResult;
    QMap<QString,QMap<QString,QString>> m_mvResults;
    int m_mode;
    QLabel *m_tipLabel;
    QPushButton *m_btnClose;
    QListWidget *m_resultWidget;
    QPixmap *m_songPicture;
    int curindex;
    QMap<int,QString> picMap;

    QPoint mouseStartPoint;
    QPoint windowsStartPoint;
};

#endif // RESULTWIDGET_H
