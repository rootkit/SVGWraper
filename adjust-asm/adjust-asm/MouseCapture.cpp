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

MouseCapture::MouseCapture(Mat &img, vector<Point> points, int facePointNum): winName("MouseControl"),
marked(-1), img(img), colors(facePointNum, UNSELECTED_COLOR), splineError(false) {
    
    this->pointNum = facePointNum;
    
    for (int i = 0; i < facePointNum; i++) {
        this->points.push_back(points[i]);
    }
    
    int facePoints[facePointNum*2];
    for (int i = 0; i < facePointNum; i++) {
        facePoints[i*2] = points[i].x;
        facePoints[i*2+1] = points[i].y;
    }
    this->faceSpline = new FaceSpline(facePoints, facePointNum*2);
}

MouseCapture::MouseCapture(string file, vector<Point> points, int facePointNum): winName("MouseControl"),
marked(-1), colors(facePointNum, UNSELECTED_COLOR), splineError(false) {
    
    this->pointNum = facePointNum;

    this->img = imread(file);
    
    for (int i = 0; i < facePointNum; i++) {
        this->points[i] = points[i];
    }
    
    int facePoints[facePointNum*2];
    for (int i = 0; i < facePointNum; i++) {
        facePoints[i*2] = points[i].x;
        facePoints[i*2+1] = points[i].y;
    }
    this->faceSpline = new FaceSpline(facePoints, facePointNum*2);
}

MouseCapture::~MouseCapture() {
    this->img.release();
    delete faceSpline;
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
            abs(x-points[i].x) <= 10 && abs(y-points[i].y) <= 10) {
            marked = i;
            refreshPoints(i);
            
            //this->faceSpline->adjustPoint(marked, x, y);
        }
    }
}

void MouseCapture::refreshPoints(int t) {
    for (int i = 0; i < colors.size(); i++) {
        colors[i] = UNSELECTED_COLOR;
    }
    if (0 <= t && t < colors.size()) {
        colors[t] = SELECTED_COLOR;
    }
}


vector<Point> MouseCapture::getAdjustedPoints() {
    vector<Point> face;
    for (int i = 0; i < pointNum; i++) {
        face.push_back(Point(faceSpline->facePoints[i*2], faceSpline->facePoints[i*2+1]));
    }
    return face;
}

void MouseCapture::onMouse(int event, int x, int y, int flag, void *param) {
    MouseCapture *mc = (MouseCapture*)param;
    switch (event) {
        case EVENT_LBUTTONDOWN:
            if (mc->marked != -1) {
                if (mc->faceSpline->checkPointInRange(mc->marked, x, y)) {
                    mc->splineError = false;
                    mc->points[mc->marked].x = x;
                    mc->points[mc->marked].y = y;
                    mc->faceSpline->adjustPoint(mc->marked, x, y);
                } else {
                    mc->refreshPoints(-1);
                    mc->splineError = true;
                }
                mc->marked = -1;
            } else {
                mc->splineError = false;
                mc->checkInRange(x, y);
            }
            break;
            
        default:
            break;
    }
}


void MouseCapture::refreshImage(Mat &tempImg) {
    for (int i = 0; i < points.size(); i++) {
        circle(tempImg, points[i], 5, colors[i]);
    }
    for (int i = 0; i < faceSpline->SPLINE_SECTIONS; i++) {
        for (int j = 0; j < faceSpline->splinePoints[i].size(); j+=2) {
            circle(tempImg, Point(faceSpline->splinePoints[i][j], faceSpline->splinePoints[i][j+1]), 1, Scalar::all(255));
        }
//        for (int j = 0; j < 100; j+=2) {
//            circle(tempImg, Point(faceSpline->splinePoints[i][j], faceSpline->splinePoints[i][j+1]), 1, Scalar::all(255));
//        }
    }
    if (!splineError) {
        string str;
        stringstream strStream;
        if (marked != -1) {
            strStream << "Point" << marked << "(" << points[marked].x << "," <<
            points[marked].y << ")";
        }
        str = strStream.str();
        putText(tempImg, str, Point(10,40), CV_FONT_HERSHEY_COMPLEX, .8,Scalar(0,255,0),2);
    } else {
        putText(tempImg, "spline point out of range", Point(10,40), CV_FONT_HERSHEY_COMPLEX, .8,Scalar(0,255,0),2);
    }
}


void MouseCapture::adjustPoints() {
    namedWindow(winName, WINDOW_AUTOSIZE);
    setMouseCallback(winName, onMouse, this);
    Mat tempImg;
    while (true) {
        tempImg = img.clone();
        
        refreshImage(tempImg);
        
        imshow(winName, tempImg);
        if(waitKey(10) == 'q')
            break;
    }
    imwrite("result.jpg", tempImg);
}