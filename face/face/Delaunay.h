//
//  Delaunay.h
//  face
//
//  Created by xyz on 16/8/7.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face__Delaunay__
#define __face__Delaunay__

#include <stdio.h>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "DelTriangle.h"

class Delaunay {
private:
    // 三角剖分控制点，包括边界控制点，但不包括四个顶点
    vector<Point> points;
    vector<int> pointIndexs;
    // 三角剖分边界四个顶点
    vector<Point> vertexs;
    vector<int> vertexIndexs;
    list<DelTriangle> tempTris;
    vector<DelTriangle> delTris;
private:
    bool isLarger(Point &a, Point &b);
    void delaunayTriangulation();
    void heapSort(vector<Point> &v, vector<int> &indexs);
    void buildHeap(vector<Point> &v, vector<int> &indexs);
    void insertHeap(vector<Point> &v, vector<int> &indexs, Point &current,
                    int curIndex, int low, int high);
public:
    Delaunay(vector<Point> points, vector<Point> vertexs);
    vector<DelTriangle> getDelaunayTriangulation();
};

#endif /* defined(__face__Delaunay__) */
