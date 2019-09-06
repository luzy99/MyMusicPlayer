#include "lyricwidget.h"
#include <QFile>
#include <QHBoxLayout>
#include <QDebug>
#include <QMouseEvent>
#include <QDateTime>

#define SCROLL_TIME 300
#define SCROLL_TIMER_TIME 30
#define ROW_HEIGHT 60

LyricWidget::LyricWidget(QWidget *parent)
    :QWidget(parent)
{
    //初始化改变播放按钮
    btnPlaythis = new QPushButton;
    btnPlaythis->setFlat(true);
    btnPlaythis->setIcon(QIcon(":/icon/res/currentRow.png"));
    btnPlaythis->setIconSize(QSize(20,20));
    btnPlaythis->setStyleSheet("background-color:rgba(0,0,0,0)");

    //初始化显示事件的标签
    lab_showtime=new QLabel;
    lab_showtime->setText("10:00");

//    QWidget *pWindow = this->window();
//    QSize pWindowSize = pWindow->size();
//    int sizeY = pWindowSize.height();
//    int sizeX = pWindowSize.width();
//    btnPlaythis->setGeometry(0,(pWindowSize.height()-45)/2,45,45);

    QHBoxLayout *HLay1=new QHBoxLayout(this);
    HLay1->setContentsMargins(0,0,5,0);

    HLay1->addWidget(btnPlaythis,0,Qt::AlignLeft);
    HLay1->addWidget(lab_showtime,0,Qt::AlignRight);

    //设置歌词的信息参数
    m_strCurLrc="";
    m_nFontPixSize=20;
    m_normalFont.setFamily("微软雅黑");
    m_normalFont.setPixelSize(20);

    //初始化未知量
    m_nCurIndex=-1;
    m_nCurStartPos=0;
    m_nCurPos=0;
    m_nSroIndex=0;
    m_nOffset=0;
    m_nMaskLen=0;

    //设置颜色
    m_HLColor.setRgb(255,120,30);
    m_NlColor.setRgb(255,255,255);

    //链接型号与槽
    initSignalsAndSlots();
}

void LyricWidget::initSignalsAndSlots()
{
    //触发信号窗口更新
    connect(btnPlaythis,SIGNAL(clicked()),
            this,SLOT(on_btnPlaythis_clicked()));
    //处理计时器
    connect(&m_timer,SIGNAL(timeout()),
            this,SLOT(slot_timer()));
    //位置改变时触发信号
    connect(this,SIGNAL(positionChanged(qint64)),
            this,SLOT(onPositionChanged(qint64)));
}

LyricWidget::~LyricWidget()
{

}

QString LyricWidget::GetLrcByIndex(int index)
{
    return m_lineMap.values().value(index);
}

qint64 LyricWidget::GetPosByindex(int index)
{
     return m_lineMap.keys().value(index);
}

QString LyricWidget::GetLrcByTime(qint64 time)
{
    int index=GetIndexByTime(time);
    return m_lineMap.values().value(index);
}

int LyricWidget::GetIndexByTime(qint64 time)
{
    //二分发查找
    //复杂度 O(logn)
    qint64 lt = 0;
    qint64 rt = m_lineMap.values().count();
    qint64 mid = 0;
    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (m_lineMap.keys().value(mid) > time)
        {
            rt = mid;
        }
        else
        {
            lt = mid;
        }
    }
    return lt;
}

