#include "lyricwidget.h"
#include "ui_lyricwidget.h"
#include <QFile>
#include <QDebug>
#define SCROLL_TIME 300
#define SCROLL_TIMER_TIME 30
#define ROW_HEIGHT 60

LyricWidget::LyricWidget(QWidget *parent)
    :QWidget(parent),
    ui(new Ui::LyricWidget)
{
    ui->setupUi(this);
    m_strCurLrc="";
    m_nFontPixSize=20;
    m_normalFont.setFamily("微软雅黑");
    m_normalFont.setPixelSize(20);

    m_nCurIndex=0;
    m_nCurStartPos=0;
    m_nCurPos=0;
    m_nSroIndex=0;
    m_nOffset=0;
    m_nMaskLen=0;

    m_HLColor.setRgb(255,120,30);
    m_NlColor.setRgb(255,255,255);

    //触发信号窗口更新
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(slot_timer()));
}

LyricWidget::~LyricWidget()
{
    delete ui;
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
    //binary search
    //time complexity O(logn)
    qint64 lt=0;
    qint64 rt= m_lineMap.values().count();
    qint64 mid=0;
    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (m_lineMap.keys().value(mid) > time)
            rt = mid;
        else
            lt = mid;
    }
    return lt;
}

void LyricWidget::GetMaskLen(int nFontSize)
{
    //m_nCurStartPos定位的第一句歌词的位置
    if(m_nCurPos<m_nCurStartPos)//prevent the first line automatically scrolling
        return;

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
        s_keyLen=metrics.width(m_strCurLrc)*percent;
        s_fPercent=percent;
    }

    if(interval!=0)
    {
       percent2=(float)(m_nCurPos-m_nCurStartPos-keyTime)/interval;
       s_curLen=s_keyLen+metrics.width(strKeyWord)*percent2;
       if(percent2<=1.0f && s_curLen >= m_nMaskLen)
           m_nMaskLen=s_curLen;
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
    QMap<int ,int> interval_map; //interval,间隔时间

    //从本地读取文件
    QString path="D:/Git/clone/Lyric/Lyric"+song_id+ ".rlc";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray t = file.readAll();
    QString result=t;
    //qDebug()<<result;
    file.close();

    //处理读取的歌词转化成好用的版本
    QStringList list=result.split("\\n");
    list[0]=list[0].section("[",-1,-1);
    list[0]="["+list[0];
    list.pop_back();
    //qDebug()<<list;

    foreach (QString strLine, list)
    {
        //解析每一句歌词之前的初始化
        word_map.clear();
        interval_map.clear(); //interval,间隔时间
        strGetLine="";

        //m_lineMap.insert(nPos,strGetLine);获取每一句开始的时间和那一句歌词
        strGetLine=strLine.section("]",-1,-1);//获取那一句的歌词
        if(strGetLine.length()==0)
            strGetLine+=" ";
        s_min=strLine.section(":",0,0).remove("[");
        min=s_min.toInt();
        s_sec=strLine.section(".",0,0).remove("[").remove(s_min).remove(":");
        sec=s_sec.toInt();
        s_msec=strLine.section("]",0,0).remove("[").remove(s_min).remove(":").remove(s_sec).remove(".");
        msec=s_msec.toInt();
        nPos=((min*60+sec)*1000+msec);//获取每一句开始的时间
        m_lineMap.insert(nPos,strGetLine);//开始时间和歌词
    }

    qDebug()<<m_lineMap;
    for(int i=0;i<m_lineMap.keys().count();i++)
    {
        int count=0;
        QString strLine=m_lineMap.values()[i];
        if(i==m_lineMap.keys().count()-1)//最后一句歌词默认时长为20s
        {
            nRowTime=20000;
            nDur=nRowTime/strLine.length();
            foreach(strWord,strLine)
            {
                nKeyTime=m_lineMap.keys()[i]+count*nDur;
                word_map.insert(nKeyTime,strWord);
                interval_map.insert(nKeyTime,nDur);
                count++;
            }
        }
        else
        {
            nRowTime=m_lineMap.keys()[i+1]-m_lineMap.keys()[i];
            nDur=nRowTime/strLine.length();
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



    qDebug()<<m_word_list;
    qDebug()<<m_interval_list;

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
    if(m_word_list.size()<=m_nCurIndex)
        return;

    const QMap<int,QString> & word_map= m_word_list.at(m_nCurIndex);

    const QMap<int ,int> & interval_map= m_interval_list.at(m_nCurIndex);




    int subvalue=abs(m_nCurPos-m_nCurStartPos);//就是正在播放的时间点
    int lt=0;
    int rt= interval_map.keys().count();
    int mid=0;

    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (interval_map.keys().value(mid) >subvalue)//和每个字开始的时间比较
            rt = mid;
        else
            lt = mid;
    }


    keyTime=interval_map.keys().value(lt);
    interval=interval_map.values().value(lt);
    precent=(float)lt/interval_map.size();//第几个字/总共几个字算百分比
    str=word_map.values().value(lt);//返回的那个字吧
}

void LyricWidget::setOriginalStatus()
{
    clearLrc();
    m_strCurLrc="";
    m_nCurPos=0;
    m_nCurStartPos=0;
    m_nMaskLen=0;
    m_nCurIndex=-1;
    m_nSroIndex=0;
    m_nOffset=0;//偏移量
}

//重定义事件的虚函数
void LyricWidget::paintEvent(QPaintEvent *event)
{
    //一行歌词的信息
    static ItemInfo Info={0};
    QPainter painter(this);

    //一个界面上最多能显示多少行
    int nRowCount=height()/ROW_HEIGHT;

    for(int i=0;i<nRowCount;++i)
    {
        Info.index=(m_nSroIndex-nRowCount/2)+i;
        if(Info.index>=0)
        {
            Info.strText= GetLrcByIndex(Info.index);

            if(Info.index<m_nCurIndex)
                Info.alpha=100/(nRowCount/2)*i+150;
            else
                Info.alpha=-100/(nRowCount/2)*i+350;

            //m_nCurIndex标记的正在滚动的那一行
            if(Info.index==m_nCurIndex && m_nOffset!=0||Info.index==m_nCurIndex)//高亮显示正在滚动的歌词
                Info.hightLight=true;
            else
                Info.hightLight=false;

            Info.Y=i*ROW_HEIGHT+m_nOffset;

            DrawItem(painter,Info);
        }
    }
}

void LyricWidget::slot_timer()
{
    m_nOffset-= ROW_HEIGHT/(SCROLL_TIME/SCROLL_TIMER_TIME);//how many pix will be added per times
    if(abs(m_nOffset) >= ROW_HEIGHT)
    {
        m_nOffset=0;
        m_nSroIndex=m_nCurIndex;
        //m_timer.stop();
    }


    repaint();
}



void LyricWidget::positionChanged(qint64 length)//这应该是进度条触发后处理的槽函数
{
    m_nCurPos=length;
    int index = GetIndexByTime(m_nCurPos); //using binary search for the current lyric index;
    if (index != m_nCurIndex)//find a lyric // && 0==m_nOffset
    {
        m_nCurStartPos=GetPosByindex(index);
        m_strCurLrc=GetLrcByIndex(index);
        m_nSroIndex=index-1; //变成上一行~
        m_nCurIndex=index;
        m_nMaskLen=0.0f;
        m_timer.start(SCROLL_TIMER_TIME);
        //emit sig_currentLrcChange(m_lyric->getLineAt(index),m_lyric->getLineAt(index+1));
    }
    else if(0==m_nOffset)
    {
        GetMaskLen(m_nFontPixSize+ROW_HEIGHT/10);
        update();
    }
}

void LyricWidget::DrawItem(QPainter &Painter, ItemInfo &Info)
{
    if(Info.hightLight)//如果高亮
    {
        if(m_nOffset!=0)//滚动状态
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


