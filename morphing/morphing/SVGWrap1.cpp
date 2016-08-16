//
//  SVGWrap1.cpp
//  morphing
//
//  Created by xyz on 16/8/16.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "SVGWrap1.h"

#include <iostream>
using namespace std;

SVGWrap::SVGWrap(int srcPoints[], int destPoints[], int svg[], int pointSize, int svgSize) {
    for (int i = 0; i < pointSize; i += 2) {
        this->srcPoints.push_back(Point(srcPoints[i], srcPoints[i+1]));
        this->destPoints.push_back(Point(destPoints[i], destPoints[i+1]));
    }
    for (int i = 0; i < svgSize; i += 2) {
        this->srcSvgPoints.push_back(Point(svg[i], svg[i+1]));
    }
    normalizeFaceData();
    this->morph.init(this->srcPoints, this->destPoints);
    this->completedBezier = BezierUtil::getBezier2(srcSvgPoints);
    normalizeBezier();
    morphBezier();
}

SVGWrap::SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg)
:srcPoints(src), destPoints(dest), srcSvgPoints(svg) {
    
    // 归一化脸部数据
    normalizeFaceData();
    this->morph.init(srcPoints, destPoints);
    
    completedBezier = BezierUtil::getBezier2(srcSvgPoints);
    
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

    cout << "scaleSVG: " << scaleSVG << endl;
    
    for (int i = 0; i < completedBezier.size(); i++) {
        completedBezier[i].x *= scaleSVG;
        completedBezier[i].y *= scaleSVG;
    }
    
    for (int i = 0; i < srcSvgPoints.size(); i++) {
        srcSvgPoints[i].x *= scaleSVG;
        srcSvgPoints[i].y *= scaleSVG;
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
    //DataProc::alignPoints(bezier, translate);
    DataProc::alignPoints(completedBezier, translate);
    DataProc::alignPoints(srcSvgPoints, translate);
    
}


void SVGWrap::morphBezier() {
    // 测试归一化效果
    Mat temp = drawSrcTriOnSvg();
    imwrite("bezierBeforeMorph.jpg", temp);
    
    morph.morphBezier(completedBezier);
    morph.morphBezier(srcSvgPoints);
    
    temp = drawDestTriOnSvg();
    imwrite("bezierAfterMorph.jpg", temp);
}


Mat SVGWrap::drawSrcTriOnSvg() {
    Mat temp(srcPoints[80].y+100, srcPoints[80].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < srcSvgPoints.size(); i++) {
        DrawUtil::drawPoint(temp, srcSvgPoints[i].x, srcSvgPoints[i].y, 1);
    }
    
    for (auto t : morph.srcTris) {
        DrawUtil::drawTriangle(temp, t, Point(0,0));
    }
    return temp;
}


Mat SVGWrap::drawDestTriOnSvg() {
    Mat temp(destPoints[80].y+100, destPoints[80].x+100, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < srcSvgPoints.size(); i++) {
        DrawUtil::drawPoint(temp, srcSvgPoints[i].x, srcSvgPoints[i].y, 1);
    }
    
    for (auto t : morph.destTris) {
        DrawUtil::drawTriangle(temp, t, Point(0,0));
    }
    return temp;
}


string SVGWrap::getWrapFacePaths() {
    string s = "M";
    s += to_string(srcSvgPoints[0].x) + " " + to_string(srcSvgPoints[0].y) + "C";
    for (int i = 1; i < srcSvgPoints.size(); i++) {
        s += to_string(srcSvgPoints[i].x) + "," + to_string(srcSvgPoints[i].y) + " ";
    }
    s += "z";
    return s;
}