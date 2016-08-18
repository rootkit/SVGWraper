//
//  MouseCapture.h
//  grabcut
//
//  Created by xyz on 16/8/17.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __grabcut__MouseCapture__
#define __grabcut__MouseCapture__

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class MouseCapture {
public:
    MouseCapture(Mat &img);
    ~MouseCapture();
    void adjustPoints();
    Rect getRect();
    static void onMouse(int event, int x, int y, int flag, void *param);
private:
    bool check(int x, int y);
    void refreshImage(Mat &tempImg);
    void drawRect(Mat &tempImg, int x1, int y1, int x2, int y2);
private:
    Mat img;
    string winName;
    bool marked;
    bool finished;
    int moveX, moveY;
    Point p1, p2;
};

#endif /* defined(__grabcut__MouseCapture__) */
