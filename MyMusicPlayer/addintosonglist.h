#ifndef ADDINTOSONGLIST_H
#define ADDINTOSONGLIST_H

#include <QDialog>
#include <QListWidget>
#include <QGroupBox>
#include <QScrollArea>
#include <QLabel>
#include <QPushButton>
#include <QSqlDatabase>

class AddIntoSongList : public QDialog
{
    Q_OBJECT
public:
    explicit AddIntoSongList(QWidget *parent = nullptr);

    QString getAddedSongList() const;

public slots:
    void closeWindow();
    void onItemClicked(QListWidgetItem *);

private:
    QSqlDatabase db;
//    QScrollArea *scrollLists;
//    QWidget *scrollListsWidget;
    QListWidget *listList;
    QPushButton *closeButton;
    QLabel *title;
    QLabel *item_titleLabel;
    QLabel *item_numLabel;
    QLabel *item_songPicture;
    QWidget *listInfoWidget;
    QString addedSongList;
};

#endif // ADDINTOSONGLIST_H
