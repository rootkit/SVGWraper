//
//  SplineUtil.cpp
//  face
//
//  Created by xyz on 16/7/29.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "MySpline.h"
#include "DrawUtil.h"
#include <cassert>


const int MySpline::POINT_INDEX[6] = {0, 3, 6, 9, 12, 15};


/**
 * x, y分别是人脸16对点的x坐标和y坐标
 */
MySpline::MySpline(vector<double> &x, vector<double> &y) {
    init(x, y);
}

void MySpline::init(vector<double> &x, vector<double> &y) {
    assert(x.size() == y.size());
    this->splines.clear();
    this->x.clear();
    this->y.clear();
    this->isInit = true;
    this->x = x;
    this->y = y;
    for (int c = 0; c < 4; c++) {
        list<double> tempX;
        list<double> tempY;
        for (int i = POINT_INDEX[c]; i <= POINT_INDEX[c+1]; i++) {
            if (c == 0) {               // y轴为标准计算样条曲线
                tempX.push_back(y[i]);
                tempY.push_back(x[i]);
            } else if (c == 3) {        // y轴为标准计算样条曲线
                tempX.push_front(y[i]);
                tempY.push_front(x[i]);
            } else if (c == 1 || c == 2) {   // x轴为标准计算样条曲线
                tempX.push_back(x[i]);
                tempY.push_back(y[i]);
            }
        }
        vector<double> X(tempX.begin(), tempX.end());
        vector<double> Y(tempY.begin(), tempY.end());
        spline s;
        s.set_points(X, Y);
        splines.push_back(s);
    }
    vector<double> X, Y;
    X.push_back(x[0]);
    Y.push_back(y[0]);
    for (int i = 15; i >= 12; i--) {
        X.push_back(x[i]);
        Y.push_back(y[i]);
    }
    spline s;
    s.set_points(X, Y);
    splines.push_back(s);
}

/**
 * x, y分别是人脸16对点的x坐标和y坐标
 */
MySpline::MySpline(double *x, double *y, int size) {
    this->init(x, y, size);
}

void MySpline::init(double *x, double *y, int size) {
    this->splines.clear();
    this->x.clear();
    this->y.clear();
    this->isInit = true;
    for (int i = 0; i < size; i++) {
        this->x.push_back(x[i]);
        this->y.push_back(y[i]);
    }
    for (int c = 0; c < 4; c++) {
        list<double> tempX;
        list<double> tempY;
        for (int i = POINT_INDEX[c]; i <= POINT_INDEX[c+1]; i++) {
            if (c == 0) {               // y轴为标准计算样条曲线
                tempX.push_back(y[i]);
                tempY.push_back(x[i]);
            } else if (c == 3) {        // y轴为标准计算样条曲线
                tempX.push_front(y[i]);
                tempY.push_front(x[i]);
            } else if (c == 1 || c == 2) {   // x轴为标准计算样条曲线
                tempX.push_back(x[i]);
                tempY.push_back(y[i]);
            }
        }
        vector<double> X(tempX.begin(), tempX.end());
        vector<double> Y(tempY.begin(), tempY.end());
        spline s;
        s.set_points(X, Y);
        splines.push_back(s);
    }
    vector<double> X, Y;
    X.push_back(x[0]);
    Y.push_back(y[0]);
    for (int i = 15; i >= 12; i--) {
        X.push_back(x[i]);
        Y.push_back(y[i]);
    }
    spline s;
    s.set_points(X, Y);
    splines.push_back(s);
}

int* MySpline::splineInterpolate() {
    assert(this->isInit == true);
    int *points = new int[32];
    int row, col, index = 0;
    for (int i = 0; i < POINT_INDEX[1]; i++) {
        row =(y[i] + y[i+1]) / 2;
        col = splines[0](row);
        points[index++] = col;
        points[index++] = row;
    }
    for (int i = POINT_INDEX[1]; i < POINT_INDEX[2]; i++) {
        col = (x[i] + x[i+1]) / 2;
        row = splines[1](col);
        points[index++] = col;
        points[index++] = row;
    }
    for (int i = POINT_INDEX[2]; i < POINT_INDEX[3]; i++) {
        col = (x[i] + x[i+1]) / 2;
        row = splines[2](col);
        points[index++] = col;
        points[index++] = row;
    }
    for (int i = POINT_INDEX[3]; i < POINT_INDEX[4]; i++) {
        row = (y[i] + y[i+1]) / 2;
        col = splines[3](row);
        points[index++] = col;
        points[index++] = row;
    }
    
    for (int i = POINT_INDEX[4]; i < POINT_INDEX[5]; i++) {
        col = (x[i] + x[i+1]) / 2;
        row = splines[4](col);
        points[index++] = col;
        points[index++] = row;
    }
    col = (x[15] + x[0]) / 2;
    row = splines[4](col);
    points[index++] = col;
    points[index++] = row;

    return points;
}

void MySpline::draw_asm_points(Mat &mat) {
    assert(this->isInit == true);
    for (int i = 0; i < 16; i++) {
        DrawUtil::draw_point(mat, x[i], y[i], 4);
    }
}

void MySpline::draw_face_contour(Mat &mat) {
    assert(this->isInit == true);
    draw_asm_points(mat);
    int* points = splineInterpolate();
    for (int i = 0; i < 16; i++) {
        DrawUtil::draw_point(mat, points[i*2], points[i*2+1], 2);
    }
}

















