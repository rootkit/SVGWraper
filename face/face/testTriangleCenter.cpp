//
//  testTriangleCenter.cpp
//  face
//
//  Created by xyz on 16/8/7.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <stdio.h>
#include <iostream>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "DrawUtil.h"

bool isThreePointsOnOneLine(const Point& p1,const Point& p2,const Point& p3);
bool calTriangleCenter(Point &p1, Point &p2, Point &p3, Point &center, double &r);

int main() {
    Point p1(-28403, -510), p2(489, 224), p3(30277, -510);
    Point p4(489, 1691);
        Point center;
        double radius;
        calTriangleCenter(p1, p2, p3, center, radius);
    cout << "center: " << center << endl;
    cout << "radius: " << radius << endl;
    
//    Mat image(500, 500, CV_8UC1, Scalar::all(0));
//    DrawUtil::draw_point(image, p1.x, p1.y, 3);
//    DrawUtil::draw_point(image, p2.x, p2.y, 3);
//    DrawUtil::draw_point(image, p3.x, p3.y, 3);
//    Point center;
//    double radius;
//    calTriangleCenter(p1, p2, p3, center, radius);
//    circle(image, center, radius, Scalar(255, 255, 255));
//    imwrite("circle.jpg", image);
}

bool isThreePointsOnOneLine(const Point& p1,const Point& p2,const Point& p3) {
    if (p2.x == p1.x) {
        if (p2.x == p3.x) {
            return true;
        }
        return false;
    } else {
        if (p2.x == p3.x) {
            return false;
        }
    }
    
    double k1 = (double)(p2.y-p1.y) / (p2.x-p1.x);
    double k2 = (double)(p3.y-p2.y) / (p3.x-p2.x);
    
    cout << k1 << "   " << k2 << endl;
    double DIFF = 0.000001;
    if (fabs(k1-k2) < DIFF) {
        return true;
    }
    return false;
}

bool calTriangleCenter(Point &p1, Point &p2, Point &p3, Point &center, double &r) {
    if (isThreePointsOnOneLine(p1, p2, p3)) {
        cout << "one line" << endl;
        return false;
    }
    double x1, x2, x3, y1, y2, y3;
    x1 = p1.x;
    x2 = p2.x;
    x3 = p3.x;
    y1 = p1.y;
    y2 = p2.y;
    y3 = p3.y;
    
    // 计算外接圆半径
    double a = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    double b = sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
    double c = sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3));
    double p = p = (a+b+c) / 2;
    double S = sqrt(p*(p-a)*(p-b)*(p-c));
    r = a*b*c / (4*S);
    
    // 计算外接圆圆心
    double t1 = x1*x1+y1*y1;
    double t2 = x2*x2+y2*y2;
    double t3 = x3*x3+y3*y3;
    double temp = x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
    double x = (t1*y2+t2*y3+t3*y1-t3*y2-t1*y3-t2*y1) / temp / 2;
    double y = (t3*x2+t2*x1+t1*x3-t1*x2-t2*x3-t3*x1) / temp / 2;
    center.x = x;
    center.y = y;
    
    return true;
    
}