#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QMenu>
#include "resultwidget.h"
#include "searchlocal.h"
#include "login.h"

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化信号与槽
    void initActions(); //初始化菜单
    ~TitleBar();

    ResultWidget *searchResult; //全网搜索的结果栏
    SearchLocal *searchLocalResult; //在线搜索的结果栏
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void showSuspensionWindow(); //表示小窗化
    void minimizeWindow(); //表示最小化主窗口
    void maximizeWindow(); //表示最大化主窗口
    void closeWindow(); //表示关闭窗口

    void beginSearchOnline(QString searchContents); //点击了全网搜索
    void beginSearchLocal(QString searchContents); //点击了本地搜索
    void beginSearchMv(QString searchContents); //点击了搜索MV

    void beginGesture(); //开启手势识别
    void closeGesture(); //关闭手势识别

public slots:

private slots:
    void onEditFinished();
    void on_actSearchOnline_triggered();
    void on_actSearchLocal_triggered();
    void on_actSearchMV_triggered();

    void on_gestureBtn_clicked();
    void on_userBtn_clicked();

private:
    QLabel *iconLabel; //标题栏图标
    QLabel *titleLabel; //标题栏标题
    QLineEdit *searchBar; //搜索栏
    QMenu *searchMenu; //搜索菜单
    QPushButton *searchBtn; //搜索按钮
    QLabel *spacingLabel; //用来制造空格的组件
    QPushButton *userBtn; //用户登录的按钮
    QPushButton *skinBtn; //换肤按钮
    QPushButton *gestureBtn; //手势按钮
    QPushButton *settingsBtn; //设置按钮
    QPushButton *resizeBtn; //小窗化按钮
    QPushButton *minimizeBtn; //最小化按钮
    QPushButton *maximizeBtn; //最大化/还原按钮
    QPushButton *closeBtn; //关闭按钮

    bool gestureControl; //是否手势控制

    UserLogin *loginForm;
};

#endif // TITLEBAR_H
