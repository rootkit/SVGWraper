//
//  main.cpp
//  grabcut
//
//  Created by xyz on 16/8/17.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <iostream>
#include "MouseCapture.h"
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main(int argc, const char * argv[]) {
    
    Mat image = imread("1.png");
    MouseCapture mc(image);
    mc.adjustPoints();
    Mat temp(image.rows, image.cols, image.type());
    image(mc.getRect()).copyTo(temp);
    imwrite("temp.jpg", temp);
    
    Mat result;
    Mat bgModel, fgModel;
    grabCut(image, result, mc.getRect(), bgModel, fgModel, 1, GC_INIT_WITH_RECT);
    compare(result, GC_PR_FGD, result, CMP_EQ);
    
    Mat foreground(image.size(), CV_8UC3, Scalar(255,255,255));
    image.copyTo(foreground, result);
    
    imwrite("foreground.jpg", foreground);
    
    foreground(mc.getRect()).copyTo(temp);
    cvtColor(temp, temp, CV_BGR2GRAY);
    //threshold(temp, temp, 0, 255, CV_THRESH_BINARY | CV_THRESH_MASK);
    imwrite("result.png", temp);
    
    
    
    return 0;
}