void LyricWidget::GetMaskLen(int nFontSize)
{
    //m_nCurStartPos定位的第一句歌词的位置
    //防止第一行歌词自动高亮
    if(m_nCurPos < m_nCurStartPos)
    {
        return;
    }

    static float s_fPercent=0.0f;
    static float s_keyLen=0.0f;
    static float s_curLen=0.0f;

    int keyTime=0;
    int interval=0;

    //是这个字占这一句总的百分比
    float percent=0;
    //这个一个字已经播的占它总播放进度的百分比
    float percent2=0.0;

    QString strKeyWord(" ");
    //获取正在播放的那一个字的开始时间，间隔，在所有字数中的百分比，那一个字
    getPosInfo(keyTime,interval,percent,strKeyWord);

    QFont fontTemp=m_normalFont;
    fontTemp.setPixelSize(nFontSize);
    QFontMetrics metrics(fontTemp);
    if(s_fPercent != percent)
    {
        s_keyLen=metrics.QFontMetrics::horizontalAdvance(m_strCurLrc)*percent;
        s_fPercent=percent;
    }

    if(interval != 0)
    {
       //percent2=(float)(m_nCurPos-m_nCurStartPos-keyTime)/interval;
       percent2=(float)(m_nCurPos-keyTime)/interval;
      // s_curLen=s_keyLen+metrics.width(strKeyWord)*percent2;
       s_curLen=s_keyLen+metrics.QFontMetrics::horizontalAdvance(strKeyWord)*percent2;
       m_nMaskLen=s_curLen;
       if(percent2<=1.0f && s_curLen >= m_nMaskLen)
       {
           m_nMaskLen=s_curLen;
       }
    }
}

void LyricWidget::analyzeLrcContent(const QString& song_id)
{
    clearLrc();

    QString strWord;//每一个字的歌词
    QString strGetLine;//所有的字加起来的一句歌词

    int min;
    int sec;//提取的分，秒，毫秒
    int msec;

    QString s_min;
    QString s_sec;
    QString s_msec;

    qint64 nPos=0;         //总时间位置
    int nRowTime=0;        //行总时间
    int nKeyTime=0;        //每一字开始的时间
    int nDur=0;       //每一字持续的时间
    //int nIndex=0;     //歌词放到第几句了

    QMap<int,QString> word_map;
    //interval,间隔时间
    QMap<int ,int> interval_map;
    //从本地读取文件
    //***********************************************
    QString path = QDir::currentPath()+"/Lyrics/"+song_id+ ".rlc";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray t = file.readAll();
    QString result = t;
    //qDebug()<<result;
    file.close();

    //处理读取的歌词转化成好用的版本
    QStringList list = result.split("\n");
    list[0] = list[0].section("[",-1,-1);
    list[0] = "["+list[0];
    list.pop_back();
    //qDebug()<<list;

    m_lineMap.insert(nPos," ");
    foreach (QString strLine, list)
    {
        //解析每一句歌词之前的初始化
        word_map.clear();
        interval_map.clear(); //interval,间隔时间
        strGetLine = "";

        //m_lineMap.insert(nPos,strGetLine);获取每一句开始的时间和那一句歌词
        //获取那一句的歌词
        strGetLine = strLine.section("]",-1,-1);
        if(strGetLine.length() == 0)
        {
            strGetLine += " ";
        }
        s_min = strLine.section(":",0,0).remove("[");
        min=s_min.toInt();
        s_sec = strLine.section(".",0,0).remove("[").remove(s_min).remove(":");
        sec=s_sec.toInt();
        s_msec = strLine.section("]",0,0).remove("[").remove(s_min).remove(":").remove(s_sec).remove(".");
        msec=s_msec.toInt();
        nPos = ((min*60+sec)*1000+msec);//获取每一句开始的时间
        m_lineMap.insert(nPos,strGetLine);//开始时间和歌词
    }

    //qDebug()<<m_lineMap;
    for(int i = 0 ; i < m_lineMap.keys().count() ; i++)
    {
        int count = 0;
        QString strLine = m_lineMap.values()[i];
        //最后一句歌词默认时长为20s
        if(i == m_lineMap.keys().count()-1)
        {
            nRowTime = 20000;
            nDur = nRowTime/strLine.length();
            foreach(strWord,strLine)
            {
                nKeyTime = m_lineMap.keys()[i]+count*nDur;
                word_map.insert(nKeyTime,strWord);
                interval_map.insert(nKeyTime,nDur);
                count++;
            }
        }
        else
        {
            nRowTime = m_lineMap.keys()[i+1]-m_lineMap.keys()[i];
            nDur = nRowTime/strLine.length();
            foreach(strWord,strLine)
            {
                nKeyTime=m_lineMap.keys()[i]+count*nDur;
                word_map.insert(nKeyTime,strWord);
                interval_map.insert(nKeyTime,nDur);
                count++;
            }
        }

        m_word_list<<word_map;
        m_interval_list<<interval_map;
        word_map.clear();
        interval_map.clear();
    }

//    qDebug()<<m_word_list;
//    qDebug()<<m_interval_list;

}

