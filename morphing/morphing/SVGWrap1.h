//
//  SVGWrap1.h
//  morphing
//
//  Created by xyz on 16/8/16.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__SVGWrap1__
#define __morphing__SVGWrap1__

#include <stdio.h>
#include <iostream>
//#include <string>
#include "Morphing.h"
#include "DrawUtil.h"
#include "BezierUtil.h"
#include "DataProc.h"
#include "stdtostring.h"
#include <opencv2/opencv.hpp>
using namespace cv;

class SVGWrap {
public:
    vector<Point> srcPoints;
    vector<Point> destPoints;
    vector<Point> srcSvgPoints;
    // 由srcSvgPoints得到的贝塞尔曲线（包含完整的贝塞尔曲线点）
    vector<Point> completedBezier;
    Morphing morph;
    // SVG归一化时的系数(平移和缩放)
    Point translate;
    double scaleSVG;
    // 归一化SVG后，贝塞尔曲线上四个极点下标
    int left, right, top, bottom;
    // 扩大目标头像人脸三角剖分边界
    double destDeltaX, destDeltaY;
    // 扩大原数据库头像人脸三角剖分边界
    double srcDeltaX, srcDeltaY;
    // 人脸尺寸
    double srcWidth, srcHeight, destWidth, destHeight;
    
private:
    void normalizeFaceData();
    void normalizeDestFace();
    void normalizeSrcFace();
    void normalizeBezier();
public:
    SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg);
    SVGWrap(int srcPoints[], int destPoints[], int svg[], int pointSize, int svgSize);
    void morphBezier();
    Mat drawSrcTriOnSvg();
    Mat drawDestTriOnSvg();

    string getWrapFacePaths();
    
};

#endif /* defined(__morphing__SVGWrap1__) */
