//
//  DelTriangle.cpp
//  face
//
//  Created by xyz on 16/8/8.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "DelTriangle.h"
#include <cmath>


DelTriangle::DelTriangle(vector<Point> &points, vector<int> &indexs) {
    assert(points.size() == 3 && indexs.size() == 3);
    this->points = points;
    this->indexs = indexs;
    
    assert(!DelTriangle::isThreePointsOnOneLine(points[0], points[1], points[2]));
    calTriangleCenter();
    
    if (radius == NAN) {
        assert(false && "radius too large");
    }
}

DelTriangle::DelTriangle(const DelTriangle& other) {
    *this = other;
}

void DelTriangle::operator=(const DelTriangle& other) {
    this->indexs = other.indexs;
    this->points = other.points;
    this->center = other.center;
    this->radius = other.radius;
}

void DelTriangle::calTriangleCenter() {
    double x1, x2, x3, y1, y2, y3;
    x1 = points[0].x;
    x2 = points[1].x;
    x3 = points[2].x;
    y1 = points[0].y;
    y2 = points[1].y;
    y3 = points[2].y;
    
    // 计算外接圆半径
    double a = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    double b = sqrt((x1-x3)*(x1-x3)+(y1-y3)*(y1-y3));
    double c = sqrt((x2-x3)*(x2-x3)+(y2-y3)*(y2-y3));
    double p = p = (a+b+c) / 2;
    double S = sqrt(p*(p-a)*(p-b)*(p-c));
    this->radius = a*b*c / (4*S);
    
    // 计算外接圆圆心
    double t1 = x1*x1+y1*y1;
    double t2 = x2*x2+y2*y2;
    double t3 = x3*x3+y3*y3;
    double temp = x1*y2+x2*y3+x3*y1-x1*y3-x2*y1-x3*y2;
    double x = (t1*y2+t2*y3+t3*y1-t3*y2-t1*y3-t2*y1) / temp / 2;
    double y = (t3*x2+t2*x1+t1*x3-t1*x2-t2*x3-t3*x1) / temp / 2;
    this->center.x = x;
    this->center.y = y;
}

bool DelTriangle::isThreePointsOnOneLine(const Point& p1,const Point& p2,const Point& p3) {
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
    
    double DIFF = 0.0000001;
    if (fabs(k1-k2) < DIFF) {
        
        cout << "isThreePointsOnOneLine" << endl;
        cout << "k1: " << k1 << endl;
        cout << "k2: " << k2 << endl;
        
        
        return true;
    }
    return false;
}


bool DelTriangle::isOutterRightOfCircle(Point point) {
    if (point.x - center.x > radius) {
        return true;
    }
    return false;
}

bool DelTriangle::isInTheCircle(Point point) {
    double r = sqrt((point.x-center.x)*(point.x-center.x)+(point.y-center.y)*(point.y-center.y));
    if (r > radius) {
        return false;
    } else {
        return true;
    }
}

bool DelTriangle::isOutterCircleButNotRight(Point point) {
    return !isOutterRightOfCircle(point) && !isInTheCircle(point);
}



