#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QWidget>
#include<QListWidget>
class MyListListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListListWidget(QWidget *parent = nullptr);

    void contextMenuEvent(QContextMenuEvent *event);

    void setTempItem(const QListWidgetItem &value);

signals:
    void sendClearListCommand(QString);
    void sendDeleteListCommand(QString);
    void sendRenameListCommand(QString);

public slots:
    void clearAction_slot();
    void deleteAction_slot();
    void renameAction_slot();

private:
    QListWidgetItem tempItem;
};

#endif // MYLISTWIDGET_H
