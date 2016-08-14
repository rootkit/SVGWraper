//
//  FaceSplineForAnd.h
//  adjust-asm
//
//  Created by xyz on 16/8/11.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __adjust_asm__FaceSplineForAnd__
#define __adjust_asm__FaceSplineForAnd__

#include <stdio.h>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class FaceSpline {
public:
    static const int SPLINE_SECTIONS = 7;
    vector<vector<double>> splinePoints;
    int *facePoints;
private:
    static const int FACE_POINTS = 20;
    static const int POINT_INDEX[SPLINE_SECTIONS];
    static const bool BASED_ON_X[SPLINE_SECTIONS];
    static const int SECTION_POINT_NUM[SPLINE_SECTIONS];
    double *pointX[SPLINE_SECTIONS];
    double *pointY[SPLINE_SECTIONS];
private:
    void initSpline(int interpolateNum);
    vector<int> findSection(int pointIndex);
    vector<double> getSplinePoints(double *x, double *y, bool basedOnX, int size, int interpolateNum);
    
public:
    // interpolateNum表示没两个asm点之间插值点的个数，包括端点。若为－1则使用默认值
    void init(int face[], int size, int interpolateNum);
    FaceSpline();
    FaceSpline(int facePoints[], int size, int interpolateNum);
    ~FaceSpline();
    void adjustPoint(int pointIndex, int x, int y);
    vector<vector<double>> getSplinePoints();
    bool checkPointInRange(int pointIndex, int x, int y);
    static Mat getSplinePoints(Mat &asmPoints, int interpolationNum);
    
};

#endif /* defined(__adjust_asm__FaceSplineForAnd__) */
