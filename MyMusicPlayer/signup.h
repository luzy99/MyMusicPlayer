#include <QObject>
#include<QWidget>
#include<QPushButton>
#include<QLineEdit>
#include<QLabel>
#include<QDialog>
#include<QSqlDatabase>
#include<QPalette>

class signup:public QDialog
{
    Q_OBJECT
public:
    signup(QWidget *parent=nullptr);
    ~signup();
private slots:
    void on_return_clicked();
    void on_cancel_clicked();
    void initWindows();
    void on_imageBtn_clicked();

signals:
    void returnValues(QString id,QString pwd);


private:
    QSqlDatabase db;

    QLabel *welcome_lb;
    QPushButton *okBtn;
    QPushButton*cancelBtn;
    QPushButton*imageBtn;
    QLabel *less_pwd;//密码没有6位
    QLabel *errorPwd_lb;
    QLabel *emptyName_lb;//用户昵称不能为空
    QLineEdit *nameLineEdit;
    QLineEdit *pwd1LineEdit;
    QLineEdit *pwd2LineEdit;
    QString strId;
    QString m_id;
    QString m_name;
    QString m_pwd;
    QString userImagedir;
    QPalette m_pal;
    QPalette pl;

};


