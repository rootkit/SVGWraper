//
//  HairDetection1.h
//  grabcut
//
//  Created by xyz on 16/8/24.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __grabcut__HairDetection1__
#define __grabcut__HairDetection1__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <stdio.h>
#include <cassert>
#include <iostream>
#include <cmath>
using namespace std;

class HairDetection {
public:
    Mat srcImg;
    Mat inputImg;
    Mat hairImg;
    Mat firstMask;
    int width;
    int height;
    // 头发滤波平均值和标准差，取代之前的高斯模糊
    float hairGrayMean;
    float hairGrayStandardDeviation;
    //    float gaussianMean;
    //    float gaussianStandardDeviation;
    float hairColorYMean;
    float hairColorYStandardDeviation;
    float hairColorBMean;
    float hairColorBStandardDeviation;
    float hairColorRMean;
    float hairColorRStandardDeviation;
    Rect faceRect;
    Rect headRect;
    Rect topRect;
    float *grayImg;
    unsigned char* frequentialMask;
    unsigned char* colorMask;
    unsigned char* fusionMask;
private:
    bool isInRect(Rect rect, int x, int y);
    bool isInFrequentialMask(int index);
    bool isInColorMask(int index);
    void calFrequentialMask();
    Mat gaussianBlurFor(Mat &img);
    void getGrayImg(Mat &img);
    void calGaussianMeanAndStandardDeviation(Mat &gaussianImg);
    float getStandardDeviation(float* array, float mean, int size);
    void calHairColorMeanAndStandardDeviation();
    void calColorMask();
    void calFusionMask();
    void mattingProcess();
public:
    HairDetection(Mat &inputImg, Rect &faceRect, string dir, int testNum);
    ~HairDetection();
    
    // for test
public:
    string dir;
    int testNum;
    void saveMat(Mat &mat, string name);
    
    
};
#endif /* defined(__grabcut__HairDetection1__) */
