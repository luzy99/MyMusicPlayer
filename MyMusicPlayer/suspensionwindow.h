//这是切换窗口后的小悬浮窗
#ifndef SUSPENSIONWINDOW_H
#define SUSPENSIONWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>

class SuspensionWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SuspensionWindow(QWidget *parent = nullptr);

    void setCoverImage(const QPixmap &value); //封面的set函数
    void setSongsInfo(const QString &value); //歌曲信息的set函数

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

signals:

public slots:

private:
    QLabel *coverLabel; //展示封面的标签
    QLabel *lyricsLabel; //展示歌词的标签
    QPushButton *likeBtn; //我喜欢按钮
    QPushButton *showListsBtn; //显示播放列表
    QPushButton *resizeBtn; //恢复大窗口

    QPixmap coverImage; //专辑封面
    QString songsInfo; //歌曲信息
    //实现窗口可拖动
    QPoint mouseStartPoint; //鼠标的初始位置
    QPoint windowsStartPoint; //窗口的初始位置
};

#endif // SUSPENSIONWINDOW_H
