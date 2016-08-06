//
//  BezierUtil.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "BezierUtil.h"

void BezierUtil::draw_cube_bezier(Mat &img, Point p0, Point p1, Point p2, Point p3) {
    int x, y;
    for (float t = 0; t <= 1; t += 0.001) {
        x = (int)((1-t)*(1-t)*(1-t)*p0.x + 3*t*(1-t)*(1-t)*p1.x + 3*t*t*(1-t)*p2.x + t*t*t*p3.x);
        y = (int)((1-t)*(1-t)*(1-t)*p0.y + 3*t*(1-t)*(1-t)*p1.y + 3*t*t*(1-t)*p2.y + t*t*t*p3.y);
        DrawUtil::draw_point(img, x, y, 1);
    }
}

void BezierUtil::draw_cube_bezier(Mat &img, Point p0, Point p1, Point p2, Point p3, Scalar color) {
    int x, y;
    for (float t = 0; t <= 1; t += 0.001) {
        x = (int)((1-t)*(1-t)*(1-t)*p0.x + 3*t*(1-t)*(1-t)*p1.x + 3*t*t*(1-t)*p2.x + t*t*t*p3.x);
        y = (int)((1-t)*(1-t)*(1-t)*p0.y + 3*t*(1-t)*(1-t)*p1.y + 3*t*t*(1-t)*p2.y + t*t*t*p3.y);
        DrawUtil::draw_point(img, x, y, 1, color);
    }
}

void BezierUtil::get_control_points(Point &p1, Point &thirdOne, Point &thirdTwo,
                        Point &p4, Point &p2, Point &p3) {
    double xb1, yb1, xb2, yb2;   // 计算的中间变量
    double f1 = 0.037037037037037037037; // (1/3)^3
    double f2 = 0.296296296296296296296; // (2/3)^3
    double x2, y2, x3, y3;    // 返回的p2、p3的坐标
    xb1 = thirdOne.x - f2 * p1.x - f1 * p4.x;
    yb1 = thirdOne.y - f2 * p1.y - f1 * p4.y;
    xb2 = thirdTwo.x - f1 * p1.x - f2 * p4.x;
    yb2 = thirdTwo.y - f1 * p1.y - f2 * p4.y;
    x2 = 3 * xb1 - 3 / (double)2 * xb2;
    y2 = 3 * yb1 - 3 / (double)2 * yb2;
    x3 = 3 * xb2 - 3 / (double)2 * xb1;
    y3 = 3 * yb2 - 3 / (double)2 * yb1;
    p2.x = (int)x2;
    p2.y = (int)y2;
    p3.x = (int)x3;
    p3.y = (int)y3;
}

/**
 * points里面，除了贝塞尔曲线前后的端点外，还包括曲线上1/3和2/3位置的点
 */
vector<Point> BezierUtil::regain_new_points(vector<Point>& points) {
    vector<Point> newPoints;
    
    for (int i = 0; i < points.size()-1; i+=3) {
        Point p2, p3;
        get_control_points(points[i], points[i+1], points[i+2],
                           points[i+3], p2, p3);
        if (i == 0) {
            newPoints.push_back(points[i]);
        }
        newPoints.push_back(p2);
        newPoints.push_back(p3);
        newPoints.push_back(points[i+3]);
    }
    return newPoints;
}

