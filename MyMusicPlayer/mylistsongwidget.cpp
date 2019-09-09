#include "mylistsongwidget.h"
#include "addintosonglist.h"
#include <QMenu>
#include<QDebug>

MyListSongWidget::MyListSongWidget(QWidget *parent) : QListWidget(parent)
{
    this->setStyleSheet("QListWidget{color:rgb(92,92,92); background:rgb(245,245,247);border:0px solid gray;}" \
                        "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}" \
                        "QListWidget::Item:hover{color:rgb(55,57,61);background:rgb(255,232,233);border:0px solid gray;}" \
                        "QListWidget::Item:selected{color:rgb(0,0,0);background:rgb(230,231,234);border:none;}" \
                        "QListWidget:focus{outline:none;}");
}

void MyListSongWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    if(this->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
    {
        QMenu* popMenu = new QMenu(this);
        QAction* removeAction = new QAction(QIcon(":/icon/res/delete.png"),
                                            "移除歌曲",this);
        QAction* likeAction = new QAction(QIcon(":/icon/res/heart.ico"),
                                          "设为我喜欢",this);
        QAction* dislikeAction = new QAction(QIcon(":/icon/res/favourite_d.ico"),
                                             "取消我喜欢", this);
        QAction* addIntoSongListAction = new QAction(QIcon(":/icon/res/addtolist.png"),
                                                     "添加到其他歌单",this);
        QAction* showSongInfoAction = new QAction(QIcon(":/icon/res/songInfo.ico"),
                                                  "曲目信息", this);
        if(currentSongLiked)
        {
            popMenu->addAction(dislikeAction);
        }
        else
        {
            popMenu->addAction(likeAction);
        }
        popMenu->addAction(removeAction);
        popMenu->addAction(addIntoSongListAction);
        popMenu->addAction(showSongInfoAction);

        popMenu->setStyleSheet(" QMenu {"
                        "background: white; "
                        "border-radius:2px;"
                        "border:1px solid rgb(200,200,200);"
                        "}"
                        "QMenu::item {"
                        " background-color: transparent;"
                        " padding:5px 30px;"
                        " margin:0px 0px;"
                        " border-bottom:0px solid #DBDBDB;"
                        "}"
                        " QMenu::item:selected { "
                        "background-color: rgb(200,200,200);"
                        "}");

        connect(removeAction ,SIGNAL(triggered()) ,
                this, SLOT(removeAction_slot()));
        connect(likeAction ,SIGNAL(triggered()) ,
                this, SLOT(likeAction_slot()));
        connect(dislikeAction ,SIGNAL(triggered()) ,
                this, SLOT(dislikeAction_slot()));
        connect(addIntoSongListAction ,SIGNAL(triggered()) ,
                this, SLOT(addIntoSongListAction_slot()));
        connect(showSongInfoAction ,SIGNAL(triggered()) ,
                this, SLOT(showSongInfoAction_slot()));

        popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
    }
}

void MyListSongWidget::setCurrentSongLikedOrNot(bool value)
{
    currentSongLiked = value;
}

void MyListSongWidget::setTempItem(const QListWidgetItem &value)
{
    tempItem = value;
}

void MyListSongWidget::setUser(const QString &value)
{
       User = value;
}

void MyListSongWidget::removeAction_slot()
{
    emit(sendRemoveCommand(tempItem.text()));
}

void MyListSongWidget::likeAction_slot()
{
    emit(sendLikeCommand(tempItem.text()));
}

void MyListSongWidget::dislikeAction_slot()
{
    emit(sendDislikeCommand(tempItem.text()));
}

void MyListSongWidget::addIntoSongListAction_slot()
{
    QString listName;
    AddIntoSongList window(User);
    int status = window.exec();
    qDebug()<<status;
    if(status)
    {
        listName = window.getAddedSongList();
        qDebug()<< listName;
        emit(sendAddIntoSongListCommand(tempItem.text(), listName));
    }
}

void MyListSongWidget::showSongInfoAction_slot()
{
    emit(sendShowSongListInfoCommand(tempItem.text()));
}


