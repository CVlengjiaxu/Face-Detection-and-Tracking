#include <stdlib.h>
#include <stdio.h>
#include "opencv2/core/core.hpp"
//#include "opencv2/imgproc/imgproc.hpp"

using namespace std;
using namespace cv;



int  initDetector(string faceDetector);

int detectFace(Mat frame,vector<Rect>& faces);//faces中为检测到的人脸
