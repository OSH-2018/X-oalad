﻿// stdafx.cpp : 只包括标准包含文件的源文件
// EyeTrack.pch 将作为预编译标头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

// TODO: 在 STDAFX.H 中引用任何所需的附加头文件，
//而不是在此文件中引用
using namespace cv;
using namespace cv::ml
using namespace std;

#define ns 100;//ns: Number of Samples.
#define npix 18;//npix: Number of PIXels per picture;

/*README:
需要的训练数据及格式
1.文件data_training.txt:用于存储训练数据。每行一个sample,每列一个像素值（RGB向量），总共18列
2.文件data_labels.txt:用于存储数据相应的所属类别。每行一个label（或用空格隔开）

训练结果：
文件model_svm.xml,存储了训练好的模型

*/
int main()
{
	//读取label文件
	ifstream data_labels;
	data_labels.open("data_labels.txt", ios::in);
	float labels[ns];
	int i;
	float label;
	i = 0;
	while (!data_labels.eof()) {
		data_labels >> label;
		labels[i] = label;
		i++;
	}

	data_labels.close();

	// 读取数据文件
	ifstream data_training;
	data_training.open("data_training.txt", ios::in);
	float data[ns][npix];
	int i, j;
	i = j = 0;
	float pixel;
	while (!data_training.eof()) {
		for (int j = 0; j < npix; j++) {
			data_training >> pixel;
			data[i][j] = pixel;
		}
		i++;
	}

	data_training.close();

	

	// 配置训练参数
	Mat labelsMat(ns, 1, CV_32SC1, labels);
	Mat trainingDataMat(ns, npix, CV_32FC3, data);//note: 默认channel=3,即图片中每个像素点由RGB三个点组成

	Ptr<SVM> model = SVM::create();
	model->setType(SVM::C_SVC);
	model->setKernel(SVM::LINEAR);
	model->setGamma(1);
	model->setC(1);
	model->setCoef0(0);
	model->setNu(0);
	model->setP(0);
	model->setTermCriteria(TermCriteria(CV_TERMCRIT_ITER + CV_TERMCRIT_EPS, 2000, 0.0001));//TermCriteria(type,max_iter,epsilon)
	svm->train(trainingDataMat, ROW_SAMPLE, labels);

	//训练数据 
	Ptr<TrainData> tData = TrainData::create(trainingDataMat, ROW_SAMPLE, labelsMat);
	model->trainAuto(tData, 10);
	model->save("svm_model.xml");
