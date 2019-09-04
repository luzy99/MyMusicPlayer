#ifndef MINILYRICS_H
#define MINILYRICS_H

#include <QWidget>
#include <QDir>
#include<QTimer>
#include <QPushButton>
#include<QMap>
#include"absframelessautosize.h"
#include "lyricdownload.h"

//这是一个显示桌面滚动歌词的窗口
class miniLyrics : public AbsFrameLessAutoSize
{
    Q_OBJECT

public:
    explicit miniLyrics(QWidget *parent = nullptr);
    void initSignalsAndSlots(); //初始化信号与槽
    ~miniLyrics();
    void initWidgetMISC();//初始化
    void setOriginalStatus();//初始化

    void setCurrentAndNextLrc(const QString &,const QString &);
    void getCurrentLrc();
    QString GetLrcByIndex(int index);//根据歌词的句数找到歌词
    void analyzeLrcContent(const QString &);//分析歌词，并转换成需要的格式，获取相关信息
    qint64 GetPosByindex(int index);//根据句数得到位置
    void clearLrc();//清空歌词
    //void DrawItem(QPainter&Painter);//画歌词
    QString GetLrcByTime(qint64 time);
    void getPosInfo(int &keyTime,int &interval, float &precent, QString &str);//得到歌词的位置信息
    void GetMaskLen();//得到蒙版长度
    int GetIndexByTime(qint64 time);

    //set函数用于与主歌词同步
    //直接用lyricWidget的结果,减少爬取次数提高程序运行效率
    void setLineMap(const QMap<qint64, QString> &lineMap);
    void setWord_list(const QList<QMap<int, QString> > &word_list);
    void setInterval_list(const QList<QMap<int, int> > &interval_list);

    QPushButton *m_closeBtn;
    QPushButton *m_previousBtn;
    QPushButton *m_playBtn;
    QPushButton *m_nextBtn;
    QPushButton*m_forward;
    QPushButton*m_backward;

protected:
    virtual void paintEvent(QPaintEvent *);//调用画歌词的函数
    virtual void resizeEvent(QResizeEvent *);
    virtual void enterEvent(QEvent*);
    virtual void leaveEvent(QEvent *);
    virtual void mouseMoveEvent(QMouseEvent*);

private slots:
    void slot_timer();


public slots:
    void onPositionChanged(qint64 length);//歌曲播放信息变化时发出的信号
    void onBecomePlaying(); //表示接下来播放
    void onBecomePausing(); //表示接下来暂停

private:

    QWidget*m_topWid;
    float m_precent;
    float m_lrcMaskWidth;
    QString m_Text1;
    QString m_Text2;
    bool m_isDrawTop;
    bool m_isDrawBrush;
    bool m_isOriginalState;
    LyricDownload *ly1;
    QLinearGradient m_maskGradient;
    QLinearGradient m_normalGradient;

    float percent;//百分比
    float maskLen;//蒙版长度

    QString leftText;//左上角歌词
    QString rightText;//右下角歌词
    QMap<qint64,QString>m_lineMap;//键是开始的时间，值是那一句歌词（由所有的字组成）就是word
    QList<QMap<int,QString>> m_word_list;//字开始的时间——字
    QList<QMap<int ,int>> m_interval_list; //interval,间隔时间
    QFont m_Font;//字体
    QString CurLrc;//正在播放的那一句的歌词
    QTimer m_timer;//计时器用的时候应该是看这个计时器，刷新的很快（200毫秒刷新一次）
    qint64 CurPos;//正在播放的那一个字的位置
    qint64 CurStartPos;//这一句歌词第一个字开始的位置
    int CurIndex;//正在播放的那一句词的位置
    QColor m_HLColor;   //蒙版高亮颜色
    QColor m_NlColor;   //正常歌词颜色
};

#endif
