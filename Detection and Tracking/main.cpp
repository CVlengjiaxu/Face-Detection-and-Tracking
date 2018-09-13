//
//  main.cpp
//  Tracker
//
//  Created by 冷佳旭 on 15/12/17.
//  Copyright © 2015年 lengjiaxu. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "kcftracker.h"
#include "FaceDetectionLib.h"

using namespace std;
using namespace cv;

Rect box;
bool drawing_box = false;
bool drawing = false;

void mouseHandler(int event, int x, int y, int flags, void *param){
    switch( event ){
            //  case CV_EVENT_LBUTTONUP:  a=true;
        case CV_EVENT_MOUSEMOVE:
            if (drawing_box){
                box.width = x-box.x;
                box.height = y-box.y;
            }
            break;
        case CV_EVENT_LBUTTONDOWN:
            drawing_box = true;
            box = Rect( x, y, 0, 0 );
            break;
        case CV_EVENT_LBUTTONUP:
            drawing_box = false;
            if( box.width < 0 ){
                box.x += box.width;
                box.width *= -1;
            }
            if( box.height < 0 ){
                box.y += box.height;
                box.height *= -1;
            }
            drawing = true;
            break;
    }
}

int main(int argc, char* argv[]){
    
    bool HOG = true;
    bool FIXEDWINDOW = false;
    bool MULTISCALE = true;
    
    // Create KCFTracker object
    KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE);
    Mat frame;
    
    /*
    initDetector("faceDetectModel");
    VideoCapture capture(0);
    cvNamedWindow("Tracker", CV_WINDOW_AUTOSIZE);
    vector<Rect> faces;
    while(1)
    {
        capture>>frame;
        if(frame.empty())
            return -1;
        detectFace(frame, faces);
        if (faces.size() > 0 )
        {
            box = faces[0];
            //expand
            int w ,h;
            Point center = Point(int(box.x + box.width/2), int(box.y + box.height/2));
            if(box.width<40)
            {
                w = box.width*2;
                h = box.height*2;
            }
            
            w = box.width*1.4;
            h = box.height*1.4;
            int x = center.x - w/2;
            int y = center.y - h/2;
            box = Rect(x,y,w,h);
            break;
        }
        rectangle(frame, box, Scalar(0,0,255), 2);
        imshow("Tracker",frame);
        waitKey(20);
    }
    */
  
    VideoCapture capture(0);
    cvNamedWindow("Tracker", 1);
    capture>>frame;
    flip(frame, frame, 1);
    //imshow("Tracker", frame);
    cvSetMouseCallback("Tracker", mouseHandler,0);
    while(!drawing)
    {
         capture >> frame;
         resize(frame, frame, Size(640, 480));
         flip(frame, frame, 1);
         rectangle(frame,box,Scalar(255,0,0),2);
         imshow("Tracker", frame);
         if (cvWaitKey(20) == 27)
             return 1;
     }
    //box=Rect(200,100,200,200);
    vector<Point> objCenters; //store centers of tracked target
    Rect result;
    int nFrames = 0;
    while (1)
    {
        double t4 = (double)cvGetTickCount();
        capture>>frame;
        resize(frame, frame, Size(640, 480));
        flip(frame, frame, 1);
        Mat showPath = Mat(frame.rows, frame.cols, frame.type(), Scalar(0, 0, 0));
        if (nFrames == 0)
        {
           // cout<<"—°∂®ƒø±Í¥Û–°£∫"<<box.size()<<endl;
            tracker.init(box, frame );
            rectangle( frame, box, Scalar( 0, 255, 255 ), 1, 8);
        }
        // Update
        else{
            result = tracker.update(frame);
            rectangle( frame, Point( result.x, result.y ), Point( result.x+result.width, result.y+result.height), Scalar( 0, 255, 255 ), 2, 8 );
            //putText(frame, "first", Point( result.x-5, result.y-5 ), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(0,255,0), 2);
            
            //show the path of tracking object
            objCenters.push_back(Point(result.x + result.width/2 , result.y + result.height/2));
            //tracker.drawingPath(frame, objCenters);
            for (int i = 0; i < objCenters.size()-1; i++)
            {
                //line(frame, objCenters[i], objCenters[i+1], Scalar(0, 255, 255));
                line(showPath, objCenters[i], objCenters[i+1], Scalar(0, 0, 255), 2, 8);
            }
            putText(showPath, "Start", objCenters[0], FONT_HERSHEY_TRIPLEX, 0.5, Scalar(0, 255, 0), 1);
            putText(showPath, "End", objCenters[objCenters.size()-1], FONT_HERSHEY_TRIPLEX, 0.5, Scalar(0, 255, 0), 1);
            if (waitKey(1) == 'p')
            {
                waitKey(-1);
            }
        }
       
        nFrames++;
        
        imshow("Tracker", frame);
        imshow("Path", showPath);
        /*char fileName[200] = "";
        sprintf(fileName, "../path-%d.jpg", nFrames);
        imwrite(fileName, showPath);*/
        if(waitKey(1) == 27)
            break;
        t4 = (double)cvGetTickCount() - t4;
        cout << "all cost time: " << t4 / ((double)cvGetTickFrequency()*1000.) << endl;
    }
    
}
