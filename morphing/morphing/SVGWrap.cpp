//
//  SVGWrap.cpp
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "SVGWrap.h"
#include <iostream>
using namespace std;

Mat testFace(2000, 2000, CV_8UC3, Scalar::all(0));

SVGWrap::SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg)
:srcPoints(src), destPoints(dest), srcSvgPoints(svg) {
    
    // 归一化脸部数据
    normalizeFaceData();
    this->morph.init(srcPoints, destPoints);
    
    BezierUtil::getBezier(srcSvgPoints, bezier, completedBezier);
    
    // 归一化贝塞尔曲线点
    normalizeBezier();
    morphBezier();
    
}

void SVGWrap::normalizeFaceData() {
    assert(srcPoints.size() == 78 && destPoints.size() == 78);
    
    normalizeDestFace();
    normalizeSrcFace();
    
}

void SVGWrap::normalizeDestFace() {
    
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
    destDeltaX = (double)maxX*0.5;
    destDeltaY = (double)maxY*0.2;
    
    // 边界点
    destPoints.push_back(Point(minX-destDeltaX, minY-destDeltaY));
    destPoints.push_back(Point(minX-destDeltaX, maxY+destDeltaY));
    destPoints.push_back(Point(maxX+destDeltaX, maxY+destDeltaY));
    destPoints.push_back(Point(maxX+destDeltaX, minY-destDeltaY));
    
    // 边界中间插值点
    destPoints.push_back(Point(destPoints[78].x, (destPoints[78].y+destPoints[79].y)/2));
    destPoints.push_back(Point((destPoints[79].x+destPoints[80].x)/2, destPoints[79].y));
    destPoints.push_back(Point(destPoints[80].x, (destPoints[80].y+destPoints[81].y)/2));
    destPoints.push_back(Point((destPoints[78].x+destPoints[81].x)/2, destPoints[81].y));
    
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
    
    cout << "destPoints===>" << endl;
    cout << destPoints[78] << endl << destPoints[79] << endl << destPoints[80] << endl << destPoints[81] << endl;
    
    DrawUtil::drawPoints(testFace, destPoints, 1, Scalar(255, 255, 0));
}

void SVGWrap::normalizeSrcFace() {
    
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
    srcDeltaX = (double)srcWidth*0.5;
    srcDeltaY = (double)srcHeight*0.2;
    
//    // 边界点
//    srcPoints.push_back(Point(minX-srcDeltaX, minY-srcDeltaY));
//    srcPoints.push_back(Point(minX-srcDeltaX, maxY+srcDeltaY));
//    srcPoints.push_back(Point(maxX+srcDeltaX, maxY+srcDeltaY));
//    srcPoints.push_back(Point(maxX+srcDeltaX, minY-srcDeltaY));
//    
//    // 边界中间插值点
//    srcPoints.push_back(Point(srcPoints[78].x, (srcPoints[78].y+srcPoints[79].y)/2));
//    srcPoints.push_back(Point((srcPoints[79].x+srcPoints[80].x)/2, srcPoints[79].y));
//    srcPoints.push_back(Point(srcPoints[80].x, (srcPoints[80].y+srcPoints[81].y)/2));
//    srcPoints.push_back(Point((srcPoints[78].x+srcPoints[81].x)/2, srcPoints[81].y));
    
//    if (srcPoints[78].x < 0 || srcPoints[78].y < 0) {
//        Point translate;
//        translate.x = srcPoints[78].x < srcPoints[78].y ? abs(srcPoints[78].x)+100 : abs(srcPoints[78].y)+100;
//        translate.y = translate.x;
//        DataProc::alignPoints(srcPoints, translate);
//    }
    
    
    // scale the testDots to the same size of targetDots
    // either on width or on height
    double scaleW = destWidth / srcWidth;
    double scaleH = destHeight / srcHeight;
    
    double minScale = scaleH <= scaleW ? scaleH : scaleW;
    srcHeight *= minScale;
    srcWidth *= minScale;
    
    for (int i = 0; i < srcPoints.size(); i++) {
        srcPoints[i].x *= minScale;
        srcPoints[i].y *= minScale;
    }
    
    // align the center of testDots to center of targetDots
    Point destCenter((destPoints[0].x+destPoints[12].x)/2,
                     (destPoints[6].y+destPoints[77].y)/2);
    Point srcCenter((srcPoints[0].x+srcPoints[12].x)/2,
                    (srcPoints[6].y+srcPoints[77].y)/2);
    Point translateV(destCenter.x-srcCenter.x, destCenter.y-srcCenter.y);
    
    cout << "destCenter" << endl << destCenter << endl;
    cout << "srcCenter" << endl << srcCenter << endl;
    
    DataProc::alignPoints(srcPoints, translateV);
    
    
    // 边界点
    srcPoints.push_back(destPoints[78]);
    srcPoints.push_back(destPoints[79]);
    srcPoints.push_back(destPoints[80]);
    srcPoints.push_back(destPoints[81]);
    
    // 边界中间插值点
    srcPoints.push_back(Point(srcPoints[78].x, (srcPoints[78].y+srcPoints[79].y)/2));
    srcPoints.push_back(Point((srcPoints[79].x+srcPoints[80].x)/2, srcPoints[79].y));
    srcPoints.push_back(Point(srcPoints[80].x, (srcPoints[80].y+srcPoints[81].y)/2));
    srcPoints.push_back(Point((srcPoints[78].x+srcPoints[81].x)/2, srcPoints[81].y));

    
    cout << "srcPoints===>" << endl;
    cout << srcPoints[78] << endl << srcPoints[79] << endl << srcPoints[80] << endl << srcPoints[81] << endl;
    
        DrawUtil::drawPoints(testFace, srcPoints, 1, Scalar(0, 255, 255));
    imwrite("testFace.jpg", testFace);
}

