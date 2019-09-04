#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QMainWindow>
#include <QPropertyAnimation>

class ErrorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ErrorWindow(QString msg ,QWidget *parent = nullptr);
    ~ErrorWindow();

    void showInstantly();

private:
    QString message;
    QPropertyAnimation *animation;
};

#endif // ERRORWINDOW_H
