#ifndef GESTURECONTROL_H
#define GESTURECONTROL_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include<QTime>
using namespace cv;
using namespace std;

class GestureControl : public QObject
{
    Q_OBJECT
public:
    explicit GestureControl(QObject *parent = nullptr);
    void mainProcess();//主函数（只要调这个）
    void setKeyboard(char value);

signals:
    void gestureLeft();
    void gestureRight();

public slots:

private:
    Mat frame;
    Mat fgMaskMOG2;
    Mat maskContour;
    Ptr<BackgroundSubtractor> pMOG2;
    bool direction;//方向：0为向左（起始点在右边），1为向右（起始点在左边）
    char keyboard;
    vector<Point> trackCenter;//追踪中心轨迹
    int recordIndex = 0;//轨迹下标
    QTime m_time;

    void drawRect(Mat MOG2, Mat frame);//绘制矩形框
    Mat polyfit(vector<Point>& in_point, int n);//计算系数矩阵 n为阶数
    void fittedCurve(vector<Point> &in_point, int n);//拟合曲线 n为阶数
};

#endif // GESTURECONTROL_H
