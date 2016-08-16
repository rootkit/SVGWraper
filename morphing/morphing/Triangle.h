//
//  Triangle.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__Triangle__
#define __morphing__Triangle__

#include <stdio.h>
#include <vector>
#include "AffineTransform.h"

using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class Triangle {
public:
    vector<Point> srcTris;
    vector<Point> destTris;
    AffineTransform affine;
public:
    Triangle() {}
    Triangle(const vector<Point> &srcTris);
    Triangle(const Triangle& tri);
    ~Triangle();
    void operator=(const Triangle& tri);
    void setDestTriangle(const vector<Point> &destTris);
    bool isInTriangle(Point p);
};

#endif /* defined(__morphing__Triangle__) */
