#ifndef DISKWIDGET_H
#define DISKWIDGET_H

#include <QPixmap>
#include <QWidget>

class QTimer;
class diskWidget : public QWidget
{
    Q_OBJECT
public:
    explicit diskWidget(QWidget *parent = nullptr,int r=100);

protected:
    void paintEvent(QPaintEvent *event);

signals:

public slots:
    void rotate();
    void rotateStop();
    void rotateStart();
    void changePic(QString newPicPath);

private:
    void PixmapToRound(QString pixmapPath);
    void setRadius(int radius);
    void setRate(double rate);
    QPixmap fitpixmap_userIcon;
    int count;
    QTimer *timer;
    int i_radius;
    double d_rate;
};

#endif // DISKWIDGET_H
