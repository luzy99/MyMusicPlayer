#pragma once

#include <QObject>
#include <QWidget>
#include<QMovie>
#include<QPainter>
#include<QHBoxLayout>
#include<QCursor>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QLayout>
#include<QMovie>
#include<QComboBox>
#include<QDialog>
#include<QSqlDatabase>
#include<QSqlQuery>

class UserLogin :public QDialog
{
    Q_OBJECT
public:
    UserLogin(QWidget *parent=nullptr);
    ~UserLogin();

    QLineEdit *id() const;

    void initWindow();
    void initSlotsandSignals();
    void initTitleBar();

signals:
    void loginSuccess(QString userId,QString userName);

public slots:
    void onClicked();
    void on_eye_clicked();

    void on_signup_clicked();
    void on_login_clicked();
    void on_editingFinished();
    void displayReturnValues(QString id,QString pwd);

private:
    bool isShown;
    QPushButton *loginBtn;
    QPushButton *signUpBtn;
    QLineEdit *m_id;
    QLineEdit *m_pwd;
    QPushButton *show_pwd;//显示密码的小眼睛
    QLabel *pic;//显示用户图像
    QLabel *errorId;//提示用户名不存在
    QLabel *errorPwd;//提示密码错误
    QPushButton *minimizeBtn;
    QPushButton *closeBtn;
    QLabel *icon_lb;
    QLabel *title_lb;

    QHBoxLayout *titleQhb;

    QComboBox *accounts;
};


