//
//  SVGWrap.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__SVGWrap__
#define __morphing__SVGWrap__

#include <iostream>
#include "Morphing.h"
#include "DrawUtil.h"
#include "BezierUtil.h"
#include "DataProc.h"

#include <opencv2/opencv.hpp>
using namespace cv;

class SVGWrap {
public:
    vector<Point> srcPoints;
    vector<Point> destPoints;
    vector<Point> srcSvgPoints;
    // 由srcSvgPoints得到的贝塞尔曲线（只包含三等分点）
    vector<Point> bezier;
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
    void morphBezier();
    Mat scaleMat(Mat mat, double width, double height);
    Mat drawSrcTriOnSvg();
    Mat drawDestTriOnSvg();
    vector<Point> regainSvgCtrlPoints();
    
    //返回形变后的最外层脸部的相对路径点
    vector<Point> getRelativePathPoints(vector<Point> svgCtrlPoints);
    string getRelativePath(vector<Point> points);
    
    Mat morphingImg(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints);
    
    // 归一化后的svg points，测试用
    vector<Point> tempSvg;
};

#endif /* defined(__morphing__SVGWrap__) */