void LyricWidget::clearLrc()
{
    m_word_list.clear();
    m_interval_list.clear();

    m_lineMap.clear();
}

//返回的正在播放的那个字的信息（这个字在总字数的百分比）
void LyricWidget::getPosInfo(int &keyTime, int &interval, float &precent, QString &str)
{
    if(m_word_list.size() <= m_nCurIndex)
    {
        return;
     }

    const QMap<int,QString> & word_map= m_word_list.at(m_nCurIndex);

    const QMap<int ,int> & interval_map= m_interval_list.at(m_nCurIndex);

    //就是正在播放的时间点：
    int subvalue = abs(m_nCurPos-m_nCurStartPos);
    int lt = 0;
    int rt = interval_map.keys().count();
    int mid = 0;

    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (interval_map.keys().value(mid) > subvalue)//和每个字开始的时间比较
        {
            rt = mid;
        }
        else
        {
            lt = mid;
        }
    }

    keyTime = interval_map.keys().value(lt);
    interval = interval_map.values().value(lt);
    precent = (float)lt/interval_map.size();//第几个字/总共几个字算百分比
    str = word_map.values().value(lt);//返回的那个字吧
}

void LyricWidget::setOriginalStatus()
{
    clearLrc();
    m_strCurLrc = "";
    m_nCurPos = 0;
    m_nCurStartPos = 0;
    m_nMaskLen = 0;
    m_nCurIndex = -1;
    m_nSroIndex = 0;
    m_nOffset = 0;//偏移量
}

//重定义事件的虚函数
void LyricWidget::paintEvent(QPaintEvent *event)
{
    //一行歌词的信息
    static ItemInfo Info={0};
    QPainter painter(this);

    //一个界面上最多能显示多少行
    int nRowCount = height()/ROW_HEIGHT;

    for(int i=0 ; i < nRowCount ; ++i)
    {
        Info.index = (m_nSroIndex-nRowCount/2)+i;
        if(Info.index >= 0)
        {
            Info.strText = GetLrcByIndex(Info.index);

            if(Info.index < m_nCurIndex)
            {
                Info.alpha = 100/(nRowCount/2)*i+150;
            }
            else
            {
                Info.alpha =- 100/(nRowCount/2)*i+350;
            }

            //m_nCurIndex标记的正在滚动的那一行
            //高亮显示正在滚动的歌词
            if(Info.index == m_nCurIndex && m_nOffset!=0||Info.index==m_nCurIndex)
            {
                Info.hightLight = true;
            }
            else
            {
                Info.hightLight = false;
            }

            Info.Y=i*ROW_HEIGHT+m_nOffset;

            DrawItem(painter,Info);
        }
    }
}

void LyricWidget::mousePressEvent(QMouseEvent* event)
{
//    qDebug()<<"press event";
    //记录鼠标的初始位置
    m_mouseStartPoint = event->globalPos();
    emit blockSignals(true);
}

void LyricWidget::mouseMoveEvent(QMouseEvent *event)
{
//    qDebug()<<"move event";
    //窗口和鼠标偏移量一致
    QPoint offset = event->globalPos() - m_mouseStartPoint;
    //得到鼠标的偏移量
    int offset_width = offset.y();
//    int offset_index = offset_width/ROW_HEIGHT;
//    m_nCurIndex += offset_index;
   //代替信号被阻塞的进度条发出时间信号（stoptime+偏移量计算得出）
    qint64 virtualpositon= m_nStoptime - offset_width*15;
    emit positionChanged(virtualpositon);
}

//鼠标松开将现在这一句发送给进度条滚动控制权归还给进度条
void LyricWidget::mouseReleaseEvent(QMouseEvent *event)
{
//    qDebug()<<"release event";
    qint64 changedtime = m_nCurStartPos;
    emit positionChanged(changedtime);
    emit positionDraggedTo(changedtime);
    emit blockSignals(false);
}

void LyricWidget::on_btnPlaythis_clicked()
{
//    qint64 position=GetPosByindex(m_nCurIndex);
    //    emit positionChanged(position);
}

