#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H

#include <QWidget>
#include <QDir>
#include <QTimer>
#include <QMap>
#include <QDebug>
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QLabel>
#include <QPushButton>
#include <QOpenGLWidget>

struct ItemInfo
{
    int index;//第几句
    int alpha;//这个有点迷，歌词透明度？
    int Y;//纵坐标吧
    bool hightLight;//这一句是否高亮
    QString strText;//这一句歌词的文本
};

class LyricWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LyricWidget(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化信号与槽
    ~LyricWidget();
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

    //get函数,用于给minilyrics组件赋值,防止重复爬取
    QMap<qint64, QString> getLineMap() const;
    QList<QMap<int, QString> > getWord_list() const;
    QList<QMap<int, int> > getInterval_list() const;

protected:
    //虚函数功能重定义
    virtual void paintEvent(QPaintEvent*event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

signals:
    void positionChanged(qint64 position);
    void positionDraggedTo(qint64 position);
    void blockSignals(bool block);

public slots:
    void on_btnPlaythis_clicked();
    void on_positionStop(qint64 stoptime);

private slots:
    void slot_timer();
    void onPositionChanged(qint64 length);

private:
    void DrawItem(QPainter&Painter,ItemInfo &index);

    QMap<qint64,QString> m_lineMap;//键是开始的时间，值是那一句歌词（由所有的字组成）就是word
    QList<QMap<int,QString>> m_word_list;
    QList<QMap<int ,int>> m_interval_list; //interval,间隔时间

    //正在播放的那一句的歌词
    QString m_strCurLrc;
    //计时器用的时候应该是看这个计时器，刷新的很快（200毫秒刷新一次）
    QTimer m_timer;
    //正常情况下的字体
    QFont m_normalFont;
    //正在播放的那一个字的位置
    qint64 m_nCurPos;
    //这一句歌词第一个字开始的位置
    qint64 m_nCurStartPos;
    //信号阻塞时最后获得position位置
    qint64 m_nStoptime;
    //就是那个K歌功能，精确到每一个字的滚
    float m_nMaskLen;
    //正在播放的那一个字的位置
    int m_nCurIndex;
    //没看懂好像是前一行，这个和上面那个一行接一行滚的意思
    int m_nSroIndex;
    //偏移量，这个也奇怪感觉没啥用
    int m_nOffset;
    //奇怪的东西后面注释了
    int m_nFontPixSize;
    //hightlight
    QColor m_HLColor;
    //normal
    QColor m_NlColor;
    QPushButton* btnPlaythis;
    QLabel* lab_showtime;

    QPoint m_mouseStartPoint; //鼠标的初始位置
};

#endif // LYRICWIDGET_H
