//
//  test_affine.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <stdio.h>
#include <vector>
#include "AffineTransform.h"
#include "Triangular.h"
#include "DrawUtil.h"

using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    Mat m1(100, 100, CV_8UC1);
    Mat m2(100, 100, CV_8UC1);
    vector<Point> srcTri;
    srcTri.push_back(Point(10, 10));
    srcTri.push_back(Point(20, 30));
    srcTri.push_back(Point(70, 30));
    Triangular t1(srcTri);
    
    vector<Point> destTri;
    destTri.push_back(Point(20, 10));
    destTri.push_back(Point(50, 30));
    destTri.push_back(Point(60, 80));
    t1.set_dest_points(destTri);
    
    Triangular t2(destTri);
    t2.set_dest_points(srcTri);
    
    DrawUtil::draw_points(m1, srcTri, 3);
    DrawUtil::draw_points(m2, destTri, 3);
    
    for (int row = 0; row < m1.rows; row++) {
        for (int col = 0; col < m1.cols; col++) {
            if (t1.is_in_triangular(Point(col, row))) {
                DrawUtil::draw_point(m1, col, row, 1);
            }
        }
    }
    namedWindow("m1");
    imshow("m1", m1);
    imwrite("m1.jpg", m1);
    waitKey();
    
    for (int row = 0; row < m2.rows; row++) {
        for (int col = 0; col < m2.cols; col++) {
            Point p(col, row);
            if (t2.is_in_triangular(p)) {
                if (t1.is_in_triangular(t2.affine.transform(p))) {
                    DrawUtil::draw_point(m2, col, row, 1);
                }
            }
        }
    }
    namedWindow("m2");
    imshow("m2", m2);
    imwrite("m2.jpg", m2);
    waitKey();
    
    
}