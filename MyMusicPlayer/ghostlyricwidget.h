#pragma once

#include <QWidget>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QOpenGLWidget>
#include"lyricwidget.h"


class GhostLyricWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GhostLyricWidget(QWidget *parent = nullptr);
    ~GhostLyricWidget();
    //根据第几句获得歌词
    QString GetLrcByIndex(int index);

    //根据第几句获得开始的时间
    qint64 GetPosByindex(int index);

    //根据时间获得这一句的歌词
    QString GetLrcByTime(qint64 time);

    //通过时间确定播放到了第几句（二分法查找）
    int GetIndexByTime(qint64 time);

    //获取精确到每一个字的播放信息
    void GetMaskLen(int nFontSize);

    //解析获取的文件，给变量进行初始化
    void analyzeLrcContent(const QString &);

    void clearLrc();

    //返回的正在播放的那个字的信息（这个字在总字数的百分比）
    void getPosInfo(int &keyTime,int &interval, float &precent, QString &str);

    void setOriginalStatus();

protected:
    virtual void paintEvent(QPaintEvent*event);

private slots:
    void slot_timer();
public slots:
    void positionChanged(qint64 length);

private:


    void DrawItem(QPainter&Painter,ItemInfo &index);

    QMap<qint64,QString> m_lineMap;//键是开始的时间，值是那一句歌词（由所有的字组成）就是word
    QList<QMap<int,QString>> m_word_list;
    QList<QMap<int ,int>> m_interval_list; //interval,间隔时间
//

    QString m_strCurLrc;//正在播放的那一句的歌词
    QTimer m_timer;//计时器用的时候应该是看这个计时器，刷新的很快（200毫秒刷新一次）
    QFont m_normalFont;//正常情况下的字体
    qint64 m_nCurPos;//正在播放的那一个字的位置
    qint64 m_nCurStartPos;//这一句歌词第一个字开始的位置
    float m_nMaskLen;//就是那个K歌功能，精确到每一个字的滚
    int m_nCurIndex;//正在播放的那一个字的位置
    int m_nSroIndex;//没看懂好像是前一行，这个和上面那个一行接一行滚的意思
    int m_nOffset;//偏移量，这个也奇怪感觉没啥用
    int m_nFontPixSize;//奇怪的东西后面注释了
    QColor m_HLColor;   //hightlight
    QColor m_NlColor;   //normal
};


