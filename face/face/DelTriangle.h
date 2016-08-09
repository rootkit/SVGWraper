//
//  DelTriangle.h
//  face
//
//  Created by xyz on 16/8/8.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face__DelTriangle__
#define __face__DelTriangle__

#include <stdio.h>
#include <vector>
#include <cassert>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class PointComp {
public:
    bool operator() (const Point &a, const Point &b) const {
        return a.x < b.x || (a.x == b.x && a.y < b.y);
    }
};
    
struct Edge {
public:
    Point start, end;
    int startIndex, endIndex;
public:
    Edge(Point s, Point e, int startIndex, int endIndex) {
        if (s.x <= e.x) {
            this->start = s;
            this->end = e;
            this->startIndex = startIndex;
            this->endIndex = endIndex;
        } else {
            this->start = e;
            this->end = s;
            this->startIndex = endIndex;
            this->endIndex = startIndex;
        }
    }
    bool cmp(const Point &a, const Point &b) const {
        if (a.x < b.x) {
            return true;
        } else if (a.x == b.x) {
            if (a.y < b.y) {
                return true;
            }
        }
        return false;
    }
    bool operator<(const Edge &e2) const {
        if (start == e2.start) {
            return cmp(end, e2.end);
        } else {
            return cmp(start, e2.start);
        }
    }
};

class DelTriangle{
public:
    vector<int> indexs;
    vector<Point> points;
    Point center;
    double radius;
public:
    DelTriangle(vector<Point> &points, vector<int> &indexs);
    DelTriangle(const DelTriangle& other);
    void operator=(const DelTriangle& other);
    void calTriangleCenter();
    static bool isThreePointsOnOneLine(const Point& p1,const Point& p2,const Point& p3);
    bool isOutterRightOfCircle(Point point);
    bool isInTheCircle(Point point);
    bool isOutterCircleButNotRight(Point point);
};



#endif /* defined(__face__DelTriangle__) */
