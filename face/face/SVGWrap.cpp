//
//  SVGWrap.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "SVGWrap.h"

SVGWrap::SVGWrap(vector<Point> &src, vector<Point> &dest, vector<Point> &svg)
:srcPoints(src), destPoints(dest), srcSvgPoints(svg), deltaX(10), deltaY(10) {
    
    // 归一化脸部数据
    normalize_face_data();
    this->morph.init(srcPoints, destPoints);
    
    this->bezier = BezierUtil::get_bezier(srcSvgPoints);
    
    // 归一化贝塞尔曲线点
    normalize_bezier();
    wrap_bezier();
    

}

void SVGWrap::normalize_face_data() {
    assert(srcPoints.size() == 78 && destPoints.size() == 78);
    
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
    
    destPoints.push_back(Point(minX-deltaX, minY-deltaY));
    destPoints.push_back(Point(minX-deltaX, maxY));
    destPoints.push_back(Point(maxX+deltaX, maxY));
    destPoints.push_back(Point(maxX+deltaX, minY-deltaY));
    
    
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
    
    // get the width and height of the test face
    maxY = srcPoints[6].y, minY = srcPoints[77].y;
    maxX = -100000;
    minX = 100000;
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
    
    srcPoints.push_back(Point(minX-deltaX, minY-deltaY));
    srcPoints.push_back(Point(minX-deltaX, maxY));
    srcPoints.push_back(Point(maxX+deltaX, maxY));
    srcPoints.push_back(Point(maxX+deltaX, minY-deltaY));
    
    // 在已经补充完边界点的人脸上做样条插值
    //interpolate_face();
    
    // scale the testDots to the same size of targetDots
    // either on width or on height
    double scaleW = destWidth / srcWidth;
    double scaleH = destHeight / srcHeight;
    
    cout << "destWidth: " << destWidth << endl;
    cout << "destHeight: " << destHeight << endl;
    cout << "srcWidth: " << srcWidth << endl;
    cout << "srcHeight: " << srcHeight << endl;
    
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
    for (int i = 0; i < srcPoints.size(); i++) {
        srcPoints[i].x += translateV.x;
        srcPoints[i].y += translateV.y;
    }
    
}

void SVGWrap::interpolate_face() {
    vector<double> x, y;
    for (int i = 0; i < 16; i++) {
        x.push_back(srcPoints[i].x);
        y.push_back(srcPoints[i].y);
    }
    this->srcSpline.init(x, y);
    int* points = srcSpline.splineInterpolate();
    for (int i = 0; i < 16; i++) {
        srcPoints.push_back(Point(points[i*2], points[i*2+1]));
    }
    srcPoints.push_back(Point(srcPoints[78].x, srcPoints[1].y));
    srcPoints.push_back(Point(srcPoints[78].x, srcPoints[3].y));
    srcPoints.push_back(Point(srcPoints[81].x, srcPoints[11].y));
    srcPoints.push_back(Point(srcPoints[81].x, srcPoints[9].y));
    delete [] points;
    x.clear();
    y.clear();
    
    for (int i = 0; i < 16; i++) {
        x.push_back(destPoints[i].x);
        y.push_back(destPoints[i].y);
    }
    this->destSpline.init(x, y);
    points = destSpline.splineInterpolate();
    for (int i = 0; i < 16; i++) {
        destPoints.push_back(Point(points[i*2], points[i*2+1]));
    }
    destPoints.push_back(Point(destPoints[78].x, destPoints[1].y));
    destPoints.push_back(Point(destPoints[78].x, destPoints[3].y));
    destPoints.push_back(Point(destPoints[81].x, destPoints[11].y));
    destPoints.push_back(Point(destPoints[81].x, destPoints[9].y));
    delete [] points;
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
    
    //
    //svg scaleW:  0.893491
//    svg scaleH:  0.753998
    // 缩放，注意srcPoints的四个边的顶点并代表人脸的真正大小
    double bezierHeight = maxY - minY;
    double bezierWidth = maxX - minX;
    double scaleW = srcWidth / bezierWidth;
    // 原asm点包括头发上的点，可能偏高
    double scaleH = srcHeight*0.98 / (bezierHeight);
    cout << "svg scaleW:  " << scaleW << endl;
    cout << "svg scaleH:  " << scaleH << endl;
//    if (scaleW <= scaleH) {
//        scaleSVG = scaleW;
//    } else {
//        scaleSVG = scaleH;
//    }
    
    if (scaleW <= scaleH) {
        scaleSVG = scaleH;
    } else {
        scaleSVG = scaleW;
    }
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x *= scaleSVG;
        bezier[i].y *= scaleSVG;
    }
    
    // 平移，对齐下巴最低点
    //Point srcBottom((srcPoints[81].x+srcPoints[78].x)/2, (srcPoints[79].y+srcPoints[78].y)/2);
    Point srcBottom(srcPoints[6].x, srcPoints[6].y);
    this->translate.x = srcBottom.x - bezier[bottom].x;
    this->translate.y = srcBottom.y - bezier[bottom].y;
    for (int i = 0; i < bezier.size(); i++) {
        bezier[i].x += translate.x;
        bezier[i].y += translate.y;
    }
    
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
    imshow("bezier", temp);
    imwrite("bezierBeforeMorph1.jpg", temp);
    waitKey();
    
    morph.morph_bezier(bezier);
    
    temp = draw_dest_tri_on_svg();
    imshow("bezier", temp);
    imwrite("bezierAfterMorph2.jpg", temp);
    waitKey();
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
    Mat scaleMat = scale_mat_to_dots(img, srcWidth, srcHeight);
    Point translateV(srcPoints[78].x+deltaX, srcPoints[78].y+deltaY);
    for (auto t : morph.srcTris) {
        DrawUtil::draw_triangular(scaleMat, t, translateV);
    }
    return scaleMat;
}

Mat SVGWrap::draw_dest_triangular(const Mat &img) {
    Mat scaleMat = scale_mat_to_dots(img, destWidth, destHeight);
    Point translateV(destPoints[78].x+deltaX, destPoints[78].y+deltaY);
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
    for (int i = 0; i < adjustBezier.size(); i++) {
        adjustBezier[i].x -= translate.x;
        adjustBezier[i].y -= translate.y;
    }
    for (int i = 0; i < adjustBezier.size(); i++) {
        adjustBezier[i].x /= scaleSVG;
        adjustBezier[i].y /= scaleSVG;
    }
    
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


