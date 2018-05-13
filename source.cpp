#include "stdafx.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#pragma comment(lib,"opencv_world341.lib")

using namespace cv;
using namespace std;
//去掉以上两行代码注释之后就可以不用在下面的代码中加上cv::和std::
//cv::为OpenCV的函数库
//std::为C++的标准函数库

int DetectEye(Mat src) {
	cv::Mat gray, equalize, dst;
	//int x[]
	//const char* filename = "1.jpg";
		//cv::imread(filename).copyTo(src);//复制到src中
	if (src.empty()) {
		throw("Faild open file.");
	}

	cvtColor(src, gray, cv::COLOR_RGB2GRAY);//颜色空间转换RGB<->GREY
	equalizeHist(gray, equalize); //直方图均衡化，该函数能归一化图像亮度和增强对比度

	//cv::CascadeClassifier objDetector("D:\\program files\\opencv\\build\\etc\\haarcascades\\haarcascade_eye.xml");
	cv::CascadeClassifier objDetector("D:\\program files\\opencv\\build\\etc\\haarcascades\\haarcascade_frontalface_alt.xml");
	//和脸部识别一样，就是特征文件变化了
	//这个函数说明分类器的种类
	std::vector<cv::Rect> objs;
	objDetector.detectMultiScale(equalize, objs, 1.2, 2, CV_HAAR_SCALE_IMAGE, cv::Size(30, 30));

	src.copyTo(dst);
	std::vector<cv::Rect>::const_iterator it = objs.begin();
	for (it =objs.begin() ; it != objs.end(); ++it) {
		rectangle(dst, cv::Point(it->x, it->y),
			cv::Point(it->x + it->width, it->y + it->height),
			cv::Scalar(0, 0, 255), 2, CV_AA);
		printf("it=%d\n", it);
		printf("x=%d\ny=%d\nwidth=%d\nheight=%d\n", it->x, it->y, it->width, it->height);
	}
	cv::imshow("src", src);
	cv::imshow("dst", dst);
		//IplImage* pic = cvLoadImage("1.jpg");
		IplImage temp = IplImage(src);
		IplImage* pic = &temp;
		printf("sfesf");
		//int x = 100;
		//int y = 100;
		//int width = 200;
		//int height = 200;
		//CvScalar s = cvScalar(0, 150, 100);
		for (it = objs.begin(); it != objs.end(); it++) {
			//it--;
			printf("x=%d\ny=%d\nwidth=%d\nheight=%d\n", it->x, it->y, it->width, it->height);
			cvSetImageROI(pic, cvRect(it->x, it->y, it->width, it->height));
			IplImage* Dst = cvCreateImage(
				cvSize(it->width, it->height),
				pic->depth,
				pic->nChannels
			);
			cvCopy(pic, Dst, NULL);
			/*cvAddS(pic,s,pic);*/
			cvResetImageROI(pic);
			cvNamedWindow("test");
			cvShowImage("test", Dst);

			//cv::waitKey();
		}
}

int main()
{
	//以下的方法如果想运行，直接把前头的注释去掉即可
	//运行代码的时候请改掉filename里的地址
	//我的opencv是安装在C盘目录下
	//一些目录地址请自行修改一下
	bool stop = false;
	Mat frame;
	//DetectEye();//图像眼睛识别
	VideoCapture cap(0);
	if (!cap.isOpened()) {
		printf("cap open failed\n");
	}
	while (!stop) {
		cap >> frame;
		DetectEye(frame);
		if (waitKey(30) >= 0) {
			stop = true;
		}
	}
	return 0;
}