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
#include "MySpline.h"

int main(int argc, const char * argv[]) {
    
    // 用户输入数据
    int targetDataIndex = 3;
    string targetFile = to_string(targetDataIndex) + ".png";
    //Mat targetMat = imread(targetFile, IMREAD_GRAYSCALE);
    Mat targetMat = imread(targetFile, IMREAD_UNCHANGED);
    // 数据库匹配数据
    int testDataIndex = 2;
    string testFile = to_string(testDataIndex) + ".png";
    Mat testMat = imread(testFile, IMREAD_UNCHANGED);
    // 读取数据库的asm数据
    vector<vector<Point> > dataPoints = FileUtil::read_all_asm_points("dataBase.txt");
    // 读取数据库的svg脸型数据
    vector<vector<Point> > dataSVGs = FileUtil::read_svg_points("allFaceSvgPoint.txt");
    
    //vector<Point> targetPoints = dataPoints[targetDataIndex];
    vector<Point> targetPoints = FileUtil::read_test_data("data.txt");
    vector<Point> testPoints = dataPoints[testDataIndex];
    vector<Point> testSvg = dataSVGs[testDataIndex];
    
//    vector<double> x, y;
//    for (int i = 0; i < testPoints.size(); i++) {
//        x.push_back(testPoints[i].x);
//        y.push_back(testPoints[i].y);
//    }
//    MySpline spline(x, y);
//    Mat img(1000, 1000, CV_8UC1, Scalar::all(0));
//    spline.draw_face_contour(img);
//    namedWindow("face");
//    imshow("face", img);
//    imwrite("face.jpg", img);
//    waitKey();
    
//    for (int i = 0; i < 78; i++) {
//        cout << "(" << testPoints[i].x << "," << testPoints[i].y << ")" << endl;
//        cout << "(" << targetPoints[i].x << "," << targetPoints[i].y << ")" << endl;
//    }
    
    
    SVGWrap svgWrap(testPoints, targetPoints, testSvg);
    Mat temp = svgWrap.draw_src_triangular(testMat);
    namedWindow("bezier");
    imshow("bezier", temp);
    imwrite("srcTri.jpg", temp);
    waitKey();
    temp = svgWrap.draw_dest_triangular(targetMat);
    imshow("bezier", temp);
    imwrite("destTri.jpg", temp);
    waitKey();
    
    // 形变后的svg控制点
    vector<Point> ctrl = svgWrap.regain_svg_ctrl_points();
    cout << svgWrap.get_relative_path(svgWrap.get_relative_path_points(ctrl)) << endl;
    
    Mat m(3000, 3000, CV_8UC3, Scalar::all(0));
    
    BezierUtil::draw_bezier(m, ctrl, Scalar(255, 255, 255));
    for (auto p : svgWrap.destPoints) {
        DrawUtil::draw_point(m, p.x, p.y, 7, Scalar(0, 0, 255));
    }
    namedWindow("bezier");
    imshow("bezier", m);
    imwrite(("bezierFor"+to_string(targetDataIndex)+".jpg").c_str(), m);
    waitKey();
    
    // 绘制原测试数据的贝塞尔曲线
    m = Scalar(0, 0, 0);
    BezierUtil::draw_bezier(m, svgWrap.tempSvg);
    for (auto p : svgWrap.srcPoints) {
        DrawUtil::draw_point(m, p.x, p.y, 7, Scalar(0, 0, 255));
    }
    namedWindow("bezier");
    imshow("bezier", m);
    imwrite("bezier"+to_string(testDataIndex)+".jpg", m);
    waitKey();
    
    string dir = to_string(testDataIndex) + "-" + to_string(targetDataIndex);
    
    string cmd = "mkdir " + dir;
    system(cmd.c_str());
}
