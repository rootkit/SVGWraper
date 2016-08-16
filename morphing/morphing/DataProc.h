//
//  DataProc.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__DataProc__
#define __morphing__DataProc__

#include <stdio.h>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class DataProc {
public:
    static void normalizeFaceData(vector<Point> &srcPoints, vector<Point> &targetPoints);
    static void alignAsmToSvg(vector<Point> &face, const vector<Point> &bezier, double &scale, Point &translate);
    static void alignSvgToAsm(const vector<Point> &face, vector<Point> &bezier, double &scale, Point &translate);
    static void recoverSvg(vector<Point> &bezier, double &scale, Point &translate);
    static void alignPoints(vector<Point> &points, Point translate);
};


#endif /* defined(__morphing__DataProc__) */
