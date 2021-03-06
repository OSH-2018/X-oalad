#include "stdafx.h"
//#pragma comment(lib,"opencv_world341.lib")
//#pragma comment(lib,"opencv_world341d.lib")

using namespace cv;
using namespace std;

void zoomDown(string path = "./dataset/") {
	int n;
	fstream fin(path + "sampleSize.txt");
	fin >> n;
	for (int i = 0; i < n; i++) {
		string fullpath = path + string("eyedetect/left") + to_string(i) + ".bmp";
		string fullout = path + string("small/left") + to_string(i) + ".bmp";
		Mat img, out;

		img = imread(fullpath);
		out = resizeAndGray(img);
		imwrite(fullout,out);

		fullpath = path + string("eyedetect/right") + to_string(i) + ".bmp";
		fullout = path + string("small/right") + to_string(i) + ".bmp";
		img = imread(fullpath);
		out = resizeAndGray(img);
		imwrite(fullout, out);
	}
}

//去掉以上两行代码注释之后就可以不用在下面的代码中加上cv::和std::
//cv::为OpenCV的函数库
//std::为C++的标准函数库
//
//Mat DetectEye(Mat src) {
//	cv::Mat gray, equalize, dst;
//	if (src.empty()) {
//		throw("Faild open file.");
//	}
//
//	cvtColor(src, gray, cv::COLOR_RGB2GRAY);//颜色空间转换RGB<->GREY
//	equalizeHist(gray, equalize); //直方图均衡化，该函数能归一化图像亮度和增强对比度
//	//cv::CascadeClassifier objDetector(".\\etc\\haarcascades\\haarcascade_eye.xml");
//	cv::CascadeClassifier objDetector("./etc/haarcascades/haarcascade_frontalface_alt.xml");
//	//和脸部识别一样，就是特征文件变化了
//	//这个函数说明分类器的种类
//	std::vector<cv::Rect> objs;
//	objDetector.detectMultiScale(equalize, objs, 1.2, 2, CV_HAAR_SCALE_IMAGE, cv::Size(200, 200));
//	if (!objs.empty())
//	{
//		objs[0].y += objs[0].height / 4;
//		objs[0].height /= 3;
//		objs[0].width /= 2;
//		return src(objs[0]);
//	}
//	return Mat();
//}
//
////int main()
////{
////	//以下的方法如果想运行，直接把前头的注释去掉即可
////	//运行代码的时候请改掉filename里的地址
////	//我的opencv是安装在C盘目录下
////	//一些目录地址请自行修改一下
////	//ldeng:所以要使用相对路径而不是绝对路径
////	bool stop = false;
////	Mat frame;
////	//DetectEye();//图像眼睛识别
////	VideoCapture cap(0);
////	if (!cap.isOpened()) {
////		printf("cap open failed\n");
////	}
////
////	//cap >> frame;
////
////	int start = clock();
////
////	int i = 0;
////	while (!stop) {
////		cap >> frame;
////		Mat face = DetectEye(frame);
////		if (!face.empty()) {
////			imshow("winFace", face);
////			waitKey();
////		}
////		i++;
////		if (i % 100 == 0)
////			cout << i << ": " << clock() - start << endl;
////	}
////	return 0;
////}
//
//#include <opencv2/objdetect/objdetect.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv2/imgproc/imgproc.hpp>
//
//#include <iostream>
//#include <queue>
//#include <stdio.h>
//#include <math.h>
//
//#include "constants.h"
//#include "findEyeCenter.h"
//#include "findEyeCorner.h"
//
//
/** Constants **/


/** Function Headers */
void detectAndDisplay(cv::Mat frame);

/** Global variables */
//-- Note, either copy these two files from opencv/data/haarscascades to your current folder, or change these locations
cv::String face_cascade_name = "./etc/haarcascades/haarcascade_frontalface_alt.xml";
cv::CascadeClassifier face_cascade;
std::string main_window_name = "Capture - Face detection";
std::string face_window_name = "Capture - Face";
cv::RNG rng(12345);
cv::Mat debugImage;
cv::Mat skinCrCbHist = cv::Mat::zeros(cv::Size(256, 256), CV_8UC1);