void SVGWrap::normalizeBezier() {
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
//    this->tempSvg = this->srcSvgPoints;
//    for (int i = 0; i < tempSvg.size(); i++) {
//        tempSvg[i].x *= scaleSVG;
//        tempSvg[i].y *= scaleSVG;
//        tempSvg[i].x += translate.x;
//        tempSvg[i].y += translate.y;
//    }
}


void SVGWrap::morphBezier() {
    // 测试归一化效果
        Mat temp = drawSrcTriOnSvg();
        imwrite("bezierBeforeMorph.jpg", temp);
    
    //morph.morphBezier(bezier);
    morph.morphBezier(completedBezier);
    
        temp = drawDestTriOnSvg();
        imwrite("bezierAfterMorph.jpg", temp);
}

Mat SVGWrap::scaleMat(Mat mat, double width, double height) {
    
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


Mat SVGWrap::drawSrcTriOnSvg() {
    Mat temp(srcPoints[80].y+100, srcPoints[80].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < completedBezier.size(); i++) {
        DrawUtil::drawPoint(temp, completedBezier[i].x, completedBezier[i].y, 1);
    }
    
    for (auto t : morph.srcTris) {
        DrawUtil::drawTriangle(temp, t, Point(0,0));
    }
    return temp;
}


Mat SVGWrap::drawDestTriOnSvg() {
    Mat temp(destPoints[80].y+100, destPoints[80].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < completedBezier.size(); i++) {
        DrawUtil::drawPoint(temp, completedBezier[i].x, completedBezier[i].y, 1);
    }
    
    for (auto t : morph.destTris) {
        DrawUtil::drawTriangle(temp, t, Point(0,0));
    }
    return temp;
}


Mat SVGWrap::morphingImg(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints) {
    return this->morph.morphImg(mat, srcPoints, destPoints);
}

vector<Point> SVGWrap::regainSvgCtrlPoints() {
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
    
    return BezierUtil::regainNewPoints(adjustBezier);
}

vector<Point> SVGWrap::getRelativePathPoints(vector<Point> svgCtrlPoints) {
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

string SVGWrap::getRelativePath(vector<Point> points) {
    string s = "M";
    s += to_string(points[0].x) + " " + to_string(points[0].y) + "c";
    for (int i = 1; i < points.size(); i++) {
        s += to_string(points[i].x) + "," + to_string(points[i].y) + " ";
    }
    s += "z";
    return s;
}
