//
//  FaceSplineForAnd.cpp
//  adjust-asm
//
//  Created by xyz on 16/8/11.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "FaceSplineForAnd.h"


#include <vector>
#include <iostream>
using namespace std;


#include "spline.h"
using namespace tk;

const int FaceSpline::POINT_INDEX[SPLINE_SECTIONS] = {0, 3, 6, 9, 12, 15, 17};
const bool FaceSpline::BASED_ON_X[SPLINE_SECTIONS] = {false, true, true, false, false, true, false};
const int FaceSpline::SECTION_POINT_NUM[SPLINE_SECTIONS] = {4, 4, 4, 4, 4, 3, 4};



FaceSpline::FaceSpline() {
    this->facePoints = NULL;
    for (int i = 0; i < SPLINE_SECTIONS; i++) {
        pointX[i] = NULL;
        pointY[i] = NULL;
    }
}

FaceSpline::~FaceSpline() {
    delete [] facePoints;
    for (int i = 0; i < SPLINE_SECTIONS; i++) {
        delete [] pointX[i];
        delete [] pointY[i];
        pointX[i] = NULL;
        pointY[i] = NULL;
    }
    facePoints = NULL;
    
}

FaceSpline::FaceSpline(int facePoints[], int size, int interpolateNum) {
    this->facePoints = NULL;
    for (int i = 0; i < SPLINE_SECTIONS; i++) {
        pointX[i] = NULL;
        pointY[i] = NULL;
    }
    init(facePoints, size, interpolateNum);
}

void FaceSpline::init(int face[], int size, int interpolateNum) {
    assert(size == FACE_POINTS*2);
    
    splinePoints.clear();
    if (facePoints != NULL) {
        delete [] facePoints;
    }
    for (int i = 0; i < SPLINE_SECTIONS; i++) {
        if (pointX[i] != NULL) {
            delete [] pointX[i];
        }
        if (pointY[i] != NULL) {
            delete [] pointY[i];
        }
    }
    
    this->facePoints = new int[size];
    for (int i = 0; i < size; i++) {
        this->facePoints[i] = face[i];
    }
    
    splinePoints = vector<vector<double>>(SPLINE_SECTIONS);
    initSpline(interpolateNum);
}


