
//
//  BezierUtil.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__BezierUtil__
#define __face1__BezierUtil__

#include <stdio.h>
#include "DrawUtil.h"

#include <opencv2/opencv.hpp>
using namespace cv;

class BezierUtil {
public:
    static void draw_quad_bezier(Mat &img, Point p0, Point p1, Point p2);
    static void draw_cube_bezier(Mat &img, Point p0, Point p1, Point p2, Point p3);
    // points里面，除了贝塞尔曲线前后的端点外，还包括曲线上1/3和2/3位置的点
    static vector<Point> regain_new_points(vector<Point>& points);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点，主要用于之后曲线形变以及重推控制点
    static vector<Point> get_bezier(vector<Point> &svg);
    // 绘制贝塞尔曲线，支持二次和三次
    static void draw_bezier(Mat &img, vector<Point> &bezier);
private:
    static void get_control_points(Point &p1, Point &thirdOne, Point &thirdTwo,
                                   Point &p4, Point &p2, Point &p3);
    
};

#endif /* defined(__face1__BezierUtil__) */
