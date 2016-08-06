//
//  DrawUtil.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "DrawUtil.h"


bool DrawUtil::is_valid_in(Mat &img, int x, int y) {
    int width = img.cols;
    int height = img.rows;
    return 0 <= x && x < width && 0 <= y && y < height;
}

void DrawUtil::draw_point(Mat &img, int x, int y, int r) {
//    for (int i = x-r; i <= x+r; i++) {
//        for (int j = y-r; j <= y+r; j++) {
//            if (is_valid_in(img, i, j)) {
                circle(img, Point(x, y), r, Scalar(255, 255, 255));
                //img.at<uchar>(j, i) = 0xff;
//            }
//        }
//    }
}

void DrawUtil::draw_point(Mat &img, int x, int y, int r, Scalar color) {
//    for (int i = x-r; i <= x+r; i++) {
//        for (int j = y-r; j <= y+r; j++) {
//            if (is_valid_in(img, i, j)) {
                //img.at<Vec3b>(j, i)[channel] = 0xff;
                circle(img, Point(x, y), r, color);
//            }
//        }
//    }
}

void DrawUtil::draw_points(Mat &img, vector<Point> &points, int r) {
    for (auto p : points) {
        draw_point(img, p.x, p.y, r);
    }
}

void DrawUtil::draw_points(Mat &img, vector<Point> &points, int r, Scalar color) {
    for (auto p : points) {
        draw_point(img, p.x, p.y, r, color);
    }
}





