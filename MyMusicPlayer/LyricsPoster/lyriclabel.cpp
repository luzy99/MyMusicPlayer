#include "lyriclabel.h"

LyricLabel::LyricLabel()
    :
{

}

void LyricLabel::mouseMoveEvent(QMouseEvent *event)
{
    m_mouseStartPoint = event->globalPos();
}

void LyricLabel::mousePressEvent(QMouseEvent *event)
{
    QPoint offset=event->globalPos()-m_mouseStartPoint;
    this->move(offset);
}
