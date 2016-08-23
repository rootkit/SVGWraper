//
//  Marker.cpp
//  grabcut
//
//  Created by xyz on 16/8/23.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Marker.h"

Marker::Marker(Mat mask, Mat img) {
    this->mask = mask;
    this->img = img.clone();
}

void Marker::onMouse(int event, int x, int y, int flag, void *param) {
    Marker *m = (Marker*)param;
    switch (event) {
        case EVENT_LBUTTONDOWN:
            cout << "left button: " << x << "," << y << endl;
            circle(m->img, Point(x, y), 3, Scalar(0,0,255), -1);
            circle(m->mask, Point(x,y), 3, GC_FGD, -1);
            //m->mask.at<uchar>(y, x) = GC_FGD;
            break;
        case EVENT_RBUTTONDOWN:
            cout << "right button: " << x << "," << y << endl;
            circle(m->img, Point(x,y), 3, Scalar(255, 0, 0), -1);
            circle(m->mask, Point(x,y), 3, GC_BGD, -1);
            //m->mask.at<uchar>(y, x) = GC_BGD;
            break;
        default:
            break;
    }
}

void Marker::adjustPoints() {
    namedWindow("marker", WINDOW_AUTOSIZE);
    setMouseCallback("marker", onMouse, this);
    //Mat tempImg;
    while (true) {
        imshow("marker", img);
        if (waitKey(10) == 'q') {
            break;
        }
    }
}

