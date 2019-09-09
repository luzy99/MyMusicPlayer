#include "gesturecontrol.h"
#include <QDebug>

GestureControl::GestureControl(QObject *parent) : QObject(parent)
{
    frame = Mat::zeros(Size(640, 480), CV_8UC3);
    fgMaskMOG2 = Mat::zeros(frame.size(), CV_8UC3);
    maskContour = Mat::zeros(frame.size(), CV_8UC3);
    direction = 0;//方向：0为向左（起始点在右边），1为向右（起始点在左边）
    pMOG2 = createBackgroundSubtractorMOG2(300, 25, false);
    recordIndex =0;
}

void GestureControl::mainProcess()
{
    VideoCapture capture(0);

    keyboard = 0;
    while (keyboard != 'q')
    {
        //qDebug()<<"in while";
        if(m_time.elapsed()>5000)
        {
            //cout<<"reset!!!!!\n";
            recordIndex =0;
        }
        if (!capture.read(frame)) {
            //cerr << "Unable to read next frame." << endl;
            //cerr << "Exiting..." << endl;qq
            exit(EXIT_FAILURE);
        }
        flip(frame, frame, 1);

        //frame = YCrCb_Otsu_detect(frame);
        ////转换色彩通道 RGB to GRAY
        //cvtColor(frame, frame, COLOR_BGR2GRAY);
        ////转换为二值图，黑白
        //threshold(frame, frame, 50, 255, THRESH_BINARY);
        //cvtColor(frame, frame, COLOR_BGR2GRAY);

        pMOG2->apply(frame, fgMaskMOG2);

        //cv::imshow("Frame", frame);
        Mat element = getStructuringElement(MORPH_CROSS, Size(10, 5));
        //进行腐蚀操作
        erode(fgMaskMOG2, fgMaskMOG2, element);

        //cv::imshow("FG Mask MOG 2", fgMaskMOG2);

        Mat labels = Mat::zeros(fgMaskMOG2.size(), CV_8UC3);
        Mat stats = Mat::zeros(fgMaskMOG2.size(), CV_8UC3);
        Mat centroids = Mat::zeros(fgMaskMOG2.size(), CV_8UC3);
        Mat img_color = Mat::zeros(fgMaskMOG2.size(), CV_8UC3);
        int nccomps = connectedComponentsWithStats(
                    fgMaskMOG2, //二值图像
                    labels,     //和原图一样大的标记图
                    stats, //nccomps×5的矩阵 表示每个连通区域的外接矩形和面积（pixel）
                    centroids //nccomps×2的矩阵 表示每个连通区域的质心
                    );

        ////imshow("原图中连通区域数", fgMaskMOG2);

        //去除过小区域，初始化颜色表
        vector<cv::Vec3b> colors(nccomps);
        colors[0] = cv::Vec3b(0, 0, 0); // background pixels remain black.
        for (int i = 1; i < nccomps; i++) {
            colors[i] = cv::Vec3b(255, 255, 255);
            //去除面积小于100的连通域
            if (stats.at<int>(i, cv::CC_STAT_AREA) < 300)
                colors[i] = cv::Vec3b(0, 0, 0); // small regions are painted with black too.
        }
        //按照label值，对不同的连通域进行着色

        for (int y = 0; y < img_color.rows; y++)
            for (int x = 0; x < img_color.cols; x++)
            {
                int label = labels.at<int>(y, x);
                CV_Assert(0 <= label && label <= nccomps);
                img_color.at<cv::Vec3b>(y, x) = colors[label];
            }

    //获取自定义核
        element = getStructuringElement(MORPH_CROSS, Size(30, 30));
        Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(60, 60));
        //进行腐蚀/膨胀操作
        erode(img_color, img_color, element);
         dilate(img_color,img_color, element1);
        //cv::imshow("remove", img_color);
        cvtColor(img_color, img_color, COLOR_BGR2GRAY);
        threshold(img_color, img_color, 1, 255, cv::THRESH_BINARY);//二值化
        drawRect(img_color, frame);
        if(keyboard == 'q')
        {
            break;
        }
        waitKey(30);
    }
    capture.release();
//    cv::destroyWindow("FG Mask MOG 2");
//    cv::destroyWindow("remove");
//    cv::destroyWindow("trackObject");
}

void GestureControl::setKeyboard(char value)
{
    keyboard = value;
}

