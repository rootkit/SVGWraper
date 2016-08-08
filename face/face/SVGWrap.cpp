//
//  SVGWrap.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "SVGWrap.h"
#include <iostream>
using namespace std;

SVGWrap::SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg)
:srcPoints(src), destPoints(dest), srcSvgPoints(svg) {
    
    // 归一化脸部数据
    normalize_face_data();
    this->morph.init(srcPoints, destPoints);
    
    //this->bezier = BezierUtil::get_bezier(srcSvgPoints);
    BezierUtil::get_bezier(srcSvgPoints, bezier, completedBezier);
    
    // 归一化贝塞尔曲线点
    normalize_bezier();
    wrap_bezier();
    
}

void SVGWrap::normalize_face_data() {
    assert(srcPoints.size() == 78 && destPoints.size() == 78);
    
    normalize_dest_face();
    normalize_src_face();
    
}

void SVGWrap::normalize_dest_face() {
    
    // get the width and height of the target face
    int maxX = -100000, minX = 100000,
    maxY = destPoints[6].y, minY = destPoints[77].y;
    for (int i = 0; i < 13; i++) {
        if (minX > destPoints[i].x) {
            minX = destPoints[i].x;
        }
        if (maxX < destPoints[i].x) {
            maxX = destPoints[i].x;
        }
    }
    destWidth = maxX - minX;
    destHeight = maxY - minY;
    
    // 通过目标图像的宽高调整扩大三角剖分的尺寸
    destDeltaX = (double)maxX;
    destDeltaY = (double)maxY;
    
    destPoints.push_back(Point(minX-destDeltaX, minY-destDeltaY));
    destPoints.push_back(Point(minX-destDeltaX, maxY+destDeltaY));
    destPoints.push_back(Point(maxX+destDeltaX, maxY+destDeltaY));
    destPoints.push_back(Point(maxX+destDeltaX, minY-destDeltaY));
    
    if (destPoints[78].x < 0 || destPoints[78].y < 0) {
        Point translate;
        translate.x = destPoints[78].x < destPoints[78].y ? abs(destPoints[78].x)+100 : abs(destPoints[78].y)+100;
        translate.y = translate.x;
        DataProc::alignPoints(destPoints, translate);
    }
    
    // 若输入样本的宽高太小，要适当放缩
    double destScale = 0;
    if (destHeight < 200 || destWidth < 200) {
        if (destHeight < destWidth) {
            destScale = 300 / destHeight;
        } else {
            destScale = 300 / destWidth;
        }
        destWidth *= destScale;
        destHeight *= destScale;
        for (int i = 0; i < destPoints.size(); i++) {
            destPoints[i].x *= destScale;
            destPoints[i].y *= destScale;
        }
    }
}

void SVGWrap::normalize_src_face() {
    
    // get the width and height of the test face
    int maxY = srcPoints[6].y, minY = srcPoints[77].y, maxX = -100000, minX = 100000;
    for (int i = 0; i < 13; i++) {
        if (minX > srcPoints[i].x) {
            minX = srcPoints[i].x;
        }
        if (maxX < srcPoints[i].x) {
            maxX = srcPoints[i].x;
        }
    }
    
    srcWidth = maxX - minX;
    srcHeight = maxY - minY;
    
    // 通过数据库图像的宽高调整扩大三角剖分的尺寸
    srcDeltaX = (double)srcWidth;
    srcDeltaY = (double)srcHeight;
    
    srcPoints.push_back(Point(minX-srcDeltaX, minY-srcDeltaY));
    srcPoints.push_back(Point(minX-srcDeltaX, maxY+srcDeltaY));
    srcPoints.push_back(Point(maxX+srcDeltaX, maxY+srcDeltaY));
    srcPoints.push_back(Point(maxX+srcDeltaX, minY-srcDeltaY));
    
    if (srcPoints[78].x < 0 || srcPoints[78].y < 0) {
        Point translate;
        translate.x = srcPoints[78].x < srcPoints[78].y ? abs(srcPoints[78].x)+100 : abs(srcPoints[78].y)+100;
        translate.y = translate.x;
        DataProc::alignPoints(srcPoints, translate);
    }

    cout << "destWidth: " << destWidth << endl;
    cout << "destHeight: " << destHeight << endl;
    cout << "srcWidth: " << srcWidth << endl;
    cout << "srcHeight: " << srcHeight << endl;
    
    // scale the testDots to the same size of targetDots
    // either on width or on height
    double scaleW = destWidth / srcWidth;
    double scaleH = destHeight / srcHeight;
    
    double minScale = scaleH <= scaleW ? scaleH : scaleW;
    srcHeight *= minScale;
    srcWidth *= minScale;
    
    cout << "minScale: " << minScale << endl;
    
    for (int i = 0; i < srcPoints.size(); i++) {
        srcPoints[i].x *= minScale;
        srcPoints[i].y *= minScale;
    }
    
    // align the center of testDots to center of targetDots
    Point destCenter((destPoints[78].x+destPoints[81].x)/2,
                     (destPoints[78].y+destPoints[79].y)/2);
    Point srcCenter((srcPoints[78].x+srcPoints[81].x)/2,
                    (srcPoints[78].y+srcPoints[79].y)/2);
    Point translateV(destCenter.x-srcCenter.x, destCenter.y-srcCenter.y);
    
    DataProc::alignPoints(srcPoints, translateV);
}

