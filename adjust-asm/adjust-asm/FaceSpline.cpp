//
//  FaceSpline.cpp
//  adjust-asm
//
//  Created by xyz on 16/8/10.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <vector>
#include <iostream>
using namespace std;

#include "FaceSpline.h"

#include "spline.h"
using namespace tk;

const int FaceSpline::POINT_INDEX[6] = {0, 3, 6, 9, 12, 15};
const bool FaceSpline::BASED_ON_X[5] = {false, true, true, false, true};


FaceSpline::FaceSpline(int facePoints[], int size):
splinePoints(SPLINE_SECTIONS) {
    this->facePoints = new int[size];
    for (int i = 0; i < size; i++) {
        this->facePoints[i] = facePoints[i];
    }
    //this->splinePoints = new int*[SPLINE_SECTIONS];
    initSpline();
}

FaceSpline::~FaceSpline() {
    delete [] facePoints;
    for (int i = 0; i < SPLINE_SECTIONS; i++) {
        delete [] pointX[i];
        delete [] pointY[i];
        //delete [] splinePoints[i];
    }
}


void FaceSpline::initSpline() {
    for (int index = 0; index < 4; index++) {
        pointX[index] = new double[4];
        pointY[index] = new double[4];
        if (index == 3) {
            for (int i = POINT_INDEX[index+1], j = 0; i >= POINT_INDEX[index]; i--, j++) {
                pointX[index][j] = facePoints[i*2];
                pointY[index][j] = facePoints[i*2+1];
            }
        } else {
            for (int i = POINT_INDEX[index], j = 0; i <= POINT_INDEX[index+1]; i++, j++) {
                pointX[index][j] = facePoints[i*2];
                pointY[index][j] = facePoints[i*2+1];
            }
        }
        splinePoints[index] = FaceSpline::getSplinePoints(pointX[index], pointY[index], BASED_ON_X[index], 4);
    }
    pointX[4] = new double[5];
    pointY[4] = new double[5];
    pointX[4][0] = facePoints[0];
    pointY[4][0] = facePoints[1];
    for (int i = 15, j = 1; i >= 12; i--, j++) {
        pointX[4][j] = facePoints[i*2];
        pointY[4][j] = facePoints[i*2+1];
    }
    splinePoints[4] = FaceSpline::getSplinePoints(pointX[4], pointY[4], BASED_ON_X[4], 5);
}


vector<int> FaceSpline::findSection(int pointIndex) {
    vector<int> indexes;
    if (pointIndex == 0) {
        indexes.push_back(0);
        indexes.push_back(4);
    } else if (pointIndex == 3) {
        indexes.push_back(0);
        indexes.push_back(1);
    } else if (pointIndex == 6) {
        indexes.push_back(1);
        indexes.push_back(2);
    } else if (pointIndex == 9) {
        indexes.push_back(2);
        indexes.push_back(3);
    } else if (pointIndex == 12) {
        indexes.push_back(3);
        indexes.push_back(4);
    }
    if (pointIndex > 0 && pointIndex < 3) {
        indexes.push_back(0);
    } else if (pointIndex > 3 && pointIndex < 6) {
        indexes.push_back(1);
    } else if (pointIndex > 6 && pointIndex < 9) {
        indexes.push_back(2);
    } else if (pointIndex > 9 && pointIndex < 12) {
        indexes.push_back(3);
    } else if (pointIndex > 12 && pointIndex <= 15) {
        indexes.push_back(4);
    } else if (pointIndex > 15) {
        assert(false && "pointIndex out of range");
    }
    return indexes;
}


