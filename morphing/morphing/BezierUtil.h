//
//  BezierUtil.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__BezierUtil__
#define __morphing__BezierUtil__

#include <stdio.h>
#include "DrawUtil.h"

#include <opencv2/opencv.hpp>
using namespace cv;

class BezierUtil {
public:
    static void drawCubeBezier(Mat &img, Point p0, Point p1, Point p2, Point p3);
    static void drawCubeBezier(Mat &img, Point p0, Point p1, Point p2, Point p3, Scalar color);
    // points里面，除了贝塞尔曲线前后的端点外，还包括曲线上1/3和2/3位置的点
    static vector<Point> regainNewPoints(vector<Point>& points);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点，主要用于之后曲线形变以及重推控制点
    static vector<Point> getBezier1(vector<Point> &svg);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的点，主要用于确定贝塞尔曲线的范围大小，点越密越精确
    static vector<Point> getBezier2(vector<Point> &svg);
    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点以及完整的贝塞尔曲线点
    static void getBezier(vector<Point> &svg, vector<Point> &bezier1, vector<Point> &bezier2);
    // 绘制贝塞尔曲线，支持三次
    static void drawBezier(Mat &img, vector<Point> &bezier);
    static void drawBezier(Mat &img, vector<Point> &ctrl, Scalar color);
    
    static void getBezierBorder(vector<Point> &bezier, int &left, int &right,
                                  int &top, int &bottom);
private:
    static void getControlPoints(Point &p1, Point &thirdOne, Point &thirdTwo,
                                   Point &p4, Point &p2, Point &p3);
    
};
#endif /* defined(__morphing__BezierUtil__) */
