//
//  MouseCapture.cpp
//  grabcut
//
//  Created by xyz on 16/8/17.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "MouseCapture.h"


MouseCapture::MouseCapture(Mat &img):
img(img), marked(false), finished(false), winName("grabcut"){
    
}

MouseCapture::~MouseCapture() {
    this->img.release();
}

bool MouseCapture::check(int x, int y) {
    return !(p1.x == x || p1.y == y);
}

void MouseCapture::onMouse(int event, int x, int y, int flag, void *param) {
    MouseCapture *mc = (MouseCapture*)param;
    switch (event) {
        case EVENT_LBUTTONDOWN:
            if (mc->marked == true) {
                if (mc->check(x, y)) {
                    mc->p2 = Point(x, y);
                    mc->marked = false;
                    mc->finished = true;
                    //mc->canMove = false;
                }
            } else {
                mc->p1 = Point(x, y);
                mc->marked = true;
                mc->finished = false;
                //mc->canMove = true;
            }
            break;
            
        default:
            break;
    }
}

//void MouseCapture::onMouse(int event, int x, int y, int flag, void *param) {
//    MouseCapture *mc = (MouseCapture*)param;
//    switch (event) {
//        case EVENT_LBUTTONDOWN:
//            if (mc->marked == false) {
//                mc->p1 = Point(x, y);
//                mc->marked = true;
//                //mc->canMove = true;
//            }
//            break;
//        case EVENT_MOUSEMOVE:
//            if (flag == EVENT_FLAG_LBUTTON) {
//                if (mc->marked) {
//                    mc->moveX = x;
//                    mc->moveY = y;
//                }
//            }
//        case EVENT_LBUTTONUP:
//            if (mc->marked == true) {
//                mc->p2 = Point(x, y);
//                mc->marked = false;
//            }
//            break;
//            
//            
//        default:
//            break;
//    }
//}


void MouseCapture::drawRect(Mat &tempImg, int x1, int y1, int x2, int y2) {
    Point lt(min(x1, x2), min(y1, y2));
    Point rb(max(x1, x2), max(y1, y2));
    line(tempImg, lt, Point(rb.x, lt.y), Scalar(0, 255, 255));
    line(tempImg, Point(rb.x, lt.y), rb, Scalar(0, 255, 255));
    line(tempImg, rb, Point(lt.x, rb.y), Scalar(0, 255, 255));
    line(tempImg, Point(lt.x, rb.y), lt, Scalar(0, 255, 255));
}


void MouseCapture::refreshImage(Mat &tempImg) {
    if (marked == true) {
        circle(tempImg, p1, 2, Scalar(255, 255, 255));
    }
    if (finished == true) {
        int minX = min(p1.x, p2.x), maxX = max(p1.x, p2.x),
        minY = min(p1.y, p2.y), maxY = max(p1.y, p2.y);
        drawRect(tempImg, minX, minY, maxX, maxY);
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
        if (waitKey(10) == 'q') {
            break;
        }
    }
}

Rect MouseCapture::getRect() {
    int width = abs(p2.x-p1.x), height = abs(p2.y-p1.y);
    Point lt(min(p1.x, p2.x), min(p1.y, p2.y));
    return Rect(lt.x, lt.y, width, height);
}





