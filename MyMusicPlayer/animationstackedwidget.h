//这是带动画效果的自定义QStackedWidget
#ifndef ANISTACKEDWIDGET
#define ANISTACKEDWIDGET

#include<QStackedWidget>
#include<QPainter>
#include<QPropertyAnimation>
class AnimationStackedWidget : public QStackedWidget
{
    Q_OBJECT
public:
    explicit AnimationStackedWidget(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);
    //设置动画持续的间隔
    void setDuration(int);

    ~AnimationStackedWidget();

signals:

public slots:
    //属性动画值改变的槽
    void valueChanged_slot(QVariant );
    //动画切换完成
    void animationFinished();
    //前一页
    void nextPage();
    //下一页

private:
    void paintNext(QPainter &, int index);

private:
    QPropertyAnimation *animation;
    int duration;                   //动画的持续时间
    bool isAnimation;               //是否正在动画
    QVariant currentValue;    //被Animation动画改变的值

};

#endif
