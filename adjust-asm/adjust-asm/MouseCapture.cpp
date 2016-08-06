//
//  MouseCapture.cpp
//  adjust-asm
//
//  Created by xyz on 16/8/6.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "MouseCapture.h"

const Scalar MouseCapture::SELECTED_COLOR = Scalar(0, 255, 255);
const Scalar MouseCapture::UNSELECTED_COLOR = Scalar(0, 0, 255);

MouseCapture::MouseCapture(Mat &img, vector<Point> points): winName("MouseControl"),
marked(-1), img(img), points(points), colors(points.size(), UNSELECTED_COLOR) {
}

MouseCapture::MouseCapture(string file, vector<Point> points): winName("MouseControl"),
marked(-1), points(points){
    this->img = imread(file);
}

void MouseCapture::loadImg(cv::Mat &img) {
    this->img = img;
}

void MouseCapture::loadImg(string file) {
    img = imread(file);
}

void MouseCapture::checkInRange(int x, int y) {
    for (int i = 0; i < points.size(); i++) {
        if (x >= 0 && x < img.cols && y >= 0 && y < img.rows &&
            abs(x-points[i].x) <= 5 && abs(y-points[i].y) <= 5) {
            marked = i;
            refreshPoints(i);
        }
    }
    cout << endl;
}

void MouseCapture::refreshPoints(int t) {
    for (int i = 0; i < colors.size(); i++) {
        colors[i] = UNSELECTED_COLOR;
    }
    colors[t] = SELECTED_COLOR;
}

vector<Point> MouseCapture::getAdjustedPoints() {
    return this->points;
}

void MouseCapture::onMouse(int event, int x, int y, int flag, void *param) {
    MouseCapture *mc = (MouseCapture*)param;
    switch (event) {
        case EVENT_LBUTTONDOWN:
            if (mc->marked != -1) {
                mc->points[mc->marked].x = x;
                mc->points[mc->marked].y = y;
                mc->marked = -1;
            } else {
                mc->checkInRange(x, y);
            }
            break;
            
        default:
            break;
    }
}

void MouseCapture::adjustPoints() {
    namedWindow(winName, WINDOW_AUTOSIZE);
    setMouseCallback(winName, onMouse, this);
    Mat tempImg;
    while (true) {
        tempImg = img.clone();
        for (int i = 0; i < points.size(); i++) {
            circle(tempImg, points[i], 3, colors[i]);
        }
        string str;
        stringstream strStream;
        if (marked != -1) {
            strStream << "Point" << marked << "(" << points[marked].x << ":" <<
            points[marked].y << ")";
        }
        str = strStream.str();
        putText(tempImg, str, Point(10,40), CV_FONT_HERSHEY_COMPLEX, .8,Scalar(0,255,0),2);
        
        imshow(winName, tempImg);
        if(waitKey(10) == 'q')
            break;
    }
}