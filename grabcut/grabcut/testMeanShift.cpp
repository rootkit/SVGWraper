//
//  testMeanShift.cpp
//  grabcut
//
//  Created by xyz on 16/8/24.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "MouseCapture.h"
#include "FileUtil.h"
#include "HairDetection.h"
#include "DrawUtl.h"

#include <stdio.h>

#include <opencv2/opencv.hpp>
using namespace std;
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
    grabCut(image, firstMask, headRect, bgModel, fgModel, 2, GC_INIT_WITH_RECT);
    compare(firstMask, GC_PR_FGD, firstMask, CMP_EQ);
    Mat testImg;
    image.copyTo(testImg, firstMask);
    imshow("test", testImg);
    waitKey();
    
    
    
    int spatialRad = 10, colorRad = 34, maxPryLevel = 1;
    ////调用meanshift图像金字塔进行分割
    Mat dst;
    pyrMeanShiftFiltering(image, dst, spatialRad, colorRad, maxPryLevel);
    
    RNG rng = theRNG();
    
    Mat mask(dst.rows + 2, dst.cols + 2, CV_8UC1, Scalar::all(0));
    
    for (int i = 0; i<dst.rows; i++)    //opencv图像等矩阵也是基于0索引
        
        for (int j = 0; j<dst.cols; j++)
            
            if (mask.at<uchar>(i + 1, j + 1) == 0)
                
            {
                
                Scalar newcolor(rng(256), rng(256), rng(256));
                
                floodFill(dst, mask, Point(j, i), newcolor, 0, Scalar::all(1), Scalar::all(1));
                
            }
    
    imshow("dst", dst);
    
    imwrite("dst.png", dst);
    
    
    
    
    
    
}