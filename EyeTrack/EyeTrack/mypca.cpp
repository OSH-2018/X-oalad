#include "stdafx.h"
 
using namespace cv;
//using namespace std;

//mypca
Mat mypca(const Mat &_data)
{

    Mat Data =  cv::Mat_<double>(_data);  // Mat_ ???
    // 设有m条n维数据
 
    int rows = Data.rows;
    int cols = Data.cols;
 

    //计算均值
    //求取每列数据对应的均值存放在E矩阵中
    Mat E = Mat::zeros( 1, cols, Data.type() );
 
    for ( int j=0; j<cols; j++ )
    {
        for ( int i=0; i<rows; i++ )
        {
            E.at<double>(0,j) += Data.at<double>(i,j);
        }
    }
    
    E = E/rows; 
    
    

    //计算协方差矩阵
    // 数据矩阵的值减去对应列的均值
    Mat S =  Mat::zeros( rows, cols, Data.type() );
    for ( int i=0; i<rows; i++ )
    {
        for ( int j=0; j<cols; j++ )
        {
            S.at<double>(i,j) = Data.at<double>(i,j) - E.at<double>(0,j); 
        }
    }
    //计算协方差矩阵:   (S矩阵的转置 * S矩阵)/行数     n*m X m*n = n*n
    Mat Average = (S.t() * S) /(rows);
    
    Mat eigenvalues;
    Mat eigenvectors;

    //使用opencv提供的eigen函数求特征值以及特征向量
    eigen(Average, eigenvalues, eigenvectors);

    //P是前两个最大的特征值对应的特征向量  2*n
    Mat P = eigenvectors.rowRange(0,2).clone();

    //m*n  X  2*n.t()   =   m*2
    Mat pca_result = Data * P.t();
    //return m*2
    return pca_result;
}