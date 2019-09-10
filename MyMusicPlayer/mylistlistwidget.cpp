#include "mylistlistwidget.h"
#include<QMenu>
#include<QDebug>

MyListListWidget::MyListListWidget(QWidget *parent)
    : QListWidget(parent)
{
    this->setStyleSheet("QListWidget{color:rgb(92,92,92); background:rgb(245,245,247);border:0px solid gray;}" \
                        "QListWidget::Item{height:30px;border:0px solid gray;padding-left:15;}" \
                        "QListWidget::Item:hover{color:rgb(55,57,61);background:rgb(255,232,233);border:0px solid gray;}" \
                        "QListWidget::Item:selected{color:rgb(0,0,0);background:rgb(230,231,234);border:none;}" \
                        "QListWidget:focus{outline:none;}");
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MyListListWidget::contextMenuEvent(QContextMenuEvent *event)
{
    Q_UNUSED(event);
    if(this->itemAt(mapFromGlobal(QCursor::pos())) != NULL)
    {
        QMenu* popMenu = new QMenu(this);
        QAction* deleteAction = new QAction(QIcon(":/icon/res/delete.png"),
                                            "删除歌单",this);
        QAction* renameAction = new QAction(QIcon(":/icon/res/rename.png"),
                                            "重命名",this);
        QAction* clearAction = new QAction(QIcon(":/icon/res/clear.png"),
                                           "清空歌单", this);
        popMenu->addAction(clearAction);
        popMenu->addAction(deleteAction);
        popMenu->addAction(renameAction);
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

        connect(clearAction ,SIGNAL(triggered()) ,
                this, SLOT(clearAction_slot()));
        connect(deleteAction ,SIGNAL(triggered()) ,
                this, SLOT(deleteAction_slot()));
        connect(renameAction ,SIGNAL(triggered()) ,
                this, SLOT(renameAction_slot()));
        popMenu->exec(QCursor::pos()); // 菜单出现的位置为当前鼠标的位置
    }
}

void MyListListWidget::clearAction_slot()
{
    qDebug()<<tempItem.text();
    emit(sendClearListCommand(tempItem.text()));
}

void MyListListWidget::deleteAction_slot()
{
    qDebug()<<tempItem.text();
    emit(sendDeleteListCommand(tempItem.text()));
}

void MyListListWidget::renameAction_slot()
{
    qDebug()<<tempItem.text();
    emit(sendRenameListCommand(tempItem.text()));
}

void MyListListWidget::setTempItem(const QListWidgetItem &value)
{
    tempItem = value;
}


