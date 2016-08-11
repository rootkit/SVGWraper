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

class FaceSpline {
public:
    static const int SPLINE_SECTIONS = 7;
    //int **splinePoints;
    vector<vector<int>> splinePoints;
    int *facePoints;
private:
    static const int FACE_POINTS = 20;
    static const int POINT_INDEX[SPLINE_SECTIONS];
    static const bool BASED_ON_X[SPLINE_SECTIONS];
    static const int SECTION_POINT_NUM[SPLINE_SECTIONS];
    double *pointX[SPLINE_SECTIONS];
    double *pointY[SPLINE_SECTIONS];
private:
    void initSpline();
    vector<int> findSection(int pointIndex);
    //int* getSplinePoints(double *x, double *y, bool basedOnX, int size);
    vector<int> getSplinePoints(double *x, double *y, bool basedOnX, int size);
public:
    void init(int face[], int size);
    FaceSpline();
    FaceSpline(int facePoints[], int size);
    ~FaceSpline();
    void adjustPoint(int pointIndex, int x, int y);
    //int** getSplinePoints();
    vector<vector<int>> getSplinePoints();
    bool checkPointInRange(int pointIndex, int x, int y);
    
};

#endif /* defined(__adjust_asm__FaceSplineForAnd__) */
