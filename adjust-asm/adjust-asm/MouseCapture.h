//
//  MouseCapture.h
//  adjust-asm
//
//  Created by xyz on 16/8/6.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __adjust_asm__MouseCapture__
#define __adjust_asm__MouseCapture__

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "FaceSpline.h"

class MouseCapture {
public:
    MouseCapture(Mat &img, vector<Point> points);
    MouseCapture(string file, vector<Point> points);
    ~MouseCapture();
    void loadImg(Mat &img);
    void loadImg(string file);
    void adjustPoints();
    vector<Point> getAdjustedPoints();
    static void onMouse(int event, int x, int y, int flag, void *param);
private:
    void checkInRange(int x, int y);
    void refreshPoints(int t);
    void refreshImage(Mat &tempImg);
private:
    FaceSpline* faceSpline;
    Mat img;
    string winName;
    vector<Point> points;
    vector<Scalar> colors;
    int marked;
    bool splineError;
    
    const static int POINT_NUMS = 16;
    const static Scalar SELECTED_COLOR;
    const static Scalar UNSELECTED_COLOR;
    
};

#endif /* defined(__adjust_asm__MouseCapture__) */
