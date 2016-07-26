//
//  DataProc.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "DataProc.h"


void DataProc::normalize_face_data(vector<Point> &testPoints, vector<Point> &targetPoints) {
    Mat face(1500, 1500, CV_8UC3);
    
    // get the width and height of the target face
    int maxX = -100000, minX = 100000,
    maxY = targetPoints[6].y, minY = targetPoints[77].y;
    for (int i = 0; i < 13; i++) {
        if (minX > targetPoints[i].x) {
            minX = targetPoints[i].x;
        }
        if (maxX < targetPoints[i].x) {
            maxX = targetPoints[i].x;
        }
    }
    double targetW = maxX - minX;
    double targetH = maxY - minY;
    
    targetPoints.push_back(Point(minX, minY));
    targetPoints.push_back(Point(minX, maxY));
    targetPoints.push_back(Point(maxX, maxY));
    targetPoints.push_back(Point(maxX, minY));
    
    //draw_face(face, targetDots, 1, 1);
    
    // get the width and height of the test face
    maxY = testPoints[6].y, minY = testPoints[77].y;
    maxX = -100000;
    minX = 100000;
    for (int i = 0; i < 13; i++) {
        if (minX > testPoints[i].x) {
            minX = testPoints[i].x;
        }
        if (maxX < testPoints[i].x) {
            maxX = testPoints[i].x;
        }
    }
    
    double testW = maxX - minX;
    double testH = maxY - minY;
    
    testPoints.push_back(Point(minX, minY));
    testPoints.push_back(Point(minX, maxY));
    testPoints.push_back(Point(maxX, maxY));
    testPoints.push_back(Point(maxX, minY));
    
    //    draw_face(face, testDots, 1, 2);
    
    // scale the testDots to the same size of targetDots
    // either on width or on height
        double scaleW = targetW / testW;
        double scaleH = targetH / testH;
    
//        cout << "targetW===>" << targetW << "  targetH===>" << targetH << endl;
//        cout << "testW===>" << testW << "  testH===>" << testH << endl;
    
        double minScale = scaleH <= scaleW ? scaleH : scaleW;
    
        for (int i = 0; i < testPoints.size(); i++) {
            testPoints[i].x *= minScale;
            testPoints[i].y *= minScale;
        }
    
    // align the center of testDots to center of targetDots
    Point targetCenter((targetPoints[78].x+targetPoints[81].x)/2,
                     (targetPoints[78].y+targetPoints[79].y)/2);
    Point testCenter((testPoints[78].x+testPoints[81].x)/2,
                   (testPoints[78].y+testPoints[79].y)/2);
    Point translateV(targetCenter.x-testCenter.x, targetCenter.y-testCenter.y);
    for (int i = 0; i < testPoints.size(); i++) {
        testPoints[i].x += translateV.x;
        testPoints[i].y += translateV.y;
    }
    
//    draw_face(face, testDots, 1, 2);
//    draw_face(face, targetPoints, 1, 0);
//    imwrite("testDots.jpg", face);
}
