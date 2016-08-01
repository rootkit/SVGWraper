//
//  SVGWrap.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__SVGWrap__
#define __face1__SVGWrap__

#include <iostream>
#include "Morphing.h"
#include "DrawUtil.h"
#include "BezierUtil.h"
#include "MySpline.h"

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
    // 扩大原人脸三角剖分边界
    double deltaX, deltaY;
    // 人脸尺寸
    double srcWidth, srcHeight, destWidth, destHeight;
    // 人脸插值类
    MySpline srcSpline, destSpline;
    
private:
    void interpolate_face();
    void normalize_face_data();
    void normalize_bezier();
public:
    SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg);
    void wrap_bezier();
    Mat draw_src_triangular(const Mat &img);
    Mat draw_dest_triangular(const Mat &img);
    Mat scale_mat_to_dots(Mat mat, double width, double height);
    Mat draw_src_tri_on_svg();
    Mat draw_dest_tri_on_svg();
    vector<Point> regain_svg_ctrl_points();
    
    //返回形变后的最外层脸部的相对路径点
    vector<Point> get_relative_path_points(vector<Point> svgCtrlPoints);
    string get_relative_path(vector<Point> points);
    
    Mat morphing_img(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints);

    // 归一化后的svg points，测试用
    vector<Point> tempSvg;
};

#endif /* defined(__face1__SVGWrap__) */
