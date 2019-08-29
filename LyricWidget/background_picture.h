#include <opencv2/opencv.hpp>
using namespace cv;

int get_Background()
{
    namedWindow("[原图]腐蚀操作");
    namedWindow("高斯模糊1");
    namedWindow("高斯模糊2");

    //载入原图
    Mat srcImage = imread("C:/Users/Lenovo/Downloads/HZ1.png", IMREAD_COLOR);
    //显示原图
    imshow("[原图]腐蚀操作", srcImage);
    //进行腐蚀操作
    Mat element = getStructuringElement(MORPH_RECT, Size(15, 15));
    Mat dstImage;
    Mat dstImage1;
    int g_nGaussianBlurValue = 20;
    GaussianBlur(srcImage, dstImage, Size(g_nGaussianBlurValue * 2 + 1, g_nGaussianBlurValue * 2 + 1), 0, 0);
    GaussianBlur(srcImage, dstImage1, Size(40 * 2 + 1, 40 * 2 + 1), 0, 0);
    //erode(srcImage, dstImage, element);
    //显示效果图
    imshow("高斯模糊1",dstImage);
    imshow("高斯模糊2",dstImage1);
    waitKey(0);

    return 0;
}
