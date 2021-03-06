//
//  DelTriangle.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__DelTriangle__
#define __morphing__DelTriangle__

#include <stdio.h>
#include <vector>
#include <cassert>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

struct Edge {
public:
    Point start, end;
    int startIndex, endIndex;
public:
    Edge(Point s, Point e, int startIndex, int endIndex) {
        if (s.x < e.x) {
            this->start = s;
            this->end = e;
            this->startIndex = startIndex;
            this->endIndex = endIndex;
        } else if (s.x == e.x) {
            if (s.y < e.y) {
                this->start = s;
                this->end = e;
                this->startIndex = startIndex;
                this->endIndex = endIndex;
            } else if (s.y == e.y) {
                assert(false && "the start point and end point shoule not be the same");
            } else {
                this->start = e;
                this->end = s;
                this->startIndex = endIndex;
                this->endIndex = startIndex;
            }
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


#endif /* defined(__morphing__DelTriangle__) */
