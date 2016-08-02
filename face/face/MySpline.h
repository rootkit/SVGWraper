//
//  SplineUtil.h
//  face
//
//  Created by xyz on 16/7/29.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face__SplineUtil__
#define __face__SplineUtil__

#include <stdio.h>
#include <vector>
#include <list>

using namespace std;

#include "spline.h"
using namespace tk;

#include <opencv2/opencv.hpp>
using namespace cv;

class MySpline {
private:
    vector<spline> splines;
    vector<double> x, y;
    static const int POINT_INDEX[6];
    bool isInit;
public:
    MySpline() {this->isInit = false;};
    MySpline(vector<double> &x, vector<double> &y);
    MySpline(double *x, double *y, int size);
    void init(double *x, double *y, int size);
    void init(vector<double> &x, vector<double> &y);
    int* splineInterpolate();
    void draw_asm_points(Mat &mat);
    void draw_face_contour(Mat &mat);
};

#endif /* defined(__face__SplineUtil__) */
