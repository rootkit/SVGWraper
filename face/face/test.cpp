//
//  test.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <stdio.h>
#include "SVGWrap.h"
#include "FiltUtil.h"

int main(int argc, const char * argv[]) {
    
    // 用户输入数据
    int targetDataIndex = 17;
    string targetFile = to_string(targetDataIndex) + ".png";
    //Mat targetMat = imread(targetFile, IMREAD_GRAYSCALE);
    Mat targetMat = imread(targetFile, IMREAD_UNCHANGED);
    // 数据库匹配数据
    int testDataIndex = 1;
    string testFile = to_string(testDataIndex) + ".png";
    Mat testMat = imread(testFile, IMREAD_UNCHANGED);
    // 读取数据库的asm数据
    vector<vector<Point> > dataPoints = FileUtil::read_all_asm_points("all-asmpoints.txt");
    // 读取数据库的svg脸型数据
    vector<vector<Point> > dataSVGs = FileUtil::read_svg_points("allFaceSvgPoint.txt");
    
    vector<Point> targetPoints = dataPoints[targetDataIndex];
    vector<Point> testPoints = dataPoints[testDataIndex];
    vector<Point> testSvg = dataSVGs[testDataIndex];
    
    // 归一化人脸数据点
    DataProc::normalize_face_data(testPoints, targetPoints);
    
    SVGWrap svgWrap(testPoints, targetPoints, testSvg);
    Mat temp = svgWrap.draw_src_triangular(testMat);
    namedWindow("tri");
    imshow("tri", temp);
    imwrite("srcTri.jpg", temp);
    waitKey();
    temp = svgWrap.draw_dest_triangular(targetMat);
    imshow("tri", temp);
    imwrite("destTri.jpg", temp);
    waitKey();
    
    Mat morphingMat = svgWrap.morphing_img(testMat, testPoints, targetPoints);
    namedWindow("morphing");
    imshow("morphing", morphingMat);
    imwrite("morphingMat.jpg", morphingMat);
    waitKey();
    
    string dir = to_string(testDataIndex) + "-" + to_string(targetDataIndex);
    string cmd = "mkdir " + dir;
    system(cmd.c_str());
}