void SVGWrap::interpolate_face() {
    // 插值脸部轮廓16个点
    double *x = new double[16];
    double *y = new double[16];
    for (int i = 0; i < 16; i++) {
        x[i] = srcPoints[i].x;
        y[i] = srcPoints[i].y;
    }
    this->srcSpline.init(x, y, 16);
    int* points = srcSpline.splineInterpolate();
    for (int i = 0; i < 16; i++) {
        srcPoints.push_back(Point(points[i*2], points[i*2+1]));
    }
    srcPoints.push_back(Point(srcPoints[78].x, srcPoints[1].y));   // 第98个点
    srcPoints.push_back(Point(srcPoints[78].x, srcPoints[3].y));   // 第99个点
    srcPoints.push_back(Point(srcPoints[81].x, srcPoints[11].y));  // 第100个点
    srcPoints.push_back(Point(srcPoints[81].x, srcPoints[9].y));   // 第101个点
    srcPoints.push_back(Point(srcPoints[4].x, srcPoints[79].y));   // 第102个点
    srcPoints.push_back(Point(srcPoints[6].x, srcPoints[79].y));   // 第103个点
    srcPoints.push_back(Point(srcPoints[8].x, srcPoints[79].y));   // 第104个点
    srcPoints.push_back(Point(srcPoints[78].x, srcPoints[97].y));  // 第105个点
    srcPoints.push_back(Point(srcPoints[81].x, srcPoints[94].y));  // 第106个点
    delete [] points;
    
    for (int i = 0; i < 16; i++) {
        x[i] = destPoints[i].x;
        y[i] = destPoints[i].y;
    }
    this->destSpline.init(x, y, 16);
    points = destSpline.splineInterpolate();
    for (int i = 0; i < 16; i++) {
        destPoints.push_back(Point(points[i*2], points[i*2+1]));
    }
    destPoints.push_back(Point(destPoints[78].x, destPoints[1].y));  // 第98个点
    destPoints.push_back(Point(destPoints[78].x, destPoints[3].y));  // 第99个点
    destPoints.push_back(Point(destPoints[81].x, destPoints[11].y)); // 第100个点
    destPoints.push_back(Point(destPoints[81].x, destPoints[9].y));  // 第101个点
    destPoints.push_back(Point(destPoints[4].x, destPoints[79].y));   // 第102个点
    destPoints.push_back(Point(destPoints[6].x, destPoints[79].y));   // 第103个点
    destPoints.push_back(Point(destPoints[8].x, destPoints[79].y));   // 第104个点
    destPoints.push_back(Point(destPoints[78].x, destPoints[97].y));  // 第105个点
    destPoints.push_back(Point(destPoints[81].x, destPoints[94].y));  // 第106个点
    delete [] points;
    
    delete [] x;
    delete [] y;
    x = NULL; y = NULL; points = NULL;
}

void SVGWrap::normalize_bezier() {
    int minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    for (int i = 0; i < completedBezier.size(); i++) {
        if (minX > completedBezier[i].x) {
            minX = completedBezier[i].x;
            left = i;
        }
        if (minY > completedBezier[i].y) {
            minY = completedBezier[i].y;
            top = i;
        }
        if (maxX < completedBezier[i].x) {
            maxX = completedBezier[i].x;
            right = i;
        }
        if (maxY < completedBezier[i].y) {
            maxY = completedBezier[i].y;
            bottom = i;
        }
    }
    
    // 缩放，注意srcPoints的四个边的顶点并代表人脸的真正大小
    double bezierHeight = maxY - minY;
    double bezierWidth = maxX - minX;

    // 直接取高度缩放比
    scaleSVG = srcHeight / bezierHeight;
    cout << "svg scaleSVG: " << scaleSVG << endl;
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x *= scaleSVG;
        bezier[i].y *= scaleSVG;
    }
    
    for (int i = 0; i < completedBezier.size(); i++) {
        completedBezier[i].x *= scaleSVG;
        completedBezier[i].y *= scaleSVG;
    }
    
    
    // 平移，对齐下巴最低点。改进版本：下巴最低点不再默认是第六个asm点，而是先判断第5和第7个是否比第六个底
    Point srcBottom(srcPoints[6].x, srcPoints[6].y);
    // 第五个点比第六个点低的情况
    if (srcPoints[6].y <= srcPoints[5].y) {
        srcBottom.x = (srcPoints[6].x + srcPoints[5].x) / (double)2;
        srcBottom.y = (srcPoints[6].y + srcPoints[5].y) / (double)2;
    }
    // 第七个点是最低点的情况
    if (srcPoints[7].y > srcPoints[6].y && srcPoints[7].y > srcPoints[5].y) {
        srcBottom.x = (srcPoints[6].x + srcPoints[7].x) / (double)2;
        srcBottom.y = (srcPoints[6].y + srcPoints[7].y) / (double)2;
    }
    
    // svg的最低点由completedBezier确定
    this->translate.x = srcBottom.x - completedBezier[bottom].x;
    this->translate.y = srcBottom.y - completedBezier[bottom].y;
    DataProc::alignPoints(bezier, translate);
    DataProc::alignPoints(completedBezier, translate);
    
    // 测试用
    this->tempSvg = this->srcSvgPoints;
    for (int i = 0; i < tempSvg.size(); i++) {
        tempSvg[i].x *= scaleSVG;
        tempSvg[i].y *= scaleSVG;
        tempSvg[i].x += translate.x;
        tempSvg[i].y += translate.y;
    }
}


