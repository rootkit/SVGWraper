//
//  testPointSet.cpp
//  face
//
//  Created by xyz on 16/8/9.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <stdio.h>

#include <iostream>
#include <set>
#include <map>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "DelTriangle.h"

//class PointComp {
//public:
//    bool operator() (const Point &a, const Point &b) const {
//        return a.x < b.x || (a.x == b.x && a.y < b.y);
//    }
//};

int main() {
//    set<Point, PointComp> points;
//    points.insert(Point(1,1));
//    points.insert(Point(2,1));
//    points.insert(Point(3,1));
//    points.insert(Point(1,1));
//    
//    for (auto p : points) {
//        cout << p << endl;
//    }
    
    
    set<Edge> edges;
    Edge e1(Point(1,1), Point(1,1), 0, 1),
    e2(Point(1,1), Point(2,2), 1, 2),
    e3(Point(3,3), Point(2,2), 3, 2),
    e4(Point(3,3), Point(2,2), 3, 2);
    edges.insert(e1);
    edges.insert(e2);
    edges.insert(e3);
    edges.insert(e4);
    for (auto e : edges) {
        cout << e.start << "," << e.end << endl;
    }
    set<Edge>::iterator it;
    Edge tmp(Point(3,3), Point(2,2), 3, 2);
    if ((it = edges.find(tmp)) != edges.end()) {
        edges.erase(it);
    }
    Edge e5(Point(4,4), Point(3,3), 4, 3);
    if ((it = edges.find(e5)) == edges.end()) {
        edges.insert(e5);
    }
    
    cout << endl;
    for (auto e : edges) {
        cout << e.start << "," << e.end << endl;
    }
    
//    map<Point, int, PointComp> points;
//    points.insert(make_pair(Point(1,1), 1));
//    points.insert(make_pair(Point(2,1), 3));
//    points.insert(make_pair(Point(3,1), 2));
//    points.insert(make_pair(Point(1,1), 4));
//    
//    for (auto p : points) {
//        cout << p.first << " " << p.second << endl;
//    }
}
