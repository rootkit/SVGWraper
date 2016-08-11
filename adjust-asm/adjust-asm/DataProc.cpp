//
//  DataProc.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "DataProc.h"


void DataProc::normalizeFaceData(vector<Point> &testPoints, vector<Point> &targetPoints) {

    // get the width and height of the target face
    int maxX = -100000, minX = 100000,
    maxY = targetPoints[6].y, minY = targetPoints[77].y;
    for (int i = 0; i < 13; i++) {
        if (minX > targetPoints[i].x) {
            minX = targetPoints[i].x;
        }
        if (maxX < targetPoints[i].x) {
            maxX = targetPoints[i].x;
        }
    }
    double targetW = maxX - minX;
    double targetH = maxY - minY;
    
    targetPoints.push_back(Point(minX, minY));
    targetPoints.push_back(Point(minX, maxY));
    targetPoints.push_back(Point(maxX, maxY));
    targetPoints.push_back(Point(maxX, minY));
    
    // get the width and height of the test face
    maxY = testPoints[6].y, minY = testPoints[77].y;
    maxX = -100000;
    minX = 100000;
    for (int i = 0; i < 13; i++) {
        if (minX > testPoints[i].x) {
            minX = testPoints[i].x;
        }
        if (maxX < testPoints[i].x) {
            maxX = testPoints[i].x;
        }
    }
    
    double testW = maxX - minX;
    double testH = maxY - minY;
    
    testPoints.push_back(Point(minX, minY));
    testPoints.push_back(Point(minX, maxY));
    testPoints.push_back(Point(maxX, maxY));
    testPoints.push_back(Point(maxX, minY));
    
    // scale the testDots to the same size of targetDots
    // either on width or on height
    double scaleW = targetW / testW;
    double scaleH = targetH / testH;
    
    double minScale = scaleH <= scaleW ? scaleH : scaleW;
    
    for (int i = 0; i < testPoints.size(); i++) {
        testPoints[i].x *= minScale;
        testPoints[i].y *= minScale;
    }
    
    // align the center of testDots to center of targetDots
    Point targetCenter((targetPoints[78].x+targetPoints[81].x)/2,
                     (targetPoints[78].y+targetPoints[79].y)/2);
    Point testCenter((testPoints[78].x+testPoints[81].x)/2,
                   (testPoints[78].y+testPoints[79].y)/2);
    Point translateV(targetCenter.x-testCenter.x, targetCenter.y-testCenter.y);
    for (int i = 0; i < testPoints.size(); i++) {
        testPoints[i].x += translateV.x;
        testPoints[i].y += translateV.y;
    }
    
}

void DataProc::alignAsmToSvg(vector<Point> &face, const vector<Point> &bezier,
                             double &scale, Point &translate) {
    assert(face.size() == 77);
    int maxX = -100000, minX = 100000, maxY = face[6].y, minY = face[14].y;
    for (int i = 0; i < 13; i++) {
        if (minX > face[i].x) {
            minX = face[i].x;
        }
        if (maxX < face[i].x) {
            maxX = face[i].x;
        }
    }
    double faceWidth = maxX - minX, faceHeight = maxY - minY;
    
    minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    // 贝塞尔曲线最低点，对齐用
    int bottom = 0;
    for (int i = 0; i < bezier.size(); i++) {
        if (minX > bezier[i].x) {
            minX = bezier[i].x;
        }
        if (minY > bezier[i].y) {
            minY = bezier[i].y;
        }
        if (maxX < bezier[i].x) {
            maxX = bezier[i].x;
        }
        if (maxY < bezier[i].y) {
            maxY = bezier[i].y;
            bottom = i;
        }
    }
    double svgWidth = maxX - minX, svgHeight = maxY - minY;
    
    double scaleW = svgWidth / faceWidth, scaleH = svgHeight / faceHeight;
    cout << "scaleW: " << scaleW << "   scaleH: " << scaleH << endl;
    //scale = scaleH > scaleW ? scaleH : scaleW;
    scale = scaleW;
    
    for (int i = 0; i < face.size(); i++) {
        face[i].x *= scale;
        face[i].y *= scale;
    }
    
    translate = bezier[bottom] - face[6];
    
    for (int i = 0; i < face.size(); i++) {
        face[i].x += translate.x;
        face[i].y += translate.y;
    }
    
}



void DataProc::alignSvgToAsm(vector<Point> &face, vector<Point> &bezier,
                             double &scale, Point &translate) {
    assert(face.size() == 78);
    int maxX = -100000, minX = 100000, maxY = face[6].y, minY = face[77].y;
    for (int i = 0; i < 13; i++) {
        if (minX > face[i].x) {
            minX = face[i].x;
        }
        if (maxX < face[i].x) {
            maxX = face[i].x;
        }
    }
    double faceWidth = maxX - minX, faceHeight = maxY - minY;
    
    if (minY < 0 || minX < 0) {
        Point translate;
        translate.x = minY < minX ? abs(minY) : abs(minX);
        translate.y = translate.x;
        DataProc::alignPoints(face, translate);
    }
    
    // 脸部太小，则缩放脸部
    if (faceWidth < 100 || faceHeight < 100) {
        double scale = 400.0 / fmin(faceHeight, faceWidth);
        for (int i = 0; i < face.size(); i++) {
            face[i].x *= scale;
            face[i].y *= scale;
        }
        minX *= scale;
        maxX *= scale;
        minY *= scale;
        maxY *= scale;
    }
    
//    if (minY < 0 || minX < 0) {
//        Point translate;
//        translate.x = minY < minX ? abs(minY) : abs(minX);
//        translate.y = translate.x;
//        DataProc::alignPoints(face, translate);
//    }
    
    minX = 10000, minY = 10000, maxX = -10000, maxY = -10000;
    // 贝塞尔曲线最低点，对齐用
    int bottom = 0;
    for (int i = 0; i < bezier.size(); i++) {
        if (minX > bezier[i].x) {
            minX = bezier[i].x;
        }
        if (minY > bezier[i].y) {
            minY = bezier[i].y;
        }
        if (maxX < bezier[i].x) {
            maxX = bezier[i].x;
        }
        if (maxY < bezier[i].y) {
            maxY = bezier[i].y;
            bottom = i;
        }
    }
    double bezierWidth = maxX - minX, bezierHeight = maxY - minY;
    
    double scaleW = faceWidth / bezierWidth, scaleH = faceHeight / bezierHeight;
    cout << "scaleW: " << scaleW << "   scaleH: " << scaleH << endl;
    //scale = scaleH > scaleW ? scaleH : scaleW;
    scale = scaleH;
    
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x *= scale;
        bezier[i].y *= scale;
    }
    
    translate = face[6] - bezier[bottom];
    
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x += translate.x;
        bezier[i].y += translate.y;
    }
}

void DataProc::recoverSvg(vector<Point> &bezier, double &scale, Point &translate) {
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x -= translate.x;
        bezier[i].y -= translate.y;
    }
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x /= scale;
        bezier[i].y /= scale;
    }
}

void DataProc::alignPoints(vector<Point> &points, Point translate) {
    for (int i = 0; i < points.size(); i++) {
        points[i].x += translate.x;
        points[i].y += translate.y;
    }
}