void LyricWidget::on_positionStop(qint64 stoptime)
{
//    qDebug()<<"on_positionStop";
       m_nStoptime = stoptime;
}

void LyricWidget::slot_timer()
{
    //计算每秒加几个像素
    m_nOffset-= ROW_HEIGHT/(SCROLL_TIME/SCROLL_TIMER_TIME);
    if(abs(m_nOffset) >= ROW_HEIGHT)
    {
        m_nOffset = 0;
        m_nSroIndex = m_nCurIndex;
        m_timer.stop();
    }

    GetMaskLen(m_nFontPixSize+abs(m_nOffset/1000));
    //update()
    repaint();
}

//这应该是进度条触发后处理的槽函数
void LyricWidget::onPositionChanged(qint64 length)
{
    QDateTime time = QDateTime::fromMSecsSinceEpoch(length);
    lab_showtime->setText(time.toString("mm:ss"));

    m_nCurPos=length;
    //用二元搜索现在这句歌词的编号
    int index = GetIndexByTime(m_nCurPos);
    if (index != m_nCurIndex)//寻找一句歌词
    {
        m_nCurStartPos = GetPosByindex(index);
        m_strCurLrc = GetLrcByIndex(index);
        m_nSroIndex = index-1; //变成上一行~
        m_nCurIndex = index;
        m_nMaskLen = 0.0f;
        m_timer.start(SCROLL_TIMER_TIME);
        //emit sig_currentLrcChange(m_lyric->getLineAt(index),m_lyric->getLineAt(index+1));
    }
    else if(0 == m_nOffset) // && 0==m_nOffset
    {
        GetMaskLen(m_nFontPixSize+ROW_HEIGHT/10);
        update();
    }
}

void LyricWidget::DrawItem(QPainter &Painter, ItemInfo &Info)
{
    if(Info.hightLight)//如果高亮
    {
        if(m_nOffset != 0)//滚动状态
        {
            //pixel size 是所占的像素大小
            //字体在屏幕上的实际大小 = 字体像素大小 * 点距
            m_normalFont.setPixelSize(m_nFontPixSize+abs(m_nOffset/10));
            Painter.setFont(m_normalFont);
        }
        else//非滚动状态
        {
            m_normalFont.setPixelSize(m_nFontPixSize+ROW_HEIGHT/10);
            Painter.setFont(m_normalFont);
        }
        m_NlColor.setAlpha(Info.alpha);
        Painter.setPen(m_HLColor);//设置画笔颜色（高亮颜色）
    }
    else
    {
        if(m_nSroIndex == Info.index)
        {
            m_normalFont.setPixelSize(m_nFontPixSize+ROW_HEIGHT/10-abs(m_nOffset/10));
            Painter.setFont(m_normalFont);
        }
        else
        {
            m_normalFont.setPixelSize(m_nFontPixSize);
            Painter.setFont(m_normalFont);
        }

        m_NlColor.setAlpha(Info.alpha);
        Painter.setPen(m_NlColor);//设置画笔颜色（普通颜色）
    }

    //计算出字符串的宽度
    QFontMetrics metric(m_normalFont);

    //让字体在那一行垂直和水平都居中
    int X= (width()-metric.QFontMetrics::horizontalAdvance(Info.strText))/2;//整个歌词栏的宽度-歌词的宽度/2
    Info.Y+=(ROW_HEIGHT-metric.height())/2;

    //所有的歌词都用normal的颜色画一遍
    Painter.setFont(m_normalFont);
    Painter.drawText(X,Info.Y,width(),metric.height(),Qt::AlignLeft,Info.strText);

    //高亮颜色再高亮一遍
    if(Info.hightLight)
    {
        Painter.setPen(QColor(255,40,80));
        Painter.drawText(X,Info.Y,m_nMaskLen,metric.height(),
                              Qt::AlignLeft,Info.strText);
    }
}

QList<QMap<int, int> > LyricWidget::getInterval_list() const
{
    return m_interval_list;
}

QList<QMap<int, QString> > LyricWidget::getWord_list() const
{
    return m_word_list;
}

QMap<qint64, QString> LyricWidget::getLineMap() const
{
    return m_lineMap;
}
