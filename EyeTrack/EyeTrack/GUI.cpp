#include "stdafx.h"
/** Constants **/

const int PointNum = 10;
Point px[PointNum];
bool LeftButton = false;
/**
* @function main
*/
int runGUI() {
	while (true)
	{
		int t;
		RandomPoints();
		while (true) {
			ifstream fin("result.txt");
			fin >> t;
			fin.close();
			//if (RandomPonitsShow(t)) break;
			Gui4(t);
		}
	}
	return 0;
}



void RandomPoints()
{
	Mat img = Mat::zeros(Size(1280, 720), CV_8UC3);

	img.setTo(255);
	//Point p1(100, 100);
	//Point p2(758, 50);
	//line(img, p1, p2, Scalar(0, 0, 255), 2);
	//line(img, Point(300, 300), Point(758, 400), Scalar(0, 255, 255), 3);

	//Point p(20, 20);//初始化点坐标为(20,20)
	//circle(img, p, 2, Scalar(0, 255, 0), -1);
	//srand((unsigned)time(NULL));

	int i;
	for (i = 0; i < PointNum; i++)
	{
		px[i].x = rand() % 1280;
		px[i].y = rand() % 720;
		//circle(img, px[i],12, Scalar(0, 255, 0), -1);
	}


	//imshow("RandomPoints", img);

	//waitKey();
}

int Gui16(int t)
{
	int i, j;
	t = t % 16;
	int a, b;
	a = t / 4;
	b = t % 4;
	Mat org = Mat::zeros(Size(1280, 720), CV_8UC3);
	org.setTo(0);


	for (i = 0; i < 4; i++)
		for (j = 0; j < 4; j++)
		{
			if (i == b && j == a)
				rectangle(org,
					Point(i * 320, j * 180),
					Point((i + 1) * 320, (j + 1) * 180),//长方形中两个相反的顶部
					Scalar(255, 255, 255),//BGR中的黄色
					-1,//这个长方形将被填充
					8);
			else
				rectangle(org,
					Point(i * 320, j * 180),
					Point((i + 1) * 320, (j + 1) * 180),//长方形中两个相反的顶部
					Scalar(0, 0, 0),//BGR中的黄色
					-1,//这个长方形将被填充
					8);
		}
	line(org, Point(0, 0), Point(1280, 0), Scalar(200, 200, 200), 6);
	line(org, Point(0, 180), Point(1280, 180), Scalar(200, 200, 200), 6);
	line(org, Point(0, 360), Point(1280, 360), Scalar(200, 200, 200), 6);
	line(org, Point(0, 540), Point(1280, 540), Scalar(200, 200, 200), 6);
	line(org, Point(0, 720), Point(1280, 720), Scalar(200, 200, 200), 6);
	line(org, Point(0, 0), Point(0, 720), Scalar(200, 200, 200), 6);
	line(org, Point(320, 0), Point(320, 720), Scalar(200, 200, 200), 6);
	line(org, Point(640, 0), Point(640, 720), Scalar(200, 200, 200), 6);
	line(org, Point(960, 0), Point(960, 720), Scalar(200, 200, 200), 6);
	line(org, Point(1280, 0), Point(1280, 720), Scalar(200, 200, 200), 6);
	Mat temp1;
	org.copyTo(temp1);//用来显示点的坐标以及临时的方框  
					  //namedWindow("FullScreen");//定义一个img窗口   
	cv::namedWindow("FullScreen1", CV_WINDOW_NORMAL);
	cv::setWindowProperty("FullScreen1", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow("FullScreen1", temp1);
	if (LeftButton) return 1;
	else return 0;
}

int Gui4(int t)
{
	int i, j;
	t = t % 4;
	int a, b;
	a = t / 2;
	b = t % 2;
	Mat org = Mat::zeros(Size(1280, 720), CV_8UC3);
	org.setTo(0);


	for (i = 0; i < 2; i++)
		for (j = 0; j < 2; j++)
		{
			if (i == b && j == a)
				rectangle(org,
					Point(i * 640, j * 360),
					Point((i + 1) * 640, (j + 1) * 360),//长方形中两个相反的顶部
					Scalar(255, 255, 255),//BGR中的黄色
					-1,//这个长方形将被填充
					8);
			else
				rectangle(org,
					Point(i * 640, j * 360),
					Point((i + 1) * 640, (j + 1) * 360),//长方形中两个相反的顶部
					Scalar(0, 0, 0),//BGR中的黄色
					-1,//这个长方形将被填充
					8);
		}
	line(org, Point(0, 0), Point(1280, 0), Scalar(200, 200, 200), 6);
	line(org, Point(0, 360), Point(1280, 360), Scalar(200, 200, 200), 6);
	line(org, Point(0, 720), Point(1280, 720), Scalar(200, 200, 200), 6);
	line(org, Point(0, 0), Point(0, 720), Scalar(200, 200, 200), 6);
	line(org, Point(640, 0), Point(640, 720), Scalar(200, 200, 200), 6);
	line(org, Point(1280, 0), Point(1280, 720), Scalar(200, 200, 200), 6);
	Mat temp1;
	org.copyTo(temp1);//用来显示点的坐标以及临时的方框  
					  //namedWindow("FullScreen");//定义一个img窗口   
	cv::namedWindow("FullScreen1", CV_WINDOW_NORMAL);
	cv::setWindowProperty("FullScreen1", CV_WND_PROP_FULLSCREEN, CV_WINDOW_FULLSCREEN);
	imshow("FullScreen1", temp1);
	waitKey(500);
	return 0;
}