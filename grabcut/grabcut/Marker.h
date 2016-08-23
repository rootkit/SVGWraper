//
//  Marker.h
//  grabcut
//
//  Created by xyz on 16/8/23.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __grabcut__Marker__
#define __grabcut__Marker__

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class Marker {
public:
    Mat mask;
    Mat img;
public:
    Marker(Mat mask, Mat img);
    static void onMouse(int event, int x, int y, int flag, void *param);
    void adjustPoints();
};

#endif /* defined(__grabcut__Marker__) */
