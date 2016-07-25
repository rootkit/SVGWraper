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
    
    SVGWrap svgWrap(testPoints, targetPoints, testSvg);
}
