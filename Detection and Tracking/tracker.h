//
//  tracker.h
//  Tracker
//
//  Created by 冷佳旭 on 15/12/17.
//  Copyright © 2015年 lengjiaxu. All rights reserved.
//

#ifndef tracker_h
#define tracker_h


#endif /* tracker_h */

#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class Tracker
{
public:
    Tracker()  {}
    virtual  ~Tracker() { }
    
    virtual void init(const cv::Rect &roi, cv::Mat image) = 0;
    virtual cv::Rect  update( cv::Mat image)=0;
    
    
protected:
    cv::Rect _roi;
};



