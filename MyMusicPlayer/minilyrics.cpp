#include<QLinearGradient>
#include<QPainter>
#include<QMessageBox>
#include<QDebug>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include"minilyrics.h"
#include<QFile>


#define SCROLL 30

//设置窗口位置，并初始化
miniLyrics::miniLyrics(QWidget *parent)
    : QWidget (parent)
{
    //设置字体格式
    setGeometry(200,200,500,240);
    setOriginalStatus();
    m_Font.setFamily("微软雅黑");
    setMaximumSize(1000,280);
    setMinimumSize(400,180);
    m_HLColor.setRgb(23,234,11);
    m_NlColor.setRgb(233,144,23);

    //初始化型号与槽
    initSignalsAndSlots();
}

//初始化型号与槽
void miniLyrics::initSignalsAndSlots()
{
    connect(&m_timer,SIGNAL(timeout()),
            this,SLOT(slot_timer()));
}

miniLyrics::~miniLyrics()
{

}

//初始化
void miniLyrics::initWidgetMISC()
{
    clearLrc();
    leftText=" ";
    rightText=" ";
    CurIndex=0;
    CurPos=0;
    CurStartPos=0;
    maskLen=0;
    CurLrc=" ";
}

//调用画图函数
void miniLyrics::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
//    analyzeLrcContent("28854853");
    DrawItem(painter);
}
//初始化
void miniLyrics::setOriginalStatus()
{
    percent=0;
    maskLen=0;
    leftText=" ";
    rightText=" ";
    CurPos=0;
    CurIndex=0;
    CurLrc=" ";
    CurStartPos=0;
}

QString miniLyrics::GetLrcByIndex(int index)
{
    return m_lineMap.values().value(index);
}

//传入song_id后爬取相应的歌曲信息
void miniLyrics::analyzeLrcContent(const QString &song_id)
{
    qDebug()<<"repitition!!!!";
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
    //******************************************************
    QString path = QDir::currentPath()+"/Lyrics/"+song_id+ ".rlc";
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray t = file.readAll();
    QString result=t;
    qDebug()<<result;
    file.close();

    //处理读取的歌词转化成好用的版本
    QStringList list=result.split("\n");
    list[0]=list[0].section("[",-1,-1);
    list[0]="["+list[0];
    list.pop_back();
    //qDebug()<<list;
    m_lineMap.insert(nPos," ");
    foreach (QString strLine, list)
    {
        //解析每一句歌词之前的初始化
        word_map.clear();
        interval_map.clear(); //interval,间隔时间
        strGetLine="";

        //m_lineMap.insert(nPos,strGetLine);获取每一句开始的时间和那一句歌词
        strGetLine=strLine.section("]",-1,-1);//获取那一句的歌词
        if(strGetLine.length()==0)
        {
            strGetLine+="· · · · · ·";
        }
        s_min=strLine.section(":",0,0).remove("[");
        min=s_min.toInt();
        s_sec=strLine.section(".",0,0).remove("[").remove(s_min).remove(":");
        sec=s_sec.toInt();
        s_msec=strLine.section("]",0,0).remove("[").remove(s_min).remove(":").remove(s_sec).remove(".");
        msec=s_msec.toInt();
        nPos=((min*60+sec)*1000+msec);//获取每一句开始的时间
        m_lineMap.insert(nPos,strGetLine);//开始时间和歌词
    }

    //qDebug()<<m_lineMap;
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
    /*for(int i=0;i<m_lineMap.count();i+=2)
    {
     DualLyc.insert(m_lineMap.values()[i],m_lineMap.values()[i+1]);
    }*/

//    qDebug()<<m_word_list;
//    qDebug()<<m_interval_list;

}

qint64 miniLyrics::GetPosByindex(int index)
{
    return m_lineMap.keys().value(index);
}

void miniLyrics::clearLrc()
{
    m_word_list.clear();
    m_interval_list.clear();

    m_lineMap.clear();
}

