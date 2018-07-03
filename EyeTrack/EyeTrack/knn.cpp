#include "stdafx.h"

using namespace cv;
using namespace std;


void KNNtraining(string path) {
	ifstream fin(path + "sampleSize.txt");
	int sampleSize = 2;
	fin >> sampleSize;
	cv::Mat train_data(sampleSize, 2, CV_8UC1);
	cv::Mat train_labels(sampleSize, 2, CV_8UC1);
	cout << train_data << endl;


	Ptr<ml::KNearest> knn = ml::KNearest::create();
	knn->setAlgorithmType(ml::KNearest::KDTREE);
	knn->train(train_data, cv::ml::ROW_SAMPLE, train_labels);
}