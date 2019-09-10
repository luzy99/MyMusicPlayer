//这是切换窗口后的小悬浮窗
#ifndef SUSPENSIONWINDOW_H
#define SUSPENSIONWINDOW_H

#include <QWidget>
#include <QDesktopWidget>
#include <QEvent>
#include <QMouseEvent>
#include <QAction>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>
#include <QPropertyAnimation>
#include <QStackedLayout>
#include "songinfo.h"

//定义窗体状态的枚举类型
enum WindowsStatus {Normal, LeftOut, RightOut};

class SuspensionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SuspensionWindow(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化信号与槽
    void initActions(QMenu *menu); //初始化菜单的Actions

    void setCoverImage(const QPixmap &value); //封面的set函数
    void setSongsInfo(const QString &value); //歌曲信息的set函数
    void setShowListsMenu(QMenu *value); //播放列表的set函数

    void moveOut(); //移出
    void moveIn(); //移入

    void changeSong(SongInfo &info);

    QPushButton *previousBtn; //上一首的按钮
    QPushButton *playBtn; //播放按钮
    QPushButton *nextBtn; //下一首按钮

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void hideWindow(); //恢复小窗

public slots:
    void onBecomePlaying(); //表示接下来播放
    void onBecomePausing(); //表示接下来暂停

private slots:
    void on_miniBtn_clicked();
    void on_closeBtn_clicked();

private:
    QLabel *coverLabel; //展示封面的标签
    QLabel *lyricsLabel; //展示歌词的标签
    QStackedLayout *stackedLayout;
    QWidget *buttonWidget; //按钮组
    QPushButton *miniBtn; //最小化
    QPushButton *resizeBtn; //恢复大窗口
    QPushButton *closeBtn; //关闭
    QMenu *rightClickedMenu; //点击右键出现的窗口
    QMenu *showListsMenu; //点击显示播放列表出现的窗口

    QPixmap coverImage; //专辑封面
    QString songsInfo; //歌曲信息

    //实现窗口可拖动
    QPoint mouseStartPoint; //鼠标的初始位置
    QPoint windowsStartPoint; //窗口的初始位

    //实现窗口的停靠效果
    WindowsStatus status; //表示当前窗体坐标的信息
    int screenWidth;  //屏幕宽度
    int screenHeight; //屏幕高度
    QPushButton *suspensionBtn; //悬浮窗收起时的小图标
};

#endif // SUSPENSIONWINDOW_H