void miniLyrics::DrawItem(QPainter &Painter)
{
    float offset=abs(width()-height());
    if(width()>height())
    {
    m_Font.setPixelSize((height()/7+width()/(20+offset))/2);//得到字的像素
    }
    else if(width()<=height())
    {
        m_Font.setPixelSize((height()/(7+offset)+width()/20)/2);
    }
    else
    {
    }
    /*else {
        m_Font.setPixelSize((height()+width())/14);
    }*/

    float relativeW=width()/20;//设置相对宽度
    float relativeH=height()/6;//设置相对高度
    //int index=CurIndex/2;
    //leftText=DualLyc.keys()[index];
    //rightText=DualLyc.values()[index];
    int index=CurIndex;
    Painter.setFont(m_Font);
    Painter.setPen(m_NlColor);
    QFontMetrics metric(m_Font);
    int xRight=(width()-metric.QFontMetrics::horizontalAdvance(rightText)-relativeW);
    int yRight=(height()-metric.height()-relativeH);
    GetMaskLen();
    if(index%2==0)
    {
        leftText=m_lineMap.values()[index];
        if(index!=m_lineMap.count()-1)
        {
        rightText=m_lineMap.values()[index+1];
        }
        else
        {
            rightText=" ";
        }
        Painter.drawText(relativeW,relativeH,width(),metric.height(),Qt::AlignLeft,leftText);
        Painter.drawText(xRight,yRight,width(),metric.height(),Qt::AlignLeft,rightText);
        Painter.setPen(m_HLColor);
        Painter.drawText(relativeW,relativeH,maskLen,metric.height(),Qt::AlignLeft,leftText);
    }
    else
    {
        leftText=m_lineMap.values()[index-1];
        rightText=m_lineMap.values()[index];
        Painter.drawText(relativeW,relativeH,width(),metric.height(),Qt::AlignLeft,leftText);
        Painter.drawText(xRight,yRight,width(),metric.height(),Qt::AlignLeft,rightText);
        Painter.setPen(m_HLColor);
        Painter.drawText(xRight,yRight,maskLen,metric.height(),Qt::AlignLeft,rightText);
    }


   /* Painter.drawText(width()/10,width()/10,width(),metric.height(),Qt::AlignLeft,leftText);
    Painter.drawText(xRight,yRight,width(),metric.height(),Qt::AlignLeft,rightText);
    if(CurIndex%2==0)
    {
        Painter.setPen(m_HLColor);
        Painter.drawText(0,0,maskLen,metric.height(),Qt::AlignLeft,leftText);
    }
    else {
        Painter.setPen(m_HLColor);
        Painter.drawText(xRight,35,maskLen,metric.height(),Qt::AlignLeft,rightText);
    }*/
}

QString miniLyrics::GetLrcByTime(qint64 time)
{
    int index=GetIndexByTime(time);
    return m_lineMap.values().value(index);
}

void miniLyrics::getPosInfo(int &keyTime, int &interval, float &precent, QString &str)
{
    if(m_word_list.size()<=CurIndex)
    {
        return;
    }

    const QMap<int,QString> & word_map= m_word_list.at(CurIndex);

    const QMap<int ,int> & interval_map= m_interval_list.at(CurIndex);

    int subvalue=abs(CurPos-CurStartPos);
    int lt=0;
    int rt= interval_map.keys().count();
    int mid=0;

    while (lt < rt-1)
    {
        mid = (lt+rt)/2;
        if (interval_map.keys().value(mid) >subvalue)
        {
            rt = mid;
         }
        else
        {
            lt = mid;
         }
    }

    keyTime=interval_map.keys().value(lt);
    interval=interval_map.values().value(lt);
    precent=(float)lt/interval_map.size();
    str=word_map.values().value(lt);
}

void miniLyrics::GetMaskLen()
{
    if(CurPos<CurStartPos)
     {
        return;
      }

    static float s_fPercent=0.0f;
    static float s_keyLen=0.0f;
    static float s_curLen=0.0f;

    int keyTime=0;
    int interval=0;
    //是这个字占这一句总的百分比
    float percent1=0;
    //这个一个字已经播的占它总播放进度的百分比
    float percent2=0.0;
    QString strKeyWord(" ");
    //获取正在播放的那一个字的开始时间，间隔，在所有字数中的百分比，那一个字
    getPosInfo(keyTime,interval,percent,strKeyWord);
    QFont fontTemp=m_Font;
    QFontMetrics metrics(fontTemp);
    if(s_fPercent != percent)
    {
        s_keyLen=metrics.QFontMetrics::horizontalAdvance(CurLrc)*percent;
        s_fPercent=percent;
    }

    if(interval!=0)
    {
       //percent2=(float)(m_nCurPos-m_nCurStartPos-keyTime)/interval;
        percent2=(float)(CurPos-keyTime)/interval;
       s_curLen=s_keyLen+metrics.QFontMetrics::horizontalAdvance(strKeyWord)*percent2;
       maskLen=s_curLen;
       if(percent2<=1.0f && s_curLen >= maskLen)
          {
           maskLen=s_curLen;
          }
    }

}

int miniLyrics::GetIndexByTime(qint64 time)
{
    qint64 lt=0;
    qint64 rt= m_lineMap.values().count();
    qint64 mid=0;
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

void miniLyrics::slot_timer()
{
    GetMaskLen();
    repaint();
}

void miniLyrics::onPositionChanged(qint64 length)
{
    CurPos=length;
    CurIndex=GetIndexByTime(CurPos);
    CurStartPos=GetPosByindex(CurIndex);
    CurLrc=GetLrcByIndex(CurIndex);
     maskLen=0.0f;
     m_timer.start(SCROLL);
     GetMaskLen();
     update();
}

void miniLyrics::setInterval_list(const QList<QMap<int, int> > &interval_list)
{
    m_interval_list = interval_list;
}

void miniLyrics::setWord_list(const QList<QMap<int, QString> > &word_list)
{
    m_word_list = word_list;
}

void miniLyrics::setLineMap(const QMap<qint64, QString> &lineMap)
{
    m_lineMap = lineMap;
}







