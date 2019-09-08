#include <QObject>
#include <QWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>
#include <QSqlDatabase>
#include <QMouseEvent>
#include <QPalette>

class signup:public QDialog
{
    Q_OBJECT
public:
    signup(QWidget *parent=nullptr);
    ~signup();

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void on_return_clicked();
    void on_cancel_clicked();
    void initWindows();
    void on_imageBtn_clicked();

signals:
    void returnValues(QString id,QString pwd);

private:
    QLabel *welcome_lb;
    QLabel *info_lb;
    QPushButton *okBtn;
    QPushButton*cancelBtn;
    QPushButton*imageBtn;
    QLineEdit *nameLineEdit;
    QLineEdit *pwd1LineEdit;
    QLineEdit *pwd2LineEdit;
    QWidget *welWidget;
    QWidget *tipWidget;
    QWidget *infoWidget;
    QString strId;
    QString m_id;
    QString m_name;
    QString m_pwd;
    QString userImagedir;
    QPalette pl;

    QPoint mouseStartPoint;
    QPoint windowsStartPoint;
};