/**
* @function main
*/
int getcentre() {
	//Mat a = imread("0.bmp");
	//Mat b = resizeAndGray(a,300,300);
	//imwrite("1.bmp", b);
	//return 0;


	cv::Mat frame;

	// Load the cascades
	if (!face_cascade.load(face_cascade_name)) { printf("--(!)Error loading face cascade, please change face_cascade_name in source code.\n"); return -1; };

	cv::namedWindow(main_window_name, CV_WINDOW_NORMAL);
	cv::moveWindow(main_window_name, 400, 100);
	cv::namedWindow(face_window_name, CV_WINDOW_NORMAL);
	cv::moveWindow(face_window_name, 10, 100);
	cv::namedWindow("Right Eye", CV_WINDOW_NORMAL);
	cv::moveWindow("Right Eye", 10, 600);
	cv::namedWindow("Left Eye", CV_WINDOW_NORMAL);
	cv::moveWindow("Left Eye", 10, 800);
	cv::namedWindow("aa", CV_WINDOW_NORMAL);
	cv::moveWindow("aa", 10, 800);
	cv::namedWindow("aaa", CV_WINDOW_NORMAL);
	cv::moveWindow("aaa", 10, 800);

	createCornerKernels();
	ellipse(skinCrCbHist, cv::Point(113, 155.6), cv::Size(23.4, 15.2),
		43.0, 0.0, 360.0, cv::Scalar(255, 255, 255), -1);

	// I make an attempt at supporting both 2.x and 3.x OpenCV
#if CV_MAJOR_VERSION < 3
	CvCapture* capture = cvCaptureFromCAM(0);
	if (capture) {
		while (true) {
			frame = cvQueryFrame(capture);
#else
	cv::VideoCapture capture(0);
	if (capture.isOpened()) {
		while (true) {
			capture.read(frame);
#endif
			// mirror it
			cv::flip(frame, frame, 1);
			frame.copyTo(debugImage);

			// Apply the classifier to the frame
			if (!frame.empty()) {
				detectAndDisplay(frame);
			}
			else {
				printf(" --(!) No captured frame -- Break!");
				break;
			}

			imshow(main_window_name, debugImage);

			int c = cv::waitKey(10);
			if ((char)c == 'c') { break; }
			if ((char)c == 'f') {
				imwrite("frame.png", frame);
			}

		}
	}

	releaseCornerKernels();

	return 0;
		}

void findEyes(cv::Mat frame_gray, cv::Rect face) {
	cv::Mat faceROI = frame_gray(face);
	cv::Mat debugFace = faceROI;

	if (kSmoothFaceImage) {
		double sigma = kSmoothFaceFactor * face.width;
		GaussianBlur(faceROI, faceROI, cv::Size(0, 0), sigma);
	}
	//-- Find eye regions and draw them
	int eye_region_width = face.width * (kEyePercentWidth / 100.0);
	int eye_region_height = face.width * (kEyePercentHeight / 100.0);
	int eye_region_top = face.height * (kEyePercentTop / 100.0);
	cv::Rect leftEyeRegion(face.width*(kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);
	cv::Rect rightEyeRegion(face.width - eye_region_width - face.width*(kEyePercentSide / 100.0),
		eye_region_top, eye_region_width, eye_region_height);

	//-- Find Eye Centers
	cv::Point leftPupil = findEyeCenter(faceROI, leftEyeRegion, "Left Eye");
	cv::Point rightPupil = findEyeCenter(faceROI, rightEyeRegion, "Right Eye");
	// get corner regions
	cv::Rect leftRightCornerRegion(leftEyeRegion);
	leftRightCornerRegion.width -= leftPupil.x;
	leftRightCornerRegion.x += leftPupil.x;
	leftRightCornerRegion.height /= 2;
	leftRightCornerRegion.y += leftRightCornerRegion.height / 2;
	cv::Rect leftLeftCornerRegion(leftEyeRegion);
	leftLeftCornerRegion.width = leftPupil.x;
	leftLeftCornerRegion.height /= 2;
	leftLeftCornerRegion.y += leftLeftCornerRegion.height / 2;
	cv::Rect rightLeftCornerRegion(rightEyeRegion);
	rightLeftCornerRegion.width = rightPupil.x;
	rightLeftCornerRegion.height /= 2;
	rightLeftCornerRegion.y += rightLeftCornerRegion.height / 2;
	cv::Rect rightRightCornerRegion(rightEyeRegion);
	rightRightCornerRegion.width -= rightPupil.x;
	rightRightCornerRegion.x += rightPupil.x;
	rightRightCornerRegion.height /= 2;
	rightRightCornerRegion.y += rightRightCornerRegion.height / 2;
	rectangle(debugFace, leftRightCornerRegion, 200);
	rectangle(debugFace, leftLeftCornerRegion, 200);
	rectangle(debugFace, rightLeftCornerRegion, 200);
	rectangle(debugFace, rightRightCornerRegion, 200);
	// change eye centers to face coordinates
	rightPupil.x += rightEyeRegion.x;
	rightPupil.y += rightEyeRegion.y;
	leftPupil.x += leftEyeRegion.x;
	leftPupil.y += leftEyeRegion.y;
	// draw eye centers
	circle(debugFace, rightPupil, 3, 1234);
	circle(debugFace, leftPupil, 3, 1234);

	//-- Find Eye Corners
	if (kEnableEyeCorner) {
		cv::Point2f leftRightCorner = findEyeCorner(faceROI(leftRightCornerRegion), true, false);
		leftRightCorner.x += leftRightCornerRegion.x;
		leftRightCorner.y += leftRightCornerRegion.y;
		cv::Point2f leftLeftCorner = findEyeCorner(faceROI(leftLeftCornerRegion), true, true);
		leftLeftCorner.x += leftLeftCornerRegion.x;
		leftLeftCorner.y += leftLeftCornerRegion.y;
		cv::Point2f rightLeftCorner = findEyeCorner(faceROI(rightLeftCornerRegion), false, true);
		rightLeftCorner.x += rightLeftCornerRegion.x;
		rightLeftCorner.y += rightLeftCornerRegion.y;
		cv::Point2f rightRightCorner = findEyeCorner(faceROI(rightRightCornerRegion), false, false);
		rightRightCorner.x += rightRightCornerRegion.x;
		rightRightCorner.y += rightRightCornerRegion.y;
		circle(faceROI, leftRightCorner, 3, 200);
		circle(faceROI, leftLeftCorner, 3, 200);
		circle(faceROI, rightLeftCorner, 3, 200);
		circle(faceROI, rightRightCorner, 3, 200);
	}

	imshow(face_window_name, faceROI);
	//  cv::Rect roi( cv::Point( 0, 0 ), faceROI.size());
	//  cv::Mat destinationROI = debugImage( roi );
	//  faceROI.copyTo( destinationROI );
}


cv::Mat findSkin(cv::Mat &frame) {
	cv::Mat input;
	cv::Mat output = cv::Mat(frame.rows, frame.cols, CV_8U);

	cvtColor(frame, input, CV_BGR2YCrCb);

	for (int y = 0; y < input.rows; ++y) {
		const cv::Vec3b *Mr = input.ptr<cv::Vec3b>(y);
		//    uchar *Or = output.ptr<uchar>(y);
		cv::Vec3b *Or = frame.ptr<cv::Vec3b>(y);
		for (int x = 0; x < input.cols; ++x) {
			cv::Vec3b ycrcb = Mr[x];
			//      Or[x] = (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) > 0) ? 255 : 0;
			if (skinCrCbHist.at<uchar>(ycrcb[1], ycrcb[2]) == 0) {
				Or[x] = cv::Vec3b(0, 0, 0);
			}
		}
	}
	return output;
}

/**
* @function detectAndDisplay
*/
void detectAndDisplay(cv::Mat frame) {
	std::vector<cv::Rect> faces;
	//cv::Mat frame_gray;

	std::vector<cv::Mat> rgbChannels(3);
	cv::split(frame, rgbChannels);
	cv::Mat frame_gray = rgbChannels[2];

	//cvtColor( frame, frame_gray, CV_BGR2GRAY );
	//equalizeHist( frame_gray, frame_gray );
	//cv::pow(frame_gray, CV_64F, frame_gray);
	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE | CV_HAAR_FIND_BIGGEST_OBJECT, cv::Size(150, 150));
	//  findSkin(debugImage);

	for (int i = 0; i < faces.size(); i++)
	{
		rectangle(debugImage, faces[i], 1234);
	}
	//-- Show what you got
	if (faces.size() > 0) {
		findEyes(frame_gray, faces[0]);
	}
}


int main() {

	while (true) {
		cout << "$ EyeTrack >>";
		cout.flush();
		string str;
		getline(cin, str);

		if (str == "zoomDown") {
			zoomDown();
		}
		else if (str == "show") {
			getcentre();
		}
		else if (str == "collect") {
			system("runDataCollection.bat");
		}
		else if (str == "process") {
			system("runEyeDetect");
			cout << "detect OK" << endl;
			system("runPupilLocation");
			cout << "locate OK" << endl;
		}
		if (str.find("run") != str.npos) {			//run something
			system("getVideo");
			if (str.find("knn") != str.npos) {
				cout << "UNIMPLENMENT" << endl;
			}
			else if (str.find("svm") != str.npos) {
				cout << "UNIMPLENMENT" << endl;
			}
			else if (str.find("fnn") != str.npos) {
				system("runFNN");
			}
			else if (str.find("linear") != str.npos) {
				system("runLinearRegression");
			}
			else {
				cout << "UNKOWN MODEL!" << endl;
			}
			runGUI();
		}
		else if (str.find("train") != str.npos) {	//train something
			system("getVideo");
			if (str.find("knn") != str.npos) {
				cout << "UNIMPLENMENT" << endl;
			}
			else if (str.find("svm") != str.npos) {
				svmTraining();
			}
			else if (str.find("fnn") != str.npos) {
				system("trainFNN");
			}
			else {
				cout << "UNKOWN MODEL!" << endl;
			}
		}
		else if (str == "q" || str=="quit") {
			return 0;
		}
		
	}
	return 0;
}
