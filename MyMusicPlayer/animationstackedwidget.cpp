#include"animationstackedwidget.h"
AnimationStackedWidget::AnimationStackedWidget(QWidget *parent) :
    QStackedWidget(parent)
{
    isAnimation = false;
    //设置默认的时间间隔
    duration = 700;
    //初始化animation框架、并连接信号和槽
    animation = new QPropertyAnimation(this, QByteArray());
    connect(animation, SIGNAL(valueChanged(QVariant)),
            this, SLOT(valueChanged_slot(QVariant)));
    connect(animation, SIGNAL(finished()),
            this, SLOT(animationFinished()));
}

void AnimationStackedWidget::nextPage()
{
    //如果正在动画，那么return
    if( isAnimation )
    {
        return;
    }
    isAnimation = true;

    //隐藏当前的widget
    widget(currentIndex())->hide();
    //开始动画并设置间隔和开始、结束值
    QRect g = geometry();
    int height = g.height();
    animation->setStartValue(height);
    animation->setEndValue(0);
    animation->setDuration(duration);
    animation->start();
}

void AnimationStackedWidget::valueChanged_slot(QVariant value)
{
    currentValue = value;
    update();
}

void AnimationStackedWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    if(isAnimation)
    {
        QPainter paint(this);
        paintNext(paint, currentIndex());
    }
}

void AnimationStackedWidget::paintNext(QPainter &paint, int index)
{
    if(index ==1)//当前为详情页
    {
        //背景为首页
        QWidget *nextWidget = widget(count()-index-1);
        QRect r = geometry();
        nextWidget->resize(r.width(), r.height());
        QPixmap nextPixmap(nextWidget->size());
        nextWidget->render(&nextPixmap);
        paint.drawPixmap(0,0,nextPixmap);

        //当前页面向下滚动
        QWidget *w = widget(index);
        QPixmap pixmap(w->size());
        //将Widget的内容存成一张图片
        w->render(&pixmap);
        r = w->geometry();
        double value = currentValue.toDouble();
        QRectF r1(0, 0, r.width(), value);
        QRectF r2(0,r.height() - value, r.width(), value);
        paint.drawPixmap(r2,pixmap,r1);
    }
    else//当前为主页
    {
    //背景为主页
    QWidget *w = widget(index);
    QPixmap pixmap(w->size());
    //将Widget的内容存成一张图片
    w->render(&pixmap);
    paint.drawPixmap(0,0,pixmap);

    //详情页向上滚
    QWidget *nextWidget = widget(count()-index-1);
    QRect r = geometry();
    nextWidget->resize(r.width(), r.height());
    QPixmap nextPixmap(nextWidget->size());
    nextWidget->render(&nextPixmap);
    double value = currentValue.toDouble();
    QRectF r1(0.0,value, r.width(), r.height() - value);
    QRectF r2(0.0, 0.0, r.width(), r.height() - value);
    paint.drawPixmap(r1, nextPixmap, r2);
    }
}

void AnimationStackedWidget::animationFinished()
{
    //动画结束后设置CurrentIndex
    isAnimation = false;
    widget(currentIndex())->show();
    setCurrentIndex(count()-currentIndex()-1);

}
AnimationStackedWidget::~AnimationStackedWidget()
{

}
