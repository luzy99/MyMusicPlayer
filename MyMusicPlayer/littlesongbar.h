//这是歌单界面出现在小角上用于展示歌曲信息的界面
#ifndef QLITTLESONGBAR_H
#define QLITTLESONGBAR_H

#include <QWidget>
#include <QLabel>
#include <QEvent>
#include <QMouseEvent>
#include <QGraphicsOpacityEffect>
#include "songinfo.h"

class LittleSongBar : public QWidget
{
    Q_OBJECT
public:
    explicit LittleSongBar(QWidget *parent = nullptr);
    void changeSong(SongInfo &info);

protected:
    bool eventFilter(QObject *obj, QEvent *event);

signals:
    void changePage(); //发出换页信号

public slots:

private:
    QLabel *coverLabel; //显示封面的Label
    QLabel *titleLabel; //显示标题的Label
    QLabel *authorLabel; //显示作者的Label
    QPixmap coverImage; //歌曲封面
    QGraphicsOpacityEffect *opacityEffect; //设置图片透明度相关
};

#endif // QLITTLESONGBAR_H
