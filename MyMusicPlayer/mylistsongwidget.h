#ifndef MYLISTSONGWIDGET_H
#define MYLISTSONGWIDGET_H

#include <QWidget>
#include <QListWidget>
class MyListSongWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListSongWidget(QWidget *parent = nullptr);

    void contextMenuEvent(QContextMenuEvent *event);

    void setCurrentSongLikedOrNot(bool value);

    void setTempItem(const QListWidgetItem &value);

signals:
    void sendLikeCommand(QString);

    void sendDislikeCommand(QString);

    void sendRemoveCommand(QString);

    void sendAddIntoSongListCommand(QString,QString);

    void sendShowSongListInfoCommand(QString);


public slots:
    void removeAction_slot();
    void likeAction_slot();
    void dislikeAction_slot();
    void addIntoSongListAction_slot();
    void showSongInfoAction_slot();


private:
    QListWidgetItem tempItem;
    bool currentSongLiked;
};

#endif // MYLISTSONGWIDGET_H
