//这是搜索本地的弹出框
#ifndef SEARCHLOCAL_H
#define SEARCHLOCAL_H

#include <QWidget>
#include <QSqlDatabase>
#include <QList>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

class SearchLocal : public QWidget
{
    Q_OBJECT
public:
    explicit SearchLocal(QWidget *parent = nullptr);
    void addsonginfoItems(int cur);

signals:
    void resendSongUrl(QString);
public slots:
    void on_searchReply(QMap<QString,QString>);
    void on_itemclicked(int);

private:
    QLabel *m_tipLabel;
    QPushButton *m_btnClose;
    QListWidget *m_resultWidget;
    QWidget *m_songInfoWidget;
    QLabel *m_titleLabel;
    QLabel *m_artistLabel;
    QLabel *m_pictureLabel;
    QPixmap *m_songPicture;
    QMap<QString,QString> m_localResults;
};

#endif // SEARCHLOCAL_H
