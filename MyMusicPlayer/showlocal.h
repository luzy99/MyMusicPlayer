//这是首页下面的歌曲展示
#ifndef SHOWLOCAL_H
#define SHOWLOCAL_H

#include <QObject>
#include <QWidget>
#include <QSqlDatabase>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLayout>
#include <QLabel>
#include <QStringList>

class ShowLocal : public QWidget
{
    Q_OBJECT
public:
    explicit ShowLocal(QWidget *parent = nullptr);

signals:
    void songUrl(QString);

public slots:
    void on_ItemClick1(int);
    void on_ItemClick2(int);
    void onUserLogin(QString userId);

private:
    QSqlDatabase db;
    QTabWidget *m_pLocalShowTab;
    QListWidget *m_pIconShowList;
    QListWidget *m_pListShowList;
    QWidget *m_pIconinfoWidget;
    QWidget *m_pListinfoWidget;
    QPushButton *m_pMvButton;
    QLabel *m_pPicLabel;
    QLabel *m_pTitleLabel;
    QLabel *m_pArtistLabel;
    QLabel *m_pTitleLabelicon;
    QLabel *m_pArtistLabelicon;
    QLabel *m_pLikeLabel;
    QStringList m_SongUrls;
    int index;
    QString m_userid;

};

#endif // SHOWLOCAL_H
