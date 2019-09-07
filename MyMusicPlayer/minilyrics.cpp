#include<QLinearGradient>
#include<QPainter>
#include<QMessageBox>
#include<QDebug>
#include<QHBoxLayout>
#include<QVBoxLayout>
#include<QFile>
#include"minilyrics.h"
#define SCROLL 30

//设置窗口位置，并初始化
miniLyrics::miniLyrics(QWidget *parent)
    : AbsFrameLessAutoSize(parent)
{
    m_precent = 0;
    m_border = 8 ;
    addition = 0;

    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::X11BypassWindowManagerHint|Qt::WindowStaysOnTopHint);
    //窗体透明，图标可见
    setAttribute(Qt::WA_TranslucentBackground,true);//半透明
    initWidgetMISC();//初始化界面
    //初始化型号与槽
    initSignalsAndSlots();
}

//初始化型号与槽
void miniLyrics::initSignalsAndSlots()
{
    connect(&m_timer,SIGNAL(timeout()),
            this,SLOT(slot_timer()));
    connect(m_closeBtn,SIGNAL(clicked()),
            this,SLOT(hide()));
    connect(m_forward,SIGNAL(clicked()),
            this,SLOT(on_forward_clicked()));
    connect(m_backward,SIGNAL(clicked()),
            this,SLOT(on_backward_clicked()));
}

miniLyrics::~miniLyrics()
{
      initWidgetMISC();
}

//初始化
void miniLyrics::initWidgetMISC()
{
    //梯度下降画出好看的歌词
    m_maskGradient.setColorAt(0.1,QColor(110,179,217));
    m_maskGradient.setColorAt(0.5, QColor(255,192,203));
    m_maskGradient.setColorAt(0.9, QColor(110,179,217));

    m_normalGradient.setColorAt(0.1, QColor(0, 75, 155));
    m_normalGradient.setColorAt(0.5, QColor(1, 110, 186));
    m_normalGradient.setColorAt(0.9, QColor(2, 168, 224));
    //相关参数设置
    m_isOriginalState=true;
    m_isDrawBrush=false;

    setMinimumSize(1000,120);
    setMaximumHeight(250);
    setOriginalStatus();
    QSize m_size(30,30);
    m_topWid=new QWidget(this);

    //m_topWid->setFixedHeight(20);

    /*QString Str="QPushButton"
                   "{background:cmyk(0,0,0,75);}"
                   "QPushButton::hover"
                   "{background:cmyk(0,0,0,85);}";
    QString style="QPushButton{border-image:url(:/image/topwidget/btn_close (1).png);background:rgb(0,0,0,75);}"
                                  "QPushButton::hover{border-image:url(:/image/topwidget/btn_close (2).png);}";*/
    //按钮布局
    m_previousBtn=new QPushButton;
    m_previousBtn->setIcon(QIcon(":/icon/res/16_Previous.png"));
    m_previousBtn->setIconSize(m_size);
    m_previousBtn->setFixedSize(30,30);
    m_previousBtn->setFlat(true);
    m_previousBtn->setToolTip("上一首");

    m_playBtn=new QPushButton;
    m_playBtn->setIcon(QIcon(":/icon/res/Pause (Filled) (1).png"));
    m_playBtn->setIconSize(m_size);
    m_playBtn->setFixedSize(30,30);
    m_playBtn->setFlat(true);
    m_playBtn->setToolTip("播放");

    m_nextBtn=new QPushButton;
    m_nextBtn->setFixedSize(30,30);
    m_nextBtn->setIconSize(m_size);
    m_nextBtn->setToolTip("下一首");
    m_nextBtn->setIcon(QIcon(":/icon/res/15_Next.png"));
    m_nextBtn->setFlat(true);

    m_closeBtn=new QPushButton(this);
    m_closeBtn->setIcon(QIcon(":/icon/res/closeLyrics.png"));
    m_closeBtn->setIconSize(m_size);
    m_closeBtn->setFixedSize(30,30);
    m_closeBtn->setFlat(true);
    m_closeBtn->setToolTip("关闭");

    m_forward=new QPushButton;
    m_forward->setFixedSize(30,30);
    m_forward->setIconSize(m_size);
    m_forward->setIcon(QIcon(":/icon/res/next6.png"));
    m_forward->setToolTip("歌词前进0.5秒");
    m_forward->setFlat(true);

    m_backward=new QPushButton;
    m_backward->setFixedSize(30,30);
    m_backward->setIconSize(m_size);
    m_backward->setIcon(QIcon(":/icon/res/previous6.png"));
    m_backward->setFlat(true);
    m_backward->setToolTip("歌词后退0.5秒");

    QHBoxLayout *hl=new QHBoxLayout;
    hl->setSpacing(25);
    hl->setContentsMargins(0,0,0,0);
    hl->setAlignment(Qt::AlignHCenter);
    hl->addWidget(m_previousBtn);
    hl->addWidget(m_playBtn);
    hl->addWidget(m_nextBtn);
    hl->addWidget(m_forward);
    hl->addWidget(m_backward);
    hl->addWidget(m_closeBtn);
    m_topWid->setFixedHeight(40);


    m_topWid->setLayout(hl);
    //隐藏相关按钮
    m_closeBtn->hide();
    m_playBtn->hide();
    m_previousBtn->hide();
    m_nextBtn->hide();
    m_backward->hide();
    m_forward->hide();
}

