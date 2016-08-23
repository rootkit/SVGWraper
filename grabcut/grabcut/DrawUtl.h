//
//  DrawUtl.h
//  grabcut
//
//  Created by xyz on 16/8/19.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __grabcut__DrawUtl__
#define __grabcut__DrawUtl__

#include <stdio.h>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class DrawUtil {
public:
    static bool is_valid_in(Mat &img, int x, int y);
    static void draw_point(Mat &img, int x, int y, int r);
    static void draw_point(Mat &img, int x, int y, int r, Scalar color);
    static void draw_points(Mat &img, vector<Point> &points, int r);
    static void draw_points(Mat &img, vector<Point> &points, int r, Scalar color);
};

#endif /* defined(__grabcut__DrawUtl__) */
