#include "absframelessautosize.h"
#include <qdebug.h>
AbsFrameLessAutoSize::AbsFrameLessAutoSize(QWidget *parent)
    : QWidget(parent)
{
    //初始化界面
    setGeometry(440,900,150,50);
    m_state.MousePressed=false;
    m_state.IsPressBorder=false;
    setMinimumSize(150,50);
    m_border=4;
    setMouseTracking(true);
    setWindowFlags(Qt::FramelessWindowHint); //标题栏图标可见
    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
}

void AbsFrameLessAutoSize::setBorder(int border)
{
    m_border=border;
}
//根据鼠标位置更改鼠标形状
void AbsFrameLessAutoSize::mouseMoveRect(const QPoint& p)
{
    if(!m_state.IsPressBorder)
    {
        if( p.x()>width()-m_border&&p.y()<height()-m_border&&p.y()>m_border)
            //右边框
        {
            setCursor(Qt::SizeHorCursor);
            m_curPos= CursorPos::Right;
        }
        else if(p.x()<m_border&&p.y()<height()-m_border&&p.y()>m_border)//左边框
        {
            setCursor(Qt::SizeHorCursor);
            m_curPos= CursorPos::Left;
        }
        else if(p.y()>height()-m_border&&p.x()>m_border&&p.x()<width()-m_border)//底边框
        {
            setCursor(Qt::SizeVerCursor);
            m_curPos= CursorPos::Bottom;
        }
        else if(p.y()<m_border&&p.x()>m_border&&p.x()<width()-m_border)
        {
            setCursor(Qt::SizeVerCursor);
            m_curPos=CursorPos::Top;
        }
//四个角落
        else if(p.y()<m_border&&p.x()>width()-m_border)
        {
            //右上角
            setCursor(Qt::SizeBDiagCursor);
            m_curPos=CursorPos::TopRight;
        }

        else if(p.y()<m_border&&p.x()<m_border)
        {
            //左上角
            setCursor(Qt::SizeFDiagCursor);
            m_curPos=CursorPos::TopLeft;
        }

        else if(p.x()>m_border&&p.y()>height()-m_border)
        {//右下角
            setCursor(Qt::SizeFDiagCursor);
            m_curPos=CursorPos::BottomRight;
        }

        else if(p.x()<m_border&&p.y()>height()-m_border)
        {//左下角
            setCursor(Qt::SizeBDiagCursor);
            m_curPos=CursorPos::BottomLeft;
        }
////
        else if(p.y()<height()-m_border&&p.y()>m_border&&p.x()>m_border&&p.x()<width()-m_border)
        {
            //鼠标停留在歌词界面上
            setCursor(Qt::OpenHandCursor);
            m_curPos=CursorPos::Within;
        }
        else
        {//其它为默认的箭头
            m_curPos=CursorPos::Default;
            setCursor(Qt::ArrowCursor);
        }
    }
    //拖动变换边框大小
    else
    {
        switch (m_curPos) {
        case CursorPos::Right:
        {
           int setW=QCursor::pos().x()-x();
           if(minimumWidth()<=setW&&setW<=maximumWidth())
             setGeometry(x(),y(),setW,height());
            break;
        }
        case CursorPos::Left:
        {
            int setW=x()+width()-QCursor::pos().x();
            int setX=QCursor::pos().x();
            if(minimumWidth()<=setW&&setW<=maximumWidth())
              setGeometry(setX,y(),setW,height());
             break;
        }
        case CursorPos::Bottom:
        {
            int setH=QCursor::pos().y()-y();
            if(minimumHeight()<=setH&&setH<=maximumHeight())
            setGeometry(x(),y(),width(),setH);
            break;
        }
        case CursorPos::Top:
        {
           int setH=y()-QCursor::pos().y()+height();
           if(minimumHeight()<=setH&&setH<=maximumHeight())
             setGeometry(x(),QCursor::pos().y(),width(),setH);
             break;
        }
        case CursorPos::TopRight:
        {
            int setH=y()+height()-QCursor::pos().y();
            int setW=QCursor::pos().x()-x();
            int setY=QCursor::pos().y();
            if(setH>=maximumHeight())
            {
                setY=m_state.WindowPos.y()+m_state.PressedSize.height()-height();
                setH=maximumHeight();
            }
            if(setH<=minimumHeight())
            {
                setY=m_state.WindowPos.y()+m_state.PressedSize.height()-height();
                setH=minimumHeight();
            }
            setGeometry(m_state.WindowPos.x(),setY,setW,setH);
            break;
        }
        case CursorPos::TopLeft:
        {
            int setY=QCursor::pos().y();
            int setX=QCursor::pos().x();

            int setW=pos().x()+width()-setX;
            int setH=pos().y()+height()-setY;
            int totalW= m_state.WindowPos.x()+m_state.PressedSize.width();
            int totalH=m_state.WindowPos.y()+m_state.PressedSize.height();

            if(totalW-setX>=maximumWidth())
            {
                setX=totalW-maximumWidth();
                setW=maximumWidth();
            }
            if(totalW-setX<=minimumWidth())
            {
                setX=totalW-minimumWidth();
                setW=minimumWidth();
            }
            if(totalH-setY>=maximumHeight())
            {
                setY=totalH-maximumHeight();
                setH=maximumHeight();
            }
            if(totalH-setY<=minimumHeight())
            {
                setY=totalH-minimumHeight();
                setH=minimumHeight();
            }
            setGeometry(setX,setY,setW,setH);
            break;
        }
        case CursorPos::BottomRight:
        {
           int setW=QCursor::pos().x()-x();
           int setH=QCursor::pos().y()-y();
           setGeometry(m_state.WindowPos.x(),m_state.WindowPos.y(),setW,setH);
             break;
        }
        case CursorPos::BottomLeft:
        {
            int setW=x()+width()-QCursor::pos().x();
            int setH=QCursor::pos().y()-m_state.WindowPos.y();
            int setX=QCursor::pos().x();
            int totalW= m_state.WindowPos.x()+m_state.PressedSize.width();
            if(totalW-setX>=maximumWidth())
            {
                setX=totalW-maximumWidth();
                setW=maximumWidth();
            }
            if(totalW-setX<=minimumWidth())
            {
                setX=totalW-minimumWidth();
                setW=minimumWidth();
            }
            setGeometry(setX,m_state.WindowPos.y(),setW,setH);
            break;
        }
        case CursorPos::Default:
        default:
            break;
        }
    }
}
//鼠标按下时的鼠标图标变换
void AbsFrameLessAutoSize::mousePressEvent(QMouseEvent *event)
{
    m_state.PressedSize=this->size();
    m_state.IsPressBorder=false;
      setFocus();
      if (event->button() == Qt::LeftButton)
     {
          setCursor(Qt::ClosedHandCursor);
          m_state.WindowPos = this->pos(); //保存按下坐标
          if(QRect(m_border+1,m_border+1,width()-(m_border+1)*2,height()-(m_border+1)*2).contains(QPoint(event->globalX()-x(),event->globalY()-y())))
          {
              m_state.MousePos = event->globalPos();
              m_state.MousePressed = true;
          }
          else
              m_state.IsPressBorder=true;
      }
}
//鼠标移动操作
void AbsFrameLessAutoSize::mouseMoveEvent(QMouseEvent *event)
{
    mouseMoveRect(mapFromGlobal(event->globalPos()));
    if (m_state.MousePressed)
        {
            setCursor(Qt::ClosedHandCursor);
            this->move(m_state.WindowPos + (event->globalPos() - m_state.MousePos));
        }
}
//松开鼠标操作
void AbsFrameLessAutoSize::mouseReleaseEvent(QMouseEvent *event)
{
      m_state.IsPressBorder=false;
    if (event->button() == Qt::LeftButton)
    {
        this->m_state.MousePressed = false;
    }
    setCursor(Qt::OpenHandCursor);
}
