//
//  Morphing.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Morphing.h"
#include "DrawUtil.h"

int Morphing::POINT_INDEX[31][3] = {
    {78, 79, 2},  {81, 10, 80},
    {2, 79, 3},   {10, 80, 9},
    {3, 79, 4},   {9, 80, 8},
    {4, 79, 5},   {8, 80, 7},
    {5, 79, 6},   {7, 80, 6},
    {78, 15, 14}, {81, 13, 14},
    {78, 15, 2},  {81, 13, 10},
    {15, 2, 3},   {13, 10, 9},
    {3, 4, 59},   {9, 8, 65},
    {59, 4, 5},   {65, 8, 7},
    {74, 5, 6},   {74, 6, 7},
    {15, 3, 59},  {13, 9, 65},
    {59, 74, 5},  {74, 7, 65},
    {15, 14, 59}, {13, 14, 65},
    {14, 59, 74}, {14, 65, 74},
    {78, 14, 81}
};


/**
 * The total size of points should be 82.
 * The first 78 points are asm points.
 * The last 4 points are four vertexes of boundary and
 * the order should be tl, bl, br, tr.
 * What's more, the position of points are better to be normalized
 */
Morphing::Morphing(vector<Point> &srcPoints, vector<Point> &destPoints) {
    assert(srcPoints.size() == 82 && destPoints.size() == 82);
    for (int i = 0; i < 31; i++) {
        vector<Point> src;
        src.push_back(srcPoints[POINT_INDEX[i][0]]);
        src.push_back(srcPoints[POINT_INDEX[i][1]]);
        src.push_back(srcPoints[POINT_INDEX[i][2]]);
        Triangular srcTri(src);

        vector<Point> dest;
        dest.push_back(destPoints[POINT_INDEX[i][0]]);
        dest.push_back(destPoints[POINT_INDEX[i][1]]);
        dest.push_back(destPoints[POINT_INDEX[i][2]]);
        Triangular destTri(dest);
        
        srcTri.set_dest_points(dest);
        destTri.set_dest_points(src);
        
        this->srcTris.push_back(srcTri);
        this->destTris.push_back(destTri);
    }
}

bool Morphing::findSrcTri(Point p, Triangular &t) {
    for (auto tri : this->srcTris) {
        if (tri.is_in_triangular(p)) {
            t = tri;
            return true;
        }
    }
    return false;
}

bool Morphing::findDestTri(Point p, Triangular &t) {
    for (auto tri : this->destTris) {
        if (tri.is_in_triangular(p)) {
            t = tri;
            return true;
        }
    }
    return false;
}

Mat Morphing::scale_mat(Mat &mat, vector<Point> &points) {
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

void Morphing::start_morphing(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints) {
    assert(srcPoints.size() == 82 && destPoints.size() == 82);
    Mat faceMat(destPoints[80].x+100, destPoints[80].y+100, mat.type(), Scalar::all(0));
    
    Mat scaleMat = scale_mat(mat, srcPoints);

    // 将头像调整到与srcPoints一致的位置
    scaleMat.copyTo(faceMat(Rect(srcPoints[78].x, srcPoints[78].y, scaleMat.cols, scaleMat.rows)));
    
//    for (auto t : this->srcTris) {
//        DrawUtil::draw_triangular(faceMat, t, Point(0, 0));
//    }
    
    cvtColor(faceMat, faceMat, CV_BGR2GRAY);
//    namedWindow("scale");
//    imshow("scale", faceMat);
//    imwrite("scale.jpg", faceMat);
//    waitKey();
    
    Mat morphingMat(destPoints[80].x+100, destPoints[80].y+100, faceMat.type(), Scalar::all(0));
    for (int row = destPoints[78].y; row <= destPoints[79].y; row++) {
        for (int col = destPoints[78].x; col <= destPoints[81].x; col++) {
            Triangular t;
            Point p(col, row);
            if (this->findDestTri(p, t)) {
                Point srcP = t.affine.transform(p);
                morphingMat.at<uchar>(row, col) = faceMat.at<uchar>(srcP.y,srcP.x);
            }
        }
    }
    
    namedWindow("morphing");
    imshow("morphing", morphingMat);
    imwrite("morphingMat.jpg", morphingMat);
    waitKey();
    
}

void Morphing::morph_bezier(vector<Point> &bezier) {
    for (int i = 0; i < bezier.size(); i++) {
        Triangular t;
        if (findSrcTri(bezier[i], t)) {
            bezier[i] = t.affine.transform(bezier[i]);
        }
    }
    
}
