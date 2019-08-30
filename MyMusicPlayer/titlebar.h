#ifndef TITLEBAR_H
#define TITLEBAR_H

#include <QLabel>
#include <QPushButton>
#include<QGroupBox>
class titleBar : public QWidget
{
    Q_OBJECT

public:
    explicit titleBar(QWidget *parent = nullptr);
    ~titleBar();

protected:

    //双击标题栏进行界面的最大化/还原
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    //进行界面的拖动
    virtual void mousePressEvent(QMouseEvent *event);

    //设置界面标题与图标
    virtual bool eventFilter(QObject *obj, QEvent *event);

private slots:

    //进行最小化、最大化/还原、关闭操作
    void onClicked();

private:

    //最大化/还原
    void updateMaximize();

private:
    QLabel *m_pIconLabel; //标题栏图标
    QLabel *m_pTitleLabel; //标题栏标题
    QPushButton *m_pMinimizeButton; //最小化按钮
    QPushButton *m_pMaximizeButton; //最大化/还原按钮
    QPushButton *m_pCloseButton; //关闭按钮
    QPushButton *m_pSkinButton; //皮肤按钮
    QPushButton *m_pSettingButton; //设置按钮
    QGroupBox *g_title;
};

#endif // TITLEBAR_H
