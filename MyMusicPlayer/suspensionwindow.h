//这是切换窗口后的小悬浮窗
#ifndef SUSPENSIONWINDOW_H
#define SUSPENSIONWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QAction>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QContextMenuEvent>
#include <QMenu>

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

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);

signals:

public slots:
    void on_showListBtn_clicked(bool checked); //显示播放列表按钮按下时触发

private:
    QLabel *coverLabel; //展示封面的标签
    QLabel *lyricsLabel; //展示歌词的标签
    QPushButton *likeBtn; //我喜欢按钮
    QPushButton *showListsBtn; //显示播放列表
    QPushButton *resizeBtn; //恢复大窗口
    QMenu *rightClickedMenu; //点击右键出现的窗口
    QMenu *showListsMenu; //点击显示播放列表出现的窗口

    QPixmap coverImage; //专辑封面
    QString songsInfo; //歌曲信息

    //实现窗口可拖动
    QPoint mouseStartPoint; //鼠标的初始位置
    QPoint windowsStartPoint; //窗口的初始位
};

#endif // SUSPENSIONWINDOW_H
