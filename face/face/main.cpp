//
//  main.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <iostream>
#include <vector>
#include "FiltUtil.h"
#include "DataProc.h"
#include "Morphing.h"
#include "DrawUtil.h"
#include "BezierUtil.h"
#include "MySpline.h"

#include <opencv2/opencv.hpp>
using namespace cv;

Mat scale_mat_to_dots(Mat mat, vector<Point> &points);
void draw_src_triangular(Mat &testMat, vector<Point> &testPoints, Morphing &morphing);
void draw_dest_triangular(Mat &targetMat, vector<Point> &targetPoints, Morphing &morphing);
void resize(vector<Point> &testBezier, vector<Point> &testPoints, double &scaleSVG);
Mat draw_triangular_on_svg(vector<Point> &bezier, vector<Triangular> &tris);

int main(int argc, const char * argv[]) {
    
    // 用户输入数据
    int targetDataIndex = 0;
    // 数据库匹配数据
    int testDataIndex = 9;
    
    // 读取数据库的asm数据
    vector<vector<Point> > dataPoints = FileUtil::read_all_asm_points("dataBase(old).txt");
    // 读取数据库的svg脸型数据
    vector<vector<Point> > dataSVGs = FileUtil::read_svg_points("allFaceSvgPoint.txt");
    
    vector<Point> testPoints = dataPoints[testDataIndex];
    vector<Point> targetPoints = dataPoints[targetDataIndex];
    
    double *x = new double[16];
    double *y = new double[16];
    for (int i = 0; i < 16; i++) {
        x[i] = testPoints[i].x;
        y[i] = testPoints[i].y;
    }
    MySpline spline;
    spline.init(x, y, 16);
    int* points = spline.splineInterpolate();
    
    for (int i = 0; i < 16; i++) {
        testPoints.push_back(Point(points[i*2], points[i*2+1]));
    }
    
    Mat img(1000, 1000, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < testPoints.size(); i++) {
        DrawUtil::draw_point(img, testPoints[i].x, testPoints[i].y, 10);
    }
    imwrite("test.png", img);
    
    
}













