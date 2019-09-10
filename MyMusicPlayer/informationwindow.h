#ifndef INFORMATIONWINDOW_H
#define INFORMATIONWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

class InformationWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit InformationWindow(QString msg ,QWidget *parent = nullptr);
    ~InformationWindow();

    void showInstantly();

private:
    QString message;
    QPropertyAnimation *animation;
};

#endif // INFORMATIONWINDOW_H
