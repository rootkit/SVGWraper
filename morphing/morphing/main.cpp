//
//  main.cpp
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "FileUtil.h"
#include "DataProc.h"
#include "DrawUtil.h"
#include "Morphing.h"
#include "SVGWrap1.h"

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

void testMorphing();
void testBezierMorph();

int main(int argc, const char * argv[]) {
    //testMorphing();
    testBezierMorph();
    return 0;
}

void testBezierMorph() {
    vector<vector<Point> > asmPoints = FileUtil::readAsmPoints("dataBase.txt");
    vector<Point> srcFace = asmPoints[1];
    vector<Point> testAsmFacePoints = FileUtil::readTestAsmFaceData("data.txt");
    // 更新脸部16对点
    for (int i = 1; i < 16; i++) {
        srcFace[i] = testAsmFacePoints[i];
    }
    
    vector<vector<Point> > svgs = FileUtil::readSvgPoints("allFaceSvgPoint.txt");
    vector<Point> srcSvg = svgs[1];
    
    int top, bottom, right, left;
    vector<Point> srcBezier = BezierUtil::getBezier2(srcSvg);
    BezierUtil::getBezierBorder(srcBezier, left, right, top, bottom);
    Mat srcBezierImg(srcBezier[bottom].y+100, srcBezier[right].x+100, CV_8UC1, Scalar::all(0));
    DrawUtil::drawPoints(srcBezierImg, srcBezier, 1);
    imwrite("srcBezier.jpg", srcBezierImg);
    
    for (int i = 0; i < 1; i++) {
        
        if (i == 29) {
            continue;
        }
        
        vector<Point> destFace = asmPoints[i];
        
        int srcPoints[srcFace.size()*2];
        int destPoints[asmPoints[i].size()*2];
        int svgPoints[srcSvg.size()*2];
        for (int i = 0; i < srcFace.size(); i++) {
            srcPoints[i*2] = srcFace[i].x;
            srcPoints[i*2+1] = srcFace[i].y;
            destPoints[i*2] = destFace[i].x;
            destPoints[i*2+1] = destFace[i].y;
        }
        for (int i = 0; i < srcSvg.size(); i++) {
            svgPoints[i*2] = srcSvg[i].x;
            svgPoints[i*2+1] = srcSvg[i].y;
        }
    
        //SVGWrap svgWrap(srcFace, asmPoints[i], srcSvg);
        SVGWrap svgWrap(srcPoints, destPoints, svgPoints, srcFace.size()*2, srcSvg.size()*2);
        
        int top, bottom, right, left;
        BezierUtil::getBezierBorder(svgWrap.srcSvgPoints, left, right, top, bottom);
        Mat image(svgWrap.srcSvgPoints[bottom].y+100, svgWrap.srcSvgPoints[right].x+100,
                  CV_8UC1, Scalar::all(0));
        DrawUtil::drawPoints(image, svgWrap.srcSvgPoints, 1);
        imwrite(("图片1测试结果/wrap1-"+to_string(i)+".jpg"), image);
        
        string name = "wrap1-" + to_string(i) + ".txt";
        //ofstream fout("原图1测试svg控制点形变/" + name);
        cout << svgWrap.getWrapFacePaths() << endl;
        //fout.close();
    }
    

}

void testMorphing() {
    vector<vector<Point> > asmPoints = FileUtil::readAsmPoints("dataBase(old).txt");
    vector<Point> srcFace = asmPoints[1];
    vector<Point> targetFace = asmPoints[0];
    // 将targetFace对齐到srcFace，方便srcFace与图片人脸对齐
    DataProc::normalizeFaceData(targetFace, srcFace);
    Mat face = imread("1.png");
    
    cout << "type: " << face.type() << endl;
    
//    DrawUtil::drawPoints(face, targetFace, 2);
//    imwrite("test.png", face);
    
    Morphing morphing(srcFace, targetFace);
    Mat morphMat = morphing.morphImg(face, srcFace, targetFace);
    imwrite("morphing.png", morphMat);
    
}