// 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点，主要用于之后曲线形变以及重推控制点
vector<Point> BezierUtil::get_bezier_1(vector<Point> &svg) {
    assert((svg.size()-1)%3==0);
    
    vector<Point> bezier;
    double t;
    Point p1, p2, p3, p4;  // 贝塞尔曲线端点和控制点
    Point thirdOne, thirdTwo;  // 贝塞尔曲线上t取1/3，2/3的点
    for (int i = 0; i < svg.size()-1; i+=3) {
        if (i == 0) {
            p1 = svg[0];
            bezier.push_back(p1);
        } else {
            p1 = p4;
        }
        p2 = svg[i+1];
        p3 = svg[i+2];
        p4 = svg[i+3];
        
        t = 1 / (double)3;
        thirdOne.x = (int)((1-t)*(1-t)*(1-t)*p1.x + 3*t*(1-t)*(1-t)*p2.x + 3*t*t*(1-t)*p3.x + t*t*t*p4.x);
        thirdOne.y = (int)((1-t)*(1-t)*(1-t)*p1.y + 3*t*(1-t)*(1-t)*p2.y + 3*t*t*(1-t)*p3.y + t*t*t*p4.y);
        t = 2 / (double)3;
        thirdTwo.x = (int)((1-t)*(1-t)*(1-t)*p1.x + 3*t*(1-t)*(1-t)*p2.x + 3*t*t*(1-t)*p3.x + t*t*t*p4.x);
        thirdTwo.y = (int)((1-t)*(1-t)*(1-t)*p1.y + 3*t*(1-t)*(1-t)*p2.y + 3*t*t*(1-t)*p3.y + t*t*t*p4.y);
        bezier.push_back(thirdOne);
        bezier.push_back(thirdTwo);
        bezier.push_back(p4);
    }
    
    return bezier;
}

    // 根据贝塞尔曲线控制点得到贝塞尔曲线上的点，主要用于确定贝塞尔曲线的范围大小，点越密越精确
vector<Point> BezierUtil::get_bezier_2(vector<Point> &svg) {
    assert((svg.size()-1)%3==0);
    
    vector<Point> bezier;
    int x, y;
    Point p1, p2, p3, p4;  // 贝塞尔曲线端点和控制点
    for (int i = 0; i < svg.size()-1; i+=3) {
        if (i == 0) {
            p1 = svg[0];
            bezier.push_back(p1);
        } else {
            p1 = p4;
        }
        p2 = svg[i+1];
        p3 = svg[i+2];
        p4 = svg[i+3];
        for (float t = 0.1; t <= 1; t += 0.1) {
            x = (int)((1-t)*(1-t)*(1-t)*p1.x + 3*t*(1-t)*(1-t)*p2.x + 3*t*t*(1-t)*p3.x + t*t*t*p4.x);
            y = (int)((1-t)*(1-t)*(1-t)*p1.y + 3*t*(1-t)*(1-t)*p2.y + 3*t*t*(1-t)*p3.y + t*t*t*p4.y);
            bezier.push_back(Point(x, y));
        }
    }
    return bezier;
}

/** 根据贝塞尔曲线控制点得到贝塞尔曲线上的三等分点以及完整的贝塞尔曲线点
 * bezier1: 三等分点构成的贝塞尔曲线，用于形变后还原控制点
 * bezier2: 完整的贝塞尔曲线，主要用于测量贝塞尔曲线的边界点
 */
void BezierUtil::get_bezier(vector<Point> &svg, vector<Point> &bezier1, vector<Point> &bezier2) {
    bezier1 = get_bezier_1(svg);
    bezier2 = get_bezier_2(svg);
}

void BezierUtil::draw_bezier(Mat &img, vector<Point> &ctrl) {
    if ((ctrl.size() - 1) % 3 == 0) {
        for (int i = 0; i < ctrl.size()-1; i+=3) {
            draw_cube_bezier(img, ctrl[i], ctrl[i+1], ctrl[i+2], ctrl[i+3]);
        }
    } else {
        assert(false && "support only quadratic and cube bezier");
    }
}

void BezierUtil::draw_bezier(Mat &img, vector<Point> &ctrl, Scalar color) {
    if ((ctrl.size() - 1) % 3 == 0) {
        for (int i = 0; i < ctrl.size()-1; i+=3) {
            draw_cube_bezier(img, ctrl[i], ctrl[i+1], ctrl[i+2], ctrl[i+3], color);
        }
    } else {
        assert(false && "support only quadratic and cube bezier");
    }
}



void BezierUtil::get_bezier_border(vector<Point> &bezier, int &left, int &right,
                                   int &top, int &bottom) {
    int minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    for (int i = 0; i < bezier.size(); i++) {
        if (minX > bezier[i].x) {
            minX = bezier[i].x;
            left = i;
        }
        if (minY > bezier[i].y) {
            minY = bezier[i].y;
            top = i;
        }
        if (maxX < bezier[i].x) {
            maxX = bezier[i].x;
            right = i;
        }
        if (maxY < bezier[i].y) {
            maxY = bezier[i].y;
            bottom = i;
        }
    }
}




