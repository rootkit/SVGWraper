//
//  FaceSplineNew.h
//  adjust-asm
//
//  Created by xyz on 16/8/11.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __adjust_asm__FaceSplineNew__
#define __adjust_asm__FaceSplineNew__

#include <stdio.h>

#include <stdio.h>
#include <vector>
using namespace std;

class FaceSpline {
public:
    static const int ADDED_SPLINE_POINTS = 4;
    static const int SPLINE_SECTIONS = 7;
    //int **splinePoints;
    vector<vector<int>> splinePoints;
private:
    static const int FACE_POINTS = 20;
    static const int POINT_INDEX[SPLINE_SECTIONS];
    static const bool BASED_ON_X[SPLINE_SECTIONS];
    static const int SECTION_POINT_NUM[SPLINE_SECTIONS];
    int *facePoints;
    double *pointX[SPLINE_SECTIONS];
    double *pointY[SPLINE_SECTIONS];
private:
    void initSpline();
public:
    FaceSpline(int facePoints[], int size);
    ~FaceSpline();
    void init(int face[], int size);
    void adjustPoint(int pointIndex, int x, int y);
    //int** getSplinePoints();
    vector<vector<int>> getSplinePoints();
    vector<int> findSection(int pointIndex);
    bool checkPointInRange(int pointIndex, int x, int y);
    //static int* getSplinePoints(double *x, double *y, bool basedOnX, int size);
    static vector<int> getSplinePoints(double *x, double *y, bool basedOnX, int size);
    
};

#endif /* defined(__adjust_asm__FaceSplineNew__) */
