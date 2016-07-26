//
//  SVGWrap.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "SVGWrap.h"

SVGWrap::SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg)
:srcPoints(src), destPoints(dest), srcSvgPoints(svg), morph(src, dest) {
    this->bezier = BezierUtil::get_bezier(srcSvgPoints);
    // 归一化贝塞尔曲线点
    normalize_bezier();
    wrap_bezier();
}

void SVGWrap::normalize_bezier() {
    int minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    for (int i = 0; i < bezier.size(); i++) {
        if (minX > bezier[i].x) {
            minX = bezier[i].x;
            left = i;
        }
        if (minY > bezier[i].y) {
            minY = bezier[i].y;
            top = i;
        }
        if (maxX < bezier[i].x) {
            maxX = bezier[i].x;
            right = i;
        }
        if (maxY < bezier[i].y) {
            maxY = bezier[i].y;
            bottom = i;
        }
    }
    
    // 缩放
    double bezierHeight = maxY - minY;
    double bezierWidth = maxX - minX;
    double srcHeight = srcPoints[79].y - srcPoints[78].y;
    double srcWidth = srcPoints[81].x - srcPoints[78].x;
    double scaleW = srcWidth / bezierWidth;
    double scaleH = srcHeight / bezierHeight;
    if (scaleW <= scaleH) {
        scaleSVG = scaleW;
    } else {
        scaleSVG = scaleH;
    }
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x *= scaleSVG;
        bezier[i].y *= scaleSVG;
    }
    
    // 平移
    //Point srcBottom((srcPoints[81].x+srcPoints[78].x)/2, (srcPoints[79].y+srcPoints[78].y)/2);
    Point srcBottom(srcPoints[6].x, srcPoints[6].y);
    this->translate.x = srcBottom.x - bezier[bottom].x;
    this->translate.y = srcBottom.y - bezier[bottom].y;
    cout << "srcBottom===>" << srcBottom << endl;
    cout << "bezier bottom====>" << bottom << endl;
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x += translate.x;
        bezier[i].y += translate.y;
    }
}


void SVGWrap::wrap_bezier() {
    // 测试归一化效果
    Mat temp = draw_src_tri_on_svg();
    imshow("bezier", temp);
    imwrite("bezierBeforeMorph1.jpg", temp);
    waitKey();
//    temp = draw_dest_tri_on_svg();
//    imshow("bezier", temp);
//    imwrite("bezierBeforeMorph2.jpg", temp);
//    waitKey();
    
    morph.morph_bezier(bezier);
    
//    temp = draw_src_tri_on_svg();
//    imshow("bezier", temp);
//    imwrite("bezierAfterMorph1.jpg", temp);
//    waitKey();
    temp = draw_dest_tri_on_svg();
    imshow("bezier", temp);
    imwrite("bezierAfterMorph2.jpg", temp);
    waitKey();
}

Mat SVGWrap::scale_mat_to_dots(Mat mat, vector<Point> &points) {
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


Mat SVGWrap::draw_src_tri_on_svg() {
    Mat temp(bezier[bottom].y+100, bezier[right].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < bezier.size(); i++) {
        DrawUtil::draw_point(temp, bezier[i].x, bezier[i].y, 1);
    }

    for (auto t : morph.srcTris) {
        DrawUtil::draw_triangular(temp, t, Point(0,0));
    }
    return temp;
}


Mat SVGWrap::draw_dest_tri_on_svg() {
    Mat temp(bezier[bottom].y+100, bezier[right].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < bezier.size(); i++) {
        DrawUtil::draw_point(temp, bezier[i].x, bezier[i].y, 1);
    }
    
    for (auto t : morph.destTris) {
        DrawUtil::draw_triangular(temp, t, Point(0,0));
    }
    return temp;
}

Mat SVGWrap::draw_src_triangular(const Mat &img) {
    Mat scaleMat = scale_mat_to_dots(img, srcPoints);
    Point translateV(srcPoints[78].x, srcPoints[78].y);
    for (auto t : morph.srcTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    return scaleMat;
}

Mat SVGWrap::draw_dest_triangular(const Mat &img) {
    Mat scaleMat = scale_mat_to_dots(img, destPoints);
    Point translateV(destPoints[78].x, destPoints[78].y);
    for (auto t : morph.destTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    return scaleMat;
}

Mat SVGWrap::morphing_img(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints) {
    return this->morph.morphing_img(mat, srcPoints, destPoints);
}


