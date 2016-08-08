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

class DelTriangle{
public:
    vector<int> indexs;
    vector<Point> points;
    Point center;
    double radius;
public:
    DelTriangle(vector<Point> points, vector<int> indexs);
    void calTriangleCenter();
    static bool isThreePointsOnOneLine(const Point& p1,const Point& p2,const Point& p3);
    bool isOutterRightOfCircle(Point point);
    bool isInTheCircle(Point point);
    bool isOutterCircleButNotRight(Point point);
};

#endif /* defined(__face__DelTriangle__) */
