#ifndef RENAMESONGLISTDIALOG_H
#define RENAMESONGLISTDIALOG_H

#include <QDialog>
#include <QLabel>
#include<QFrame>
#include<QLineEdit>
#include<QPushButton>

namespace Ui {
class RenameSongListDialog;
}

class RenameSongListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RenameSongListDialog(QWidget *parent = nullptr);
    ~RenameSongListDialog();

    QLineEdit *getLineEdit() const;

private slots:
    void on_renameButtonClicked();

    void on_cancelButtonClicked();

private:
    Ui::RenameSongListDialog *ui;
    QLabel *label;
    QFrame *line;
    QLineEdit *lineEdit;
    QPushButton *renameButton;
    QPushButton *cancelButton;
};

#endif // RENAMESONGLISTDIALOG_H
