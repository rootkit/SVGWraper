//
//  main.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <iostream>
#include "FiltUtil.h"
#include "DataProc.h"
#include "Morphing.h"
#include "DrawUtil.h"
#include "BezierUtil.h"

#include <opencv2/opencv.hpp>
using namespace cv;

Mat scale_mat_to_dots(Mat mat, vector<Point> &points);
void draw_src_triangular(Mat &testMat, vector<Point> &testPoints, Morphing &morphing);
void draw_dest_triangular(Mat &targetMat, vector<Point> &targetPoints, Morphing &morphing);
void resize(vector<Point> &testBezier, vector<Point> &testPoints, double &scaleSVG);
Mat draw_triangular_on_svg(vector<Point> &bezier, vector<Triangular> &tris);

int main(int argc, const char * argv[]) {
    // 用户输入数据
    int targetDataIndex = 14;
    string targetFile = "14.png";
    //Mat targetMat = imread(targetFile, IMREAD_GRAYSCALE);
    Mat targetMat = imread(targetFile, IMREAD_UNCHANGED);
    // 数据库匹配数据
    int testDataIndex = 1;
    string testFile = "1.png";
    Mat testMat = imread(testFile, IMREAD_UNCHANGED);
    // 读取数据库的asm数据
    vector<vector<Point> > dataPoints = FileUtil::read_all_asm_points("all-asmpoints.txt");
    // 读取数据库的svg脸型数据
    vector<vector<Point> > dataSVGs = FileUtil::read_svg_points("allFaceSvgPoint.txt");
    
    vector<Point> targetPoints = dataPoints[targetDataIndex];
    vector<Point> testPoints = dataPoints[testDataIndex];
    vector<Point> testSvg = dataSVGs[testDataIndex];
    
    DataProc::normalize_face_data(testPoints, targetPoints);
    
    Morphing morphing(testPoints, targetPoints);
    
    draw_src_triangular(testMat, testPoints, morphing);
    draw_dest_triangular(targetMat, targetPoints, morphing);
    
    // 测试人脸图形变效果
    morphing.morphing_img(testMat, testPoints, targetPoints);
    
    // 由testSvg得到贝塞尔曲线的点
    double scaleSVG;
    vector<Point> testBezier = BezierUtil::get_bezier(testSvg);
    resize(testBezier, testPoints, scaleSVG);
    Mat temp = draw_triangular_on_svg(testBezier, morphing.srcTris);
    imshow("bezier", temp);
    imwrite("bezierBeforeMorph1.jpg", temp);
    waitKey();
    temp = draw_triangular_on_svg(testBezier, morphing.destTris);
    imshow("bezier", temp);
    imwrite("bezierBeforeMorph2.jpg", temp);
    waitKey();
    morphing.morph_bezier(testBezier);
    temp = draw_triangular_on_svg(testBezier, morphing.srcTris);
    imshow("bezier", temp);
    imwrite("bezierAfterMorph1.jpg", temp);
    waitKey();
    temp = draw_triangular_on_svg(testBezier, morphing.destTris);
    imshow("bezier", temp);
    imwrite("bezierAfterMorph2.jpg", temp);
    waitKey();
    
}

Mat scale_mat_to_dots(Mat mat, vector<Point> &points) {
    assert(points.size() == 82);
    double width = points[81].x - points[78].x;
    double height = points[79].y - points[78].y;
    
    double scaleW = width / mat.cols;
    double scaleH = height / mat.rows;
    //    double minScale = scaleH <= scaleW ? scaleH : scaleW;
    
    Mat scaleMat;
    
    if (scaleW <= scaleH) {
        resize(mat, scaleMat, Size(mat.cols*width / mat.cols, mat.rows*width / mat.cols));
    } else {
        resize(mat, scaleMat, Size(mat.cols*height / mat.rows, mat.rows*height / mat.rows));
    }

    return scaleMat;
}
void draw_src_triangular(Mat &testMat, vector<Point> &testPoints, Morphing &morphing) {
    Mat scaleMat = scale_mat_to_dots(testMat, testPoints);
    Point translateV(testPoints[78].x, testPoints[78].y);
    for (auto t : morphing.srcTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    namedWindow("triangular");
    imshow("triangular", scaleMat);
    waitKey();
    imwrite("triangular_test.jpg", scaleMat);
}
void draw_dest_triangular(Mat &targetMat, vector<Point> &targetPoints, Morphing &morphing) {
    Mat scaleMat = scale_mat_to_dots(targetMat, targetPoints);
    Point translateV(targetPoints[78].x, targetPoints[78].y);
    for (auto t : morphing.destTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    namedWindow("triangular");
    imshow("triangular", scaleMat);
    waitKey();
    imwrite("triangular_target.jpg", scaleMat);
    
}

void resize(vector<Point> &testBezier, vector<Point> &testPoints, double &scaleSVG) {
    int minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    for (int i = 0; i < testBezier.size(); i++) {
        if (minX > testBezier[i].x) {
            minX = testBezier[i].x;
        }
        if (minY > testBezier[i].y) {
            minY = testBezier[i].y;
        }
        if (maxX < testBezier[i].x) {
            maxX = testBezier[i].x;
        }
        if (maxY < testBezier[i].y) {
            maxY = testBezier[i].y;
        }
    }
    double bezierHeight = maxY - minY;
    double bezierWidth = maxX - minX;
    double testDotHeight = testPoints[79].y - testPoints[78].y;
    double testDotWidth = testPoints[81].x - testPoints[78].x;
    double scaleW = testDotWidth / bezierWidth;
    double scaleH = testDotHeight / bezierHeight;
    
    cout << "SVG scaleW: " << scaleW << "  scaleH: " << scaleH << endl;
    if (scaleW <= scaleH) {
        scaleSVG = scaleW;
    } else {
        scaleSVG = scaleH;
    }
    for (int i = 0; i < testBezier.size(); i++) {
        testBezier[i].x *= scaleSVG;
        testBezier[i].y *= scaleSVG;
    }
    Point bezierCenter((maxX+minX)*scaleSVG/2, (maxY+minY)*scaleSVG/2);
    Point testDotCenter((testPoints[81].x+testPoints[78].x)/2, (testPoints[79].y+testPoints[78].y)/2);
    Point translateV(testDotCenter.x-bezierCenter.x, testDotCenter.y-bezierCenter.y);
    for (int i = 0; i < testBezier.size(); i++) {
        testBezier[i].x += translateV.x;
        testBezier[i].y += translateV.y;
    }
}

Mat draw_triangular_on_svg(vector<Point> &bezier, vector<Triangular> &tris) {
    int minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    for (int i = 0; i < bezier.size(); i++) {
        if (minX > bezier[i].x) {
            minX = bezier[i].x;
        }
        if (minY > bezier[i].y) {
            minY = bezier[i].y;
        }
        if (maxX < bezier[i].x) {
            maxX = bezier[i].x;
        }
        if (maxY < bezier[i].y) {
            maxY = bezier[i].y;
        }
    }
    Mat temp(maxY+100, maxX+100, CV_8UC1);
    for (int row = 0; row < temp.rows; ++row) {
        for (int col = 0; col < temp.cols; ++col) {
            temp.at<uchar>(row, col) = 0;
        }
    }
    for (int i = 0; i < bezier.size(); i++) {
        DrawUtil::draw_point(temp, bezier[i].x, bezier[i].y, 1);
    }
    
    Mat temp1(temp);
    for (auto t : tris) {
        DrawUtil::draw_triangular(temp1, t, Point(0,0));
    }
    return temp1;
    
}











