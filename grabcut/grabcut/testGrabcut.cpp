//
//  testGrabcut.cpp
//  grabcut
//
//  Created by xyz on 16/8/23.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "MouseCapture.h"
#include "FileUtil.h"
#include "HairDetection.h"
#include "DrawUtl.h"
#include "Marker.h"

#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

int main() {
    int testNum = 1;
    vector<vector<Point> > facePoints = FileUtil::read_all_asm_points("all-asmpoints.txt");
    
    Mat image = imread((to_string(testNum) + ".png").c_str());
    
    vector<Point> facePoint = facePoints[testNum];
    Rect faceRect(facePoint[0].x, facePoint[14].y, facePoint[12].x-facePoint[0].x, facePoint[6].y-facePoint[14].y);
    
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
    
    
    Rect headRect;
    headRect.x = faceRect.x - 1.0 / 3.0 * faceRect.width;
    headRect.y = faceRect.y - 2.0 / 5.0 * faceRect.height;
    headRect.width = 5.0 / 3 * faceRect.width;
    headRect.height = 9.0/5 * faceRect.height;
    
    Mat bgModel, fgModel, firstMask;
    imshow("test", image);
    waitKey();
    grabCut(image, firstMask, headRect, bgModel, fgModel, 2, GC_INIT_WITH_RECT);
    compare(firstMask, GC_PR_FGD, firstMask, CMP_EQ);
    Mat testImg;
    image.copyTo(testImg, firstMask);
    imshow("test", testImg);
    waitKey();
    
    
    firstMask = GC_PR_BGD;
    Marker marker(firstMask.clone(), testImg.clone());
    marker.adjustPoints();
    
    Mat tempMarker = marker.mask.clone();
    for (int i = 0; i < tempMarker.rows; i++) {
        for (int j = 0; j < tempMarker.cols; j++) {
            if (tempMarker.at<uchar>(i, j) == GC_FGD) {
                tempMarker.at<uchar>(i, j) = 255;
            } else if (tempMarker.at<uchar>(i, j) == GC_BGD) {
                tempMarker.at<uchar>(i, j) = 100;
            }
        }
    }
        imwrite("markermaskbefore.png", tempMarker);
    
    Mat testImg1;
    Mat bgModel1, fgModel1;
    imshow("testImg", testImg);
    waitKey();
    grabCut(testImg, marker.mask, headRect, bgModel1, fgModel1, 3, GC_INIT_WITH_MASK);
    compare(marker.mask, GC_FGD|GC_PR_FGD, marker.mask, CMP_EQ);
    imwrite("markermask.png", marker.mask);
    testImg.copyTo(testImg1, marker.mask);
    imshow("test", testImg1);
    waitKey();
    imwrite("ret.png", testImg1);
    
    
    
}