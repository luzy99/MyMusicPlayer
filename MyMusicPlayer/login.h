#ifndef LOGIN_H
#define LOGIN_H

#include <QObject>
#include <QWidget>
#include <QMovie>
#include <QPainter>
#include <QHBoxLayout>
#include <QCursor>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QLayout>
#include <QMovie>
#include <QComboBox>
#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QEvent>
#include <QMouseEvent>
#include <QTimer>

class UserLogin :public QDialog
{
    Q_OBJECT
public:
    UserLogin(QWidget *parent=nullptr);
    ~UserLogin();

    QLineEdit *id() const;

signals:
    void loginSuccess(QString userId);
    void clearMusic();

public slots:
    void onClicked();
    void on_eye_clicked();

    void on_signup_clicked();
    void on_login_clicked();
    void on_editingFinished();
    void passUserInfo(QString userId);
    void displayReturnValues(QString id,QString pwd);
    void on_timeout();

protected:
    bool eventFilter(QObject *obj,QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    bool isShown;
    QPushButton *loginBtn;
    QPushButton *signUpBtn;
    QWidget *m_infoWidget;
    QLineEdit *m_id;
    QLineEdit *m_pwd;
    QWidget *m_ptitleBar;
    QLabel *m_addPic;
    QLabel *m_backgif;
    QLabel *m_pwdIcon;
    QPushButton *show_pwd;//显示密码的小眼睛
    QLabel *pic;//显示用户图像
    QPushButton *minimizeBtn;
    QPushButton *closeBtn;
    QLabel *icon_lb;
    QLabel *title_lb;
    QTimer *m_timer;
    QComboBox *accounts;

    QPoint mouseStartPoint;
    QPoint windowsStartPoint;
};

#endif

