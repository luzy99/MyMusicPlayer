#ifndef RENAMESONGLISTDIALOG_H
#define RENAMESONGLISTDIALOG_H

#include <QDialog>
#include <QLabel>
#include<QFrame>
#include<QLineEdit>
#include<QPushButton>

class RenameSongListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameSongListDialog(QWidget *parent = nullptr);

    QLineEdit *getLineEdit() const;

private slots:
    void on_renameButtonClicked();

    void on_cancelButtonClicked();

private:
    QLabel *label;
    QFrame *line;
    QLineEdit *lineEdit;
    QPushButton *renameButton;
    QPushButton *cancelButton;
};

#endif // RENAMESONGLISTDIALOG_H
