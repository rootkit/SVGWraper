//
//  DrawUtil.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__DrawUtil__
#define __morphing__DrawUtil__

#include <stdio.h>

#include "Triangle.h"
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class DrawUtil {
public:
    static bool isValidIn(Mat &img, int x, int y);
    static void drawPoint(Mat &img, int x, int y, int r);
    static void drawPoint(Mat &img, int x, int y, int r, Scalar color);
    static void drawPoints(Mat &img, vector<Point> &points, int r);
    static void drawPoints(Mat &img, vector<Point> &points, int r, Scalar color);
    static void drawTriangle(Mat &img, Triangle &t, Point translate);
};

#endif /* defined(__morphing__DrawUtil__) */