void FaceSpline::initSpline(int interpolateNum) {
    for (int index = 0; index < SPLINE_SECTIONS-1; index++) {
        pointX[index] = new double[SECTION_POINT_NUM[index]];
        pointY[index] = new double[SECTION_POINT_NUM[index]];
        if (index == 3 || index == 4 || index == 5) {
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
        splinePoints[index] = getSplinePoints(pointX[index], pointY[index], BASED_ON_X[index], SECTION_POINT_NUM[index], interpolateNum);
    }
    int index = SPLINE_SECTIONS-1;
    pointX[index] = new double[SECTION_POINT_NUM[index]];
    pointY[index] = new double[SECTION_POINT_NUM[index]];
    int j = 0;
    for (int i = POINT_INDEX[index]; i < FACE_POINTS; i++, j++) {
        pointX[index][j] = facePoints[i*2];
        pointY[index][j] = facePoints[i*2+1];
    }
    pointX[index][j] = facePoints[0];
    pointY[index][j] = facePoints[1];
    
    splinePoints[index] = getSplinePoints(pointX[index], pointY[index], BASED_ON_X[index],
                                          SECTION_POINT_NUM[index], interpolateNum);
}


vector<int> FaceSpline::findSection(int pointIndex) {
    vector<int> indexes;
    
    // 边界点
    for (int i = 0; i < SPLINE_SECTIONS; i++) {
        if (pointIndex == POINT_INDEX[i]) {
            indexes.push_back((i-1+SPLINE_SECTIONS)%SPLINE_SECTIONS);
            indexes.push_back(i);
            return indexes;
        }
    }
    
    // 样条函数中间点
    for (int i = 0; i < SPLINE_SECTIONS-1; i++) {
        if (pointIndex > POINT_INDEX[i] && pointIndex < POINT_INDEX[i+1]) {
            indexes.push_back(i);
            return indexes;
        }
    }
    if (pointIndex > POINT_INDEX[SPLINE_SECTIONS-1] && pointIndex <= FACE_POINTS) {
        indexes.push_back(SPLINE_SECTIONS-1);
    }
    if (pointIndex > FACE_POINTS) {
        assert(false && "pointIndex out of range");
    }
    
    return indexes;
}


bool FaceSpline::checkPointInRange(int pointIndex, int x, int y) {
    int lastIndex = (pointIndex-1+FACE_POINTS) % FACE_POINTS;
    int nextIndex = (pointIndex+1+FACE_POINTS) % FACE_POINTS;
    
    cout << "FaceSpline checkPoint: " << pointIndex << endl;
    
    // 边界点
    if (pointIndex == POINT_INDEX[0]) {    // 0
        if (facePoints[lastIndex*2+1] >= y || facePoints[nextIndex*2+1] <= y) {
            return false;
        } else {
            return true;
        }
    } else if (pointIndex == POINT_INDEX[1]) {   // 3
        if (facePoints[lastIndex*2+1] >= y || facePoints[nextIndex*2] <= x) {
            return false;
        } else {
            return true;
        }
    } else if (pointIndex == POINT_INDEX[2]) {   // 6
        if (facePoints[lastIndex*2] >= x || facePoints[nextIndex*2] <= x) {
            return false;
        } else {
            return true;
        }
    } else if (pointIndex == POINT_INDEX[3]) {   // 9
        if (facePoints[lastIndex*2] >= x || facePoints[nextIndex*2+1] >= y) {
            return false;
        } else {
            return true;
        }
    } else if (pointIndex == POINT_INDEX[4]) {   // 12
        if (facePoints[lastIndex*2+1] <= y || facePoints[nextIndex*2+1] >= y) {
            return false;
        } else {
            return true;
        }
    } else if (pointIndex == POINT_INDEX[5]) {   // 15
        if (facePoints[lastIndex*2+1] <= y || facePoints[nextIndex*2] >= x) {
            return false;
        } else {
            return true;
        }
    } else if (pointIndex == POINT_INDEX[6]) {   // 17
        if (facePoints[lastIndex*2] <= x || facePoints[nextIndex*2+1] <= y) {
            return false;
        } else {
            return true;
        }
    }
    
    vector<int> indexes = findSection(pointIndex);
    assert(indexes.size() == 1);
    
    cout << "find: " << indexes[0] << endl;
    switch (indexes[0]) {
        case 0:
            if (facePoints[lastIndex*2+1] >= y || facePoints[nextIndex*2+1] <= y) {
                return false;
            }
            break;
        case 1:
            if (facePoints[lastIndex*2] >= x || facePoints[nextIndex*2] <= x) {
                return false;
            }
            break;
        case 2:
            if (facePoints[lastIndex*2] >= x || facePoints[nextIndex*2] <= x) {
                return false;
            }
            break;
        case 3:
            if (facePoints[lastIndex*2+1] <= y || facePoints[nextIndex*2+1] >= y) {
                return false;
            }
            break;
        case 4:
            if (facePoints[lastIndex*2+1] <= y || facePoints[nextIndex*2+1] >= y) {
                return false;
            }
            break;
        case 5:
            if (facePoints[lastIndex*2] <= x || facePoints[nextIndex*2] >= x) {
                return false;
            }
            break;
        case 6:
            if (facePoints[lastIndex*2+1] >= y || facePoints[nextIndex*2+1] <= y) {
                return false;
            }
            break;
            
        default:
            assert(false);
            break;
    }
    return true;
    
}


void FaceSpline::adjustPoint(int pointIndex, int x, int y) {
    facePoints[pointIndex*2] = x;
    facePoints[pointIndex*2+1] = y;
    vector<int> indexes = findSection(pointIndex);
    
    for (int i = 0; i < indexes.size(); i++) {
        int index = indexes[i];
        if (index < SPLINE_SECTIONS-1) {
            if (index == 3 || index == 4 || index == 5) {
                for (int ii = POINT_INDEX[index+1], j = 0; ii >= POINT_INDEX[index]; ii--, j++) {
                    pointX[index][j] = facePoints[ii*2];
                    pointY[index][j] = facePoints[ii*2+1];
                }
                splinePoints[index] = getSplinePoints(pointX[index], pointY[index],
                                                      BASED_ON_X[index], SECTION_POINT_NUM[index], -1);
            } else {
                for (int ii = POINT_INDEX[index], j = 0; ii <= POINT_INDEX[index + 1]; ii++, j++) {
                    pointX[index][j] = facePoints[ii * 2];
                    pointY[index][j] = facePoints[ii * 2 + 1];
                }
                splinePoints[index] = getSplinePoints(pointX[index], pointY[index],
                                                      BASED_ON_X[index], SECTION_POINT_NUM[index], -1);
            }
        } else if (index == SPLINE_SECTIONS-1) {
            int j = 0;
            for (int ii = POINT_INDEX[index]; ii < FACE_POINTS; ii++, j++) {
                pointX[index][j] = facePoints[ii*2];
                pointY[index][j] = facePoints[ii*2+1];
                
            }
            pointX[index][j] = facePoints[0];
            pointY[index][j] = facePoints[1];
            splinePoints[index] = getSplinePoints(pointX[index], pointY[index],
                                                  BASED_ON_X[index], SECTION_POINT_NUM[index], -1);
        } else {
            assert(false && "this should not happen");
        }
    }
}


vector<vector<double>> FaceSpline::getSplinePoints() {
    return this->splinePoints;
}


/**
 * x,y: 分别是插值点的坐标，用于插值的方向要事先按照升序排列.
 * basedOnX: 表示是要按照x轴方向插值，还是按照y轴方向
 * size: x,y数组的长度
 */
vector<double> FaceSpline::getSplinePoints(double *x, double *y, bool basedOnX, int size, int interpolateNum) {
    
    if (basedOnX) {
        vector<double> X, Y;
        for (int i = 0; i < size; i++) {
            X.push_back(x[i]);
            Y.push_back(y[i]);
        }
        
        spline s;
        s.set_points(X, Y);
        if (interpolateNum == -1) {
            vector<double> tempPoints;
            for (double col = x[0]; col < x[size-1]; col += 0.1) {
                tempPoints.push_back(col);
                tempPoints.push_back(s(col));
            }
            return tempPoints;
        } else {
            // 增长的步长
            vector<double> tempPoints;
            double step = (x[size-1] - x[0]) / (double)((size-1)*(interpolateNum-1));
            for (double col = x[0]; col < x[size-1]; col += step) {
                tempPoints.push_back(col);
                tempPoints.push_back(s(col));
            }
            return tempPoints;
        }
    } else {
        vector<double> X, Y;
        for (int i = 0; i < size; i++) {
            X.push_back(y[i]);
            Y.push_back(x[i]);
        }
        spline s;
        s.set_points(X, Y);
        
        if (interpolateNum == -1) {
            vector<double> tempPoints;
            for (double row = y[0]; row < y[size-1]; row+= 0.1) {
                tempPoints.push_back(s(row));
                tempPoints.push_back(row);
            }
            return tempPoints;
        } else {
            vector<double> tempPoints;
            double step = (y[size-1] - y[0]) / (double)((size-1)*(interpolateNum-1));
            for (double row = y[0]; row < y[size-1]; row += step) {
                tempPoints.push_back(s(row));
                tempPoints.push_back(row);
            }
            return tempPoints;
        }
    }
}



Mat FaceSpline::getSplinePoints(Mat &asmPoints, int interpolationNum) {
    assert(asmPoints.cols == FACE_POINTS && asmPoints.rows == 2 && asmPoints.type() == CV_32F);
    
    int face[FACE_POINTS*2];
    for (int col = 0; col < asmPoints.cols; col++) {
        face[col*2] = asmPoints.at<float>(0, col);
        face[col*2+1] = asmPoints.at<float>(1, col);
    }
    
    FaceSpline faceSpline;
    faceSpline.init(face, FACE_POINTS*2, interpolationNum);
    vector<vector<double> > splinePoints = faceSpline.getSplinePoints();
    
    // 总共插值了几个点
    int interpolationSum = 0;
    for (int i = 0; i < splinePoints.size(); i++) {
        interpolationSum += splinePoints[i].size() / 2;
    }
    Mat ret(2, interpolationSum, CV_32F, Scalar::all(0));
    int c = 0;
    for (int i = 0; i < splinePoints.size(); i++) {
        for (int j = 0; j < splinePoints[i].size(); j+=2) {
            ret.at<float>(0, c) = splinePoints[i][j];
            ret.at<float>(1, c) = splinePoints[i][j+1];
            c++;
        }
    }
    return ret;
    
}
