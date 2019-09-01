#ifndef LYRICLABEL_H
#define LYRICLABEL_H
#include <QLabel>
#include <QMouseEvent>
#include <QPoint>


class LyricLabel : public QLabel
{
public:
    LyricLabel();

protected:
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
private:
    QPoint m_mouseStartPoint;
};

#endif // LYRICLABEL_H