//画歌词
void miniLyrics::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
//    analyzeLrcContent("28854853");
    // 以下为两部分
   getCurrentLrc();

   QFont ft=font();
   QFontMetrics metrics(ft);
   if(!m_isOriginalState)
   {
           int y=((height()-20)/2-metrics.height())/2+20;
           int bottomwidth=width()-metrics.width(m_Text2);
        //第一部分

           m_normalGradient.setStart(0, y);
           m_normalGradient.setFinalStop(0, y+metrics.height());

           painter.setPen(QColor(0, 0, 0, 200));
           painter.drawText(11, y+1,metrics.width(m_Text1),metrics.height(),Qt::AlignLeft,m_Text1);//左对齐

           painter.setPen(QPen(m_normalGradient,0));
           painter.drawText(10, y,metrics.width(m_Text1),metrics.height(),Qt::AlignLeft, m_Text1);


           //第二部分

           m_normalGradient.setStart(0, y+height()/2);
           m_normalGradient.setFinalStop(0, y+(height()-20)/2+metrics.height());


           painter.setPen(QColor(0, 0, 0, 200));
           painter.drawText(bottomwidth-9, y+(height()-20)/2+1,metrics.width(m_Text2),metrics.height(),Qt::AlignLeft,m_Text2);//左对齐

           painter.setPen(QPen(m_normalGradient,0));
           painter.drawText(bottomwidth-10, y+(height()-20)/2,metrics.width(m_Text2),metrics.height(),Qt::AlignLeft, m_Text2);

           if(CurIndex%2==0)
           {
                   m_maskGradient.setStart(0, y);
                   m_maskGradient.setFinalStop(0, y+metrics.height());

                   painter.setPen(QPen(m_maskGradient, 0));
                   painter.drawText(10, y, maskLen, metrics.height(),Qt::AlignLeft ,m_Text1);

           }
           else
           {
                   m_maskGradient.setStart(0, y+(height()-20)/2);
                   m_maskGradient.setFinalStop(0, y+(height()-20)/2+metrics.height());

                   painter.setPen(QPen(m_maskGradient, 0));
                   painter.drawText(bottomwidth-10, y+(height()-20)/2, maskLen,  metrics.height(),Qt::AlignLeft, m_Text2);
           }
   }

   else//显示原始歌词
   {
       QString str="欢迎使用Rainbow音乐播放器";
       int y=(height()-20-metrics.height())/2+20;
       int x=(width()-metrics.width(str))/2;
       m_normalGradient.setStart(0, y);
       m_normalGradient.setFinalStop(0, y+metrics.height());

       painter.setPen(QColor(0, 0, 0, 200));
       painter.drawText(x+1,y+1,metrics.width(str),metrics.height(),Qt::AlignLeft,str);//左对齐

       painter.setPen(QPen(m_normalGradient,0));
       painter.drawText(x,y,metrics.width(str),metrics.height(),Qt::AlignLeft,str);//左对齐
   }

   if(m_isDrawBrush)//显示歌词操作框
   {
       painter.setPen(QPen(QColor(0,0,0,75),1,Qt::DotLine));
       painter.setBrush(QColor(0,0,0,75));
       painter.drawRect(1,20,width()-2,height()-21);
   }
}
//初始化大小
void miniLyrics::resizeEvent(QResizeEvent *)
{
    m_topWid->setGeometry(0,0,width(),20);
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(height()/4);
    setFont(font);
}
//鼠标进入事件
void miniLyrics::enterEvent(QEvent *)
{
    setCursor(Qt::SizeAllCursor);
    m_closeBtn->show();
    m_playBtn->show();
    m_previousBtn->show();
    m_nextBtn->show();
    m_forward->show();
    m_backward->show();
    m_isDrawBrush=true;
    update();
}
//鼠标离开事件
void miniLyrics::leaveEvent(QEvent *)
{
     m_isDrawBrush=false;
     setCursor(Qt::ArrowCursor);
     m_closeBtn->hide();
     m_playBtn->hide();
     m_previousBtn->hide();
     m_nextBtn->hide();
     m_backward->hide();
     m_forward->hide();
     update();
}

