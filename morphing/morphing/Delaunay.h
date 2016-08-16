//
//  Delaunay.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__Delaunay__
#define __morphing__Delaunay__

#include <stdio.h>
#include <list>
#include <vector>
#include <algorithm>
using namespace std;

#include "DelTriangle.h"

class Delaunay {
private:
    // 三角剖分控制点
    vector<Point> points;
    vector<int> pointIndexs;
    // 超级三角形顶点
    vector<Point> vertexs;
    
    list<DelTriangle> tempTris;
    list<DelTriangle> delTris;
private:
    bool isLarger(Point &a, Point &b);
    void delaunayTriangulation();
    // 对输入的控制点按x坐标排序
    void heapSort(vector<Point> &v, vector<int> &indexs);
    void buildHeap(vector<Point> &v, vector<int> &indexs);
    void insertHeap(vector<Point> &v, vector<int> &indexs, Point &current,
                    int curIndex, int low, int high);
public:
    // points是控制点，超级三角形的顶点自己计算
    Delaunay(vector<Point> points, vector<int> pointIndexs);
    list<DelTriangle> getDelaunayTriangulation();
};

#endif /* defined(__morphing__Delaunay__) */
