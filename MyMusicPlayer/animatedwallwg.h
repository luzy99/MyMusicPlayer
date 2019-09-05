#ifndef ANIMATEDWALLWG_H
#define ANIMATEDWALLWG_H

#include <QWidget>
#include <QLabel>
#include <QWidget>
#include <QVariant>
#include <QPushButton>

#define LRWIDTH     60     //左右两个点击按钮的宽度
#define SCALE       0.86   //旁边两个小Label占中间大Label的比例
#define COVER       60     //中间大Label挡住两边小Label的部分
#define DURATION    400    //动画持续时间
#define EASINGTIME  0.5    //中间缓冲时间
#define EASINGCURVE QEasingCurve::InOutCubic  //缓和曲线  InOutSine InOutQuart InOutCubic
#define fixedheight 300
#define fixedwidth 750

class AnimatedWallWG : public QWidget
{
    Q_OBJECT
public:
    explicit AnimatedWallWG(QWidget *parent = nullptr);

    //初始化
    void initVar();
    void initWallpaper();
    void initLRButton();
    void initIndexButton();
    void setWallRect();
    void setLRButtonRect();
    void setWallpaper(QLabel *pWall, int i);
    void wallIndex(bool b, int n, int &i, int &j, int &k, int &l);

    //本类实例
    static AnimatedWallWG* Instance(QWidget *parent = 0)
    {
        if (m_pInstance == NULL)
        {
            m_pInstance = new AnimatedWallWG(parent);
        }
        return m_pInstance;
    }

protected slots:
    void moveToLeft();
    void moveToRight();
    void onValueChanged(QVariant variant);
    void finished() { m_bAnimation = true; }
    void on_btnClicked0();
    void on_btnClicked1();
    void on_btnClicked2();
    void on_btnClicked3();
    void on_btnClicked4();
    void on_btnClicked5();

protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    static AnimatedWallWG *m_pInstance;

    //正中间图片在链表中的索引
    QLabel* m_pWall[6];
    QRect m_rtWall[3];      //没变化前的状态
    QRect m_rtLChange[5];   //从左到右变化过程中的状态
    QRect m_rtRChange[6];   //从右到左变化过程中的状态
    QWidget *m_pbtnwidget;
    QPushButton *m_pIndexButton[6];
    QPushButton *m_pLRButton[2];  //左右移动的两个点击按钮
    int m_cover;
    int m_iWall;
    int m_iRaise;
    int m_idTimer;
    bool m_bAnimation;


};

#endif // ANIMATEDWALLWG_H