void FaceSpline::adjustPoint(int pointIndex, int x, int y) {
    facePoints[pointIndex*2] = x;
    facePoints[pointIndex*2+1] = y;
    vector<int> indexes = findSection(pointIndex);
    
    for (int i = 0; i < indexes.size(); i++) {
        int index = indexes[i];
        if (index < 4) {
            if (index == 3) {
                for (int ii = POINT_INDEX[index+1], j = 0; ii >= POINT_INDEX[index]; ii--, j++) {
                    pointX[index][j] = facePoints[ii*2];
                    pointY[index][j] = facePoints[ii*2+1];
                }
                splinePoints[index] = getSplinePoints(pointX[index], pointY[index], BASED_ON_X[index], 4);
            } else {
                for (int ii = POINT_INDEX[index], j = 0; ii < POINT_INDEX[index + 1]; ii++, j++) {
                    pointX[index][j] = facePoints[ii * 2];
                    pointY[index][j] = facePoints[ii * 2 + 1];
                }
                splinePoints[index] = getSplinePoints(pointX[index], pointY[index], BASED_ON_X[index], 4);
            }
        } else {
            pointX[4][0] = facePoints[0];
            pointY[4][0] = facePoints[1];
            for (int ii = 15, j = 1; ii >= 12; ii--, j++) {
                pointX[4][j] = facePoints[ii*2];
                pointY[4][j] = facePoints[ii*2+1];
            }
            splinePoints[4] = getSplinePoints(pointX[4], pointY[4], BASED_ON_X[4], 5);
        }
    }
}


//int** FaceSpline::getSplinePoints() {
//    return this->splinePoints;
//}


vector<vector<int>> FaceSpline::getSplinePoints() {
    return this->splinePoints;
}


/**
 * x,y: 分别是插值点的坐标，用于插值的方向要事先按照升序排列.
 * basedOnX: 表示是要按照x轴方向插值，还是按照y轴方向
 * size: x,y数组的长度
 */
//int* FaceSpline::getSplinePoints(double *x, double *y, bool basedOnX, int size) {
//    int *points;
//    vector<int> tempPoints;
//    if (basedOnX) {
//        vector<double> X, Y;
//        for (int i = 0; i < size; i++) {
//            X.push_back(x[i]);
//            Y.push_back(y[i]);
//        }
//        spline s;
//        s.set_points(X, Y);
//        for (int col = x[0]; col < x[size-1]; col += 10) {
//            tempPoints.push_back(col);
//            tempPoints.push_back(s(col));
//        }
//        points = new int[tempPoints.size()];
//        for (int i = 0; i < tempPoints.size(); i++) {
//            points[i] = tempPoints[i];
//        }
//    } else {
//        vector<double> X, Y;
//        for (int i = 0; i < size; i++) {
//            X.push_back(y[i]);
//            Y.push_back(x[i]);
//        }
//        spline s;
//        s.set_points(X, Y);
//        
//        for (int row = y[0]; row < y[size-1]; row++) {
//            tempPoints.push_back(s(row));
//            tempPoints.push_back(row);
//        }
//        points = new int[tempPoints.size()];
//        for (int i = 0; i < tempPoints.size(); i++) {
//            points[i] = tempPoints[i];
//        }
//    }
//    return points;
//}


vector<int> FaceSpline::getSplinePoints(double *x, double *y, bool basedOnX, int size) {
    
    vector<int> tempPoints;
    if (basedOnX) {
        vector<double> X, Y;
        for (int i = 0; i < size; i++) {
            X.push_back(x[i]);
            Y.push_back(y[i]);
        }
        spline s;
        s.set_points(X, Y);
        for (int col = x[0]; col <= x[size-1]; col += 1) {
            tempPoints.push_back(col);
            tempPoints.push_back(s(col));
        }
    } else {
        vector<double> X, Y;
        for (int i = 0; i < size; i++) {
            X.push_back(y[i]);
            Y.push_back(x[i]);
        }
        spline s;
        s.set_points(X, Y);

        for (int row = y[0]; row < y[size-1]; row+=1) {
            tempPoints.push_back(s(row));
            tempPoints.push_back(row);
        }
    }
    return tempPoints;
}