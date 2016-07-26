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
    for (int i = x-r; i <= x+r; i++) {
        for (int j = y-r; j <= y+r; j++) {
            if (is_valid_in(img, i, j)) {
                circle(img, Point(x, y), 1, Scalar(255, 255, 255));
                //img.at<uchar>(j, i) = 0xff;
            }
        }
    }
}

void DrawUtil::draw_point(Mat &img, int x, int y, int r, Scalar color) {
    for (int i = x-r; i <= x+r; i++) {
        for (int j = y-r; j <= y+r; j++) {
            if (is_valid_in(img, i, j)) {
                //img.at<Vec3b>(j, i)[channel] = 0xff;
                circle(img, Point(x, y), r, color);
            }
        }
    }
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

/**
 * Draw triangular from src points
 */
void DrawUtil::draw_triangular(Mat &img, Triangular &t, Point translate) {
    Point p1(t.srcTris[0].x-translate.x, t.srcTris[0].y-translate.y);
    Point p2(t.srcTris[1].x-translate.x, t.srcTris[1].y-translate.y);
    Point p3(t.srcTris[2].x-translate.x, t.srcTris[2].y-translate.y);
    line(img, p1, p2, Scalar(255, 0, 0));
    line(img, p2, p3, Scalar(255, 0, 0));
    line(img, p3, p1, Scalar(255, 0, 0));
}




