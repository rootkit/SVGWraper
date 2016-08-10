//
//  FaceSpline.cpp
//  face
//
//  Created by xyz on 16/8/10.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <vector>
using namespace std;

#include "FaceSpline.h"

#include "spline.h"
using namespace tk;

/**
 * x,y: 分别是插值点的坐标，用于插值的方向要事先按照升序排列.
 * basedOnX: 表示是要按照x轴方向插值，还是按照y轴方向
 * size: x,y数组的长度
 */
int* FaceSpline::getSplinePoints(double *x, double *y, bool basedOnX, int size) {
    int *points;
    vector<int> tempPoints;
    if (basedOnX) {
        vector<double> X, Y;
        for (int i = 0; i < size; i++) {
            X.push_back(x[i]);
            Y.push_back(y[i]);
        }
        spline s;
        s.set_points(X, Y);
        for (int col = x[0]; col < x[size-1]; col += 10) {
            tempPoints.push_back(col);
            tempPoints.push_back(s(col));
        }
        points = new int[tempPoints.size()];
        for (int i = 0; i < tempPoints.size(); i++) {
            points[i] = tempPoints[i];
        }
    } else {
        vector<double> X, Y;
        for (int i = 0; i < size; i++) {
            X.push_back(y[i]);
            Y.push_back(x[i]);
        }
        spline s;
        s.set_points(X, Y);
        
        for (int row = y[0]; row < y[size-1]; row++) {
            tempPoints.push_back(s(row));
            tempPoints.push_back(row);
        }
        points = new int[tempPoints.size()];
        for (int i = 0; i < tempPoints.size(); i++) {
            points[i] = tempPoints[i];
        }
    }
    return points;
}