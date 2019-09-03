#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QLineEdit>
#include <QMenu>

class TitleBar : public QWidget
{
    Q_OBJECT

public:
    explicit TitleBar(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化信号与槽
    ~TitleBar();

protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);

signals:
    void showSuspensionWindow(); //表示小窗化
    void minimizeWindow(); //表示最小化主窗口
    void maximizeWindow(); //表示最大化主窗口
    void closeWindow(); //表示关闭窗口

public slots:

private:
    QLabel *iconLabel; //标题栏图标
    QLabel *titleLabel; //标题栏标题
    QPushButton *skinBtn; //换肤按钮
    QPushButton *settingsBtn; //设置按钮
    QPushButton *resizeBtn; //小窗化按钮
    QPushButton *minimizeBtn; //最小化按钮
    QPushButton *maximizeBtn; //最大化/还原按钮
    QPushButton *closeBtn; //关闭按钮
};

#endif // TITLEBAR_H
