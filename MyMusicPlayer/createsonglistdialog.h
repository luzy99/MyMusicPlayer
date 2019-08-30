#ifndef CREATESONGLISTDIALOG_H
#define CREATESONGLISTDIALOG_H

#include <QDialog>
#include<QLabel>
#include<QFrame>
#include<QLineEdit>
#include<QPushButton>


namespace Ui {
class CreateSongListDialog;
}

class CreateSongListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CreateSongListDialog(QWidget *parent = nullptr);
    ~CreateSongListDialog();

    QLineEdit *getLineEdit() const;

signals:
    void sendListName(QString);


private slots:
    void on_createButtonClicked();

    void on_cancelButtonClicked();

private:
    Ui::CreateSongListDialog *ui;
    QLabel *label;
    QFrame *line;
    QLineEdit *lineEdit;
    QPushButton *createButton;
    QPushButton *cancelButton;
};

#endif // CREATESONGLISTDIALOG_H
