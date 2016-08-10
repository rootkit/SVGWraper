//
//  testFaceSpline.cpp
//  face
//
//  Created by xyz on 16/8/10.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <stdio.h>

#include "FaceSpline.h"
#include "FiltUtil.h"

// the number of spline sections
const int SPLINE_SECTIONS = 5;

// the number of asm face points
const int FACE_POINTS = 16;

const int POINT_INDEX[]{
    0, 3, 6, 9, 12, 15
};

const bool BASED_ON_X[]{
    false, true, true, false, true
};

// asm face points
int facePoints[32];

double *pointX[SPLINE_SECTIONS];
double *pointY[SPLINE_SECTIONS];

// spline points of asm, including the original face points
int *splinePoints[SPLINE_SECTIONS];

void initSpline();

int main() {
    vector<vector<Point>> asmPoints = FileUtil::read_all_asm_points("dataBase.txt");
    
    vector<Point> testPoints = asmPoints[0];
    
    for (int i = 0; i < 16; i++) {
        facePoints[i*2] = testPoints[i].x;
        facePoints[i*2+1] = testPoints[i].y;
    }
    
    initSpline();
    
    Mat image(3000, 3000, CV_8UC1, Scalar::all(0));

}

void initSpline() {
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



