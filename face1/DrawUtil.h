
//
//  DrawUtil.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__DrawUtil__
#define __face1__DrawUtil__

#include "Triangular.h"
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class DrawUtil {
public:
    static bool is_valid_in(Mat &img, int x, int y);
    static void draw_point(Mat &img, int x, int y, int r);
    static void draw_point(Mat &img, int x, int y, int r, int channel);
    static void draw_points(Mat &img, vector<Point> &points, int r);
    static void draw_points(Mat &img, vector<Point> &points, int r, int channel);
    static void draw_triangular(Mat &img, Triangular &t, Point translate);
};

#endif /* defined(__face1__DrawUtil__) */