void miniLyrics::mouseMoveEvent(QMouseEvent *e)
{
    AbsFrameLessAutoSize::mouseMoveEvent(e);
    m_isDrawBrush=true;

    m_closeBtn->show();
    m_playBtn->show();
    m_previousBtn->show();
    m_nextBtn->show();
    m_forward->show();
    m_backward->show();
    update();
}
//初始化
void miniLyrics::setOriginalStatus()
{
    m_Text1="";
    m_Text2="";
    m_isOriginalState=true;
    CurIndex=0;
    clearLrc();
    update();
}

void miniLyrics::setCurrentAndNextLrc(const QString &, const QString &)
{

}

//获取两句可显示歌词的前一句和后一句并获取当前高亮歌词
void miniLyrics::getCurrentLrc()
{
    //clearLrc();
    //analyzeLrcContent("28854853");
    int index=CurIndex;
    if(index<m_lineMap.count())
    {
    if(index%2==0)
    {
       m_Text1=m_lineMap.values()[index];
        if(index!=m_lineMap.count()-1)
        {
        m_Text2=m_lineMap.values()[index+1];}
        else {
            m_Text2=" ";
        }
    }
    else if(index%2==1)
    {
        m_Text1=m_lineMap.values()[index-1];
        m_Text2=m_lineMap.values()[index];
    }
    }
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

QString miniLyrics::GetLrcByTime(qint64 time)
{
    int index = GetIndexByTime(time);
    return m_lineMap.values().value(index);
}

void miniLyrics::getPosInfo(int &keyTime, int &interval, float &precent, QString &str)
{
    if(m_word_list.size() <= CurIndex)
    {
        return;
    }

    const QMap<int,QString> & word_map= m_word_list.at(CurIndex);

    const QMap<int ,int> & interval_map= m_interval_list.at(CurIndex);

    int subvalue = abs(CurPos-CurStartPos);
    int lt = 0;
    int rt = interval_map.keys().count();
    int mid = 0;

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

    static float s_fPercent = 0.0f;
    static float s_keyLen = 0.0f;
    static float s_curLen = 0.0f;

    int keyTime = 0;
    int interval = 0;
    //是这个字占这一句总的百分比
    float percent1 = 0;
    //这个一个字已经播的占它总播放进度的百分比
    float percent2 = 0.0;
    QString strKeyWord(" ");
    //获取正在播放的那一个字的开始时间，间隔，在所有字数中的百分比，那一个字
    getPosInfo(keyTime,interval,percent,strKeyWord);
    QFont fontTemp=font();
    QFontMetrics metrics(fontTemp);
    if(s_fPercent != percent)
    {
        s_keyLen=metrics.QFontMetrics::horizontalAdvance(CurLrc)*percent;
        s_fPercent=percent;
    }

    if(interval != 0)
    {
       //percent2=(float)(m_nCurPos-m_nCurStartPos-keyTime)/interval;
        percent2=(float)(CurPos-keyTime)/interval;
       s_curLen=s_keyLen+metrics.QFontMetrics::horizontalAdvance(strKeyWord)*percent2;
       maskLen=s_curLen;
       if(percent2 <=1.0f && s_curLen >= maskLen)
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

//前进0.5秒的按钮按下了
void miniLyrics::on_forward_clicked()
{
    addition+=500;
}

//后退0.5的按钮按下了
void miniLyrics::on_backward_clicked()
{
    addition-=500;
}

//表示接下来播放
void miniLyrics::onBecomePlaying()
{
    m_playBtn->setIcon(QIcon(":/icon/res/Pause (Filled) (1).png"));
}

//表示接下来暂停
void miniLyrics::onBecomePausing()
{
    m_playBtn->setIcon(QIcon(":/icon/res/play8.png"));
}

void miniLyrics::onPositionChanged(qint64 length)
{
    CurPos=length+addition;
    CurIndex=GetIndexByTime(CurPos);
    CurStartPos=GetPosByindex(CurIndex);
    CurLrc=GetLrcByIndex(CurIndex);
     maskLen=0.0f;
     m_isOriginalState=false;
     GetMaskLen();
     m_timer.start(SCROLL);
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







