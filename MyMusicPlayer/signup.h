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
    QPixmap PixmapToRound(const QPixmap &src, int radius);
    ~signup();

private slots:
    void on_return_clicked();
    void on_cancel_clicked();
    void initWindows();
    void on_imageBtn_clicked();

signals:
    void returnValues(QString id,QString pwd);

private:
    QLabel *welcome_lb;
    QPushButton *okBtn;
    QPushButton*cancelBtn;
    QPushButton*imageBtn;
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


