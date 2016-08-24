//
//  main.cpp
//  grabcut
//
//  Created by xyz on 16/8/17.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "MouseCapture.h"
#include "FileUtil.h"
#include "HairDetection1.h"
#include "DrawUtl.h"

#include <string>
#include <iostream>
using namespace std;

#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, const char * argv[]) {
    
//    Mat image = imread("1.png");
//    MouseCapture mc(image);
//    mc.adjustPoints();
//    Mat temp(image.rows, image.cols, image.type());
//    image(mc.getRect()).copyTo(temp);
//    imwrite("temp.jpg", temp);
//    
//    Mat result;
//    Mat bgModel, fgModel;
//    grabCut(image, result, mc.getRect(), bgModel, fgModel, 1, GC_INIT_WITH_RECT);
//    compare(result, GC_PR_FGD, result, CMP_EQ);
//    
//    Mat foreground(image.size(), CV_8UC3, Scalar(255,255,255));
//    image.copyTo(foreground, result);
//    
//    imwrite("foreground.jpg", foreground);
//    
//    foreground(mc.getRect()).copyTo(temp);
//    cvtColor(temp, temp, CV_BGR2GRAY);
//    //threshold(temp, temp, 0, 255, CV_THRESH_BINARY | CV_THRESH_MASK);
//    imwrite("result.png", temp);
    
    
    int testNum = 61;
    vector<vector<Point> > facePoints = FileUtil::read_all_asm_points("all-asmpoints.txt");
    
    Mat image = imread((to_string(testNum) + ".png").c_str());
    
    vector<Point> facePoint = facePoints[testNum];
    Rect faceRect(facePoint[0].x, max(facePoint[14].y, 0), facePoint[12].x-facePoint[0].x, facePoint[6].y-facePoint[14].y);
    
    double STANDARD_LENGTH = 400.0;
    // 缩放图像
    if (image.rows > STANDARD_LENGTH || image.cols > STANDARD_LENGTH) {
        double scale;
        if (image.rows >= image.cols) {
            scale = STANDARD_LENGTH / image.rows;
        } else {
            scale = STANDARD_LENGTH / image.cols;
        }
        resize(image, image, Size(image.cols*scale, image.rows*scale));
        
        faceRect.x *= scale;
        faceRect.y *= scale;
        faceRect.width *= scale;
        faceRect.height *= scale;
    }
    
    HairDetection h(image, faceRect, "hair"+to_string(testNum), testNum);
    //imwrite("hair.png", h.hairImg);
    
    return 0;
}