void SVGWrap::wrap_bezier() {
    // 测试归一化效果
    Mat temp = draw_src_tri_on_svg();
    imwrite("bezierBeforeMorph.jpg", temp);
    
    morph.morph_bezier(bezier);
    
    temp = draw_dest_tri_on_svg();
    imwrite("bezierAfterMorph.jpg", temp);
}

Mat SVGWrap::scale_mat_to_dots(Mat mat, double width, double height) {
    
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
    Mat temp(srcPoints[80].y+100, srcPoints[80].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < bezier.size(); i++) {
        DrawUtil::draw_point(temp, bezier[i].x, bezier[i].y, 5);
    }

    for (auto t : morph.srcTris) {
        DrawUtil::draw_triangular(temp, t, Point(0,0));
    }
    return temp;
}


Mat SVGWrap::draw_dest_tri_on_svg() {
    Mat temp(destPoints[80].y+100, destPoints[80].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < bezier.size(); i++) {
        DrawUtil::draw_point(temp, bezier[i].x, bezier[i].y, 5);
    }
    
    for (auto t : morph.destTris) {
        DrawUtil::draw_triangular(temp, t, Point(0,0));
    }
    return temp;
}

Mat SVGWrap::draw_src_triangular(const Mat &img) {
    Mat scaleMat = scale_mat_to_dots(img, srcWidth, srcHeight);
    Point translateV(srcPoints[78].x+srcDeltaX, srcPoints[78].y+srcDeltaY);
    for (auto t : morph.srcTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    return scaleMat;
}

Mat SVGWrap::draw_dest_triangular(const Mat &img) {
    Mat scaleMat = scale_mat_to_dots(img, destWidth, destHeight);
    Point translateV(destPoints[78].x+destDeltaX, destPoints[78].y+destDeltaY);
    for (auto t : morph.destTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    return scaleMat;
}

Mat SVGWrap::morphing_img(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints) {
    return this->morph.morphing_img(mat, srcPoints, destPoints);
}

vector<Point> SVGWrap::regain_svg_ctrl_points() {
    vector<Point> adjustBezier(bezier);
    
    // 测试需要，先去掉还原(归一化)操作
    // recalculate ctrl points before normalization
//    for (int i = 0; i < adjustBezier.size(); i++) {
//        adjustBezier[i].x -= translate.x;
//        adjustBezier[i].y -= translate.y;
//    }
//    for (int i = 0; i < adjustBezier.size(); i++) {
//        adjustBezier[i].x /= scaleSVG;
//        adjustBezier[i].y /= scaleSVG;
//    }
    
    return BezierUtil::regain_new_points(adjustBezier);
}

vector<Point> SVGWrap::get_relative_path_points(vector<Point> svgCtrlPoints) {
    vector<Point> points;
    points.push_back(svgCtrlPoints[0]);
    for (int i = 1; i < svgCtrlPoints.size()-1; i+=3) {
        Point p0 = svgCtrlPoints[i-1];
        Point p1 = svgCtrlPoints[i];
        Point p2 = svgCtrlPoints[i+1];
        Point p3 = svgCtrlPoints[i+2];
        points.push_back(Point(p1.x-p0.x, p1.y-p0.y));
        points.push_back(Point(p2.x-p0.x, p2.y-p0.y));
        points.push_back(Point(p3.x-p0.x, p3.y-p0.y));
    }
    return points;
}

string SVGWrap::get_relative_path(vector<Point> points) {
    string s = "M";
    s += to_string(points[0].x) + " " + to_string(points[0].y) + "c";
    for (int i = 1; i < points.size(); i++) {
        s += to_string(points[i].x) + "," + to_string(points[i].y) + " ";
    }
    s += "z";
    return s;
}


