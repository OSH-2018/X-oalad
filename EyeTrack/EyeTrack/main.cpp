//
//  main.cpp
//  my_opencv01
//
//  Created by Ziwei Wu on 18/03/2018.
//  Copyright © 2018 Ziwei Wu. All rights reserved.
//

#include <iostream>
using namespace std;
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#pragma comment(lib,"opencv_world341d.lib")

using namespace cv;

void detectEye(Mat frame);

String face_cascade_name = "./haarcascade_frontalface_alt.xml";
CascadeClassifier face_cascade;

String eyes_cascade_name = "./haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier eyes_cascade;

int count_1;

int main(int argc, const char * argv[]) {
    VideoCapture cam(0);
    count_1 = 0;
    //-- 1. Load the cascades
    if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
    if( !eyes_cascade.load( eyes_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };

    while(1){
        Mat frame;
        //printf("Frame# %d", frame_num);
        cam >> frame;
        if(!frame.empty()){
            detectEye(frame);
        }
        else{
            printf("Done.\n");
            break;
        }
        //frame_num ++;
        //imshow("camtest", frame);
        if((char)waitKey(10) == 'q') break;
    }
    return 0;
}

void detectEye(Mat frame){
    std::vector<Rect> faces;
    Mat frame_gray;
    count_1 ++;
    if(count_1%1==0){
        cvtColor( frame, frame_gray, CV_BGR2GRAY );
        equalizeHist( frame_gray, frame_gray );
        
        //-- Detect faces
        if(face_cascade.empty())
            printf("empty model.");
        face_cascade.detectMultiScale( frame_gray, faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );
        
        for( size_t i = 0; i < faces.size(); i++ )
        {
            Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
            //ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
            
            Mat faceROI = frame_gray( faces[i] );
            std::vector<Rect> eyes;
            
            //-- In each face, detect eyes
            eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, Size(30, 30) );
            
            for( size_t j = 0; j < eyes.size(); j++ )
            {
                Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
                int radius = cvRound( (eyes[j].width + eyes[j].height)*0.25 );
                circle( frame, center, radius/2, Scalar( 255, 0, 0 ), 4, 8, 0 );
                circle( frame, center, radius/5, Scalar( 0, 0, 255 ), -1, 8, 0 );
            }
        }
        count_1 = 0;
    }
    //-- Show what you got
    imshow( "camTest", frame );
}
