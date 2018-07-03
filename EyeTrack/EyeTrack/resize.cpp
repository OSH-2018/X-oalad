#include "stdafx.h"
using namespace cv;
using namespace std;

Mat resizeAndGray(const Mat img, int width, int height) {
	Mat gray,equalize,small;
	cvtColor(img, gray, cv::COLOR_RGB2GRAY);//颜色空间转换RGB<->GREY
	equalizeHist(gray, equalize); //直方图均衡化，该函数能归一化图像亮度和增强对比度
	resize(equalize, small, Size(width, height));
	return small;
}