//
//  Morphing.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__Morphing__
#define __morphing__Morphing__

#include <stdio.h>
#include <vector>
#include "Triangle.h"
using namespace std;

class Morphing {
public:
    Morphing();
    Morphing(vector<Point> &srcPoints, vector<Point> &destPoints);
    void init(vector<Point> &srcPoints, vector<Point> &destPoints);
    bool findSrcTri(Point p, Triangle &t);
    bool findDestTri(Point p, Triangle &t);
    vector<Triangle> srcTris;
    vector<Triangle> destTris;
    Mat morphImg(const Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints);
    void morphBezier(vector<Point> &bezier);
private:
    const static int CONTROL_POINT_NUM = 27;
    const static int POINT_NUM = 86;
    static int POINT_INDEX[CONTROL_POINT_NUM];
    bool isInit;
};

#endif /* defined(__morphing__Morphing__) */