void GestureControl::drawRect(Mat MOG2, Mat frame)
{
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(MOG2, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
//CV_RETR_EXTERNAL

//    Mat dst = Mat::zeros(fgMaskMOG2.size(), CV_8UC3);
//    for( int i = 0; i< contours.size(); i++ )
//         {
//           Scalar color = Scalar(255,255,255);
//           drawContours( dst, contours, i, color, 15, 4, hierarchy, 0, Point());//用粗线条画轮廓（填充狭窄区域）
//         }
//    Rect ccomp;
//    floodFill(dst, Point(320,0), Scalar(255, 255,255), &ccomp);//将背景涂成白色
//    cv::imshow("contour", dst);
//    cvtColor(dst, dst, COLOR_BGR2GRAY);
//    threshold(dst, dst,255 , 1, cv::THRESH_BINARY);//二值化

//    findContours(dst, contours, hierarchy, CV_RETR_EXTERNAL, CHAIN_APPROX_SIMPLE, Point());
//    cv::imshow("new", dst);

    // 寻找最大连通域
    double maxArea = 0;
    vector<cv::Point> maxContour;
    if (contours.size() == 0)//无轮廓返回
    {
        //cv::imshow("trackObject", frame);
        return;
    }
    double area = 0;//轮廓面积
    for (size_t i = 0; i < contours.size(); i++)
    {
        area = contourArea(contours[i]);
        if (area > maxArea)
        {
            maxArea = area;
            maxContour = contours[i];
        }
    }
    //cout << area << endl;
    if (area <= 1)//轮廓大小<=1返回
    {
        //cv::imshow("trackObject", frame);
        return;
    }
    RotatedRect rectPoint = minAreaRect(maxContour);
    Point2f fourPoint2f[4];
    rectPoint.points(fourPoint2f);
    for (int i = 0;i < 3;i++)
    {
        line(frame, fourPoint2f[i], fourPoint2f[i + 1], Scalar(0, 0, 255), 2, LINE_AA);
    }
    line(frame, fourPoint2f[0], fourPoint2f[3]
            , Scalar(0, 0, 255), 2, LINE_AA);
    Point center;//矩形中心点

    //记录中心运动轨迹
    center = Point2i(rectPoint.center.x, rectPoint.center.y);
    //cout <<"********"<< recordIndex<<endl;

    //列表重置时
    if (recordIndex == 0)
    {
        m_time.restart();
        if (center.x >= 250 && center.x <= 390)//起始坐标不能在屏幕中央
        {
            //cv::imshow("trackObject", frame);
            return;
        }
        else if (center.x > 390)
        {
            direction = 0;//从右开始
        }
        else
        {
            direction = 1;//从左开始
        }
        circle(frame, center, 5, Scalar(0, 255, 0), -1);
        trackCenter.insert(trackCenter.begin() + recordIndex, center);
        recordIndex++;
        //cout << recordIndex;
        //cout << center << endl;
        //cv::imshow("trackObject", frame);
        return;
    }

    if (direction == 0)//起始点在右边
    {
        if (center.x < 120)
        {
            if (recordIndex < 4)
            {
                recordIndex = 0;
                //cv::imshow("trackObject", frame);
                return;
            }
            vector<Point> in_point;
            in_point.insert(in_point.begin(), trackCenter.rbegin(), trackCenter.rbegin() + recordIndex);

            //fittedCurve(in_point, 5);
            recordIndex = 0;
            //cout<<"left\n";
            emit gestureLeft();
            return;
        }
        circle(frame, center, 5, Scalar(0, 255, 0), -1);
        trackCenter.insert(trackCenter.begin() + recordIndex, center);
        recordIndex++;
        //cout << recordIndex << endl;
        //cout << center;

    }
    else if (direction == 1)//起始点在左边
    {
        if (center.x > 520)
        {
            if (recordIndex < 5||recordIndex>90)//记录点少于5或多余90返回
            {
                recordIndex = 0;
                //cv::imshow("trackObject", frame);
                return;
            }
            vector<Point> in_point;
            in_point.insert(in_point.begin(), trackCenter.begin(), trackCenter.begin() + recordIndex);
            //reserve(recordIndex);
            //fittedCurve(in_point, 5);
            //cout<<"right\n";
            emit gestureRight();
            recordIndex = 0;
            return;
        }
        circle(frame, center, 5, Scalar(0, 255, 0), -1);
        trackCenter.insert(trackCenter.begin() + recordIndex, center);
        recordIndex++;
        //cout << recordIndex << endl;
        //cout << center;

    }
    //cv::imshow("trackObject", frame);
}

Mat GestureControl::polyfit(vector<Point> &in_point, int n)
{
    int size = in_point.size();
    //所求未知数个数
    int x_num = n + 1;
    //构造矩阵U和Y
    Mat mat_u(size, x_num, CV_64F);
    Mat mat_y(size, 1, CV_64F);

    for (int i = 0; i < mat_u.rows; ++i)
        for (int j = 0; j < mat_u.cols; ++j)
        {
            mat_u.at<double>(i, j) = pow(in_point[i].x, j);
        }

    for (int i = 0; i < mat_y.rows; ++i)
    {
        mat_y.at<double>(i, 0) = in_point[i].y;
    }

    //矩阵运算，获得系数矩阵K
    Mat mat_k(x_num, 1, CV_64F);
    mat_k = (mat_u.t()*mat_u).inv()*mat_u.t()*mat_y;
    //cout << mat_k << endl;
    return mat_k;
}

void GestureControl::fittedCurve(vector<Point> &in_point, int n)
{
    //数据输入
    //n:多项式阶次

    Mat mat_k = polyfit(in_point, n);

    //计算结果可视化
    Mat out=Mat::zeros(frame.size(), CV_8UC3);

    //画出拟合曲线
    for (int i = in_point[0].x; i < in_point.back().x; ++i)
    {
        Point2d ipt;
        ipt.x = i;
        ipt.y = 0;
        for (int j = 0; j < n + 1; ++j)
        {
            ipt.y += mat_k.at<double>(j, 0)*pow(i, j);
        }
        circle(out, ipt, 1, Scalar(255, 255, 255), -1, LINE_AA);
        //CV_FILLED, CV_AA
    }

    //画出原始散点
    for (int i = 0; i < in_point.capacity(); ++i)
    {
        Point ipt = in_point[i];
        circle(out, ipt, 3, Scalar(0, 0, 255), -1, LINE_AA);
    }
    string str = std::to_string(n) + "times";
    //cv::imshow(str, out);
    //waitKey(0);
}
