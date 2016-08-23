//
//  HairDetection.h
//  grabcut
//
//  Created by xyz on 16/8/18.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __grabcut__HairDetection__
#define __grabcut__HairDetection__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <stdio.h>
#include <cassert>
#include <iostream>
using namespace std;

class HairDetection {
public:
    Mat srcImg;
    Mat inputImg;
    Mat hairImg;
    Mat firstMask;
    int width;
    int height;
    float gaussianMean;
    float gaussianStandardDeviation;
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
    Mat gaussianBlurFor(Mat img);
    void calGaussianMeanAndStandardDeviation(Mat &gaussianImg);
    float getStandardDeviation(float* array, float mean, int size);
    void calHairColorMeanAndStandardDeviation();
    void calColorMask();
    void calFusionMask();
    void mattingProcess();
public:
    HairDetection(Mat &inputImg, Rect &faceRect);
    ~HairDetection();
    
    
};

#endif /* defined(__grabcut__HairDetection__) */
