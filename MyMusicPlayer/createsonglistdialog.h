#ifndef CREATESONGLISTDIALOG_H
#define CREATESONGLISTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QFrame>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>

class CreateSongListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSongListDialog(QWidget *parent = nullptr);

    QLineEdit *getLineEdit() const;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:
    void sendListName(QString);

private slots:
    void on_createButtonClicked();
    void on_cancelButtonClicked();

private:
    QLabel *label;
    QFrame *line;
    QLineEdit *lineEdit;
    QPushButton *createButton;
    QPushButton *cancelButton;

    QPoint mouseStartPoint; //鼠标的初始位置
    QPoint windowsStartPoint; //窗口的初始位
};

#endif // CREATESONGLISTDIALOG_H
