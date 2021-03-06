
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
    static void draw_cube_bezier(Mat &img, Point p0, Point p1, Point p2, Point p3);
    static void draw_cube_bezier(Mat &img, Point p0, Point p1, Point p2, Point p3, Scalar color);
    // points里面，除了贝塞尔曲线前后的端点外，还包括曲线上1/3和2/3位置的点
    static vector<Point> regain_new_points(vector<Point>& points);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点，主要用于之后曲线形变以及重推控制点
    static vector<Point> get_bezier_1(vector<Point> &svg);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的点，主要用于确定贝塞尔曲线的范围大小，点越密越精确
    static vector<Point> get_bezier_2(vector<Point> &svg);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点以及完整的贝塞尔曲线点
    static void get_bezier(vector<Point> &svg, vector<Point> &bezier1, vector<Point> &bezier2);
    // 绘制贝塞尔曲线，支持三次
    static void draw_bezier(Mat &img, vector<Point> &bezier);
    static void draw_bezier(Mat &img, vector<Point> &ctrl, Scalar color);
    
    static void get_bezier_border(vector<Point> &bezier, int &left, int &right,
                                  int &top, int &bottom);
private:
    static void get_control_points(Point &p1, Point &thirdOne, Point &thirdTwo,
                                   Point &p4, Point &p2, Point &p3);
    
};

#endif /* defined(__face1__BezierUtil__) */
