//
//  Delaunay.cpp
//  face
//
//  Created by xyz on 16/8/7.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Delaunay.h"


Delaunay::Delaunay(vector<Point> points, vector<Point> vertexs):
points(points), vertexs(vertexs) {
    assert(vertexs.size() == 4);
}

bool Delaunay::isLarger(Point &a, Point &b) {
    return a.x > b.x;
}

void Delaunay::heapSort(vector<Point> &v, vector<int> &indexs) {
    buildHeap(v, indexs);
    for (int lastUnsorted = v.size()-1; lastUnsorted >= 0; lastUnsorted--) {
        Point current = v[lastUnsorted];
        v[lastUnsorted] = v[0];
        int curIndex = indexs[lastUnsorted];
        indexs[lastUnsorted] = indexs[0];
        insertHeap(v, indexs, current, curIndex, 0, lastUnsorted-1);
    }
}

void Delaunay::buildHeap(vector<Point> &v, vector<int> &indexs) {
    for (int low = (v.size()-1)/2; low >= 0; low--) {
        Point current = v[low];
        int curIndex = indexs[low];
        insertHeap(v, indexs, current, curIndex, low, v.size()-1);
    }
}

void Delaunay::insertHeap(vector<Point> &v, vector<int> &indexs, Point &current,
        int curIndex, int low, int high) {
    int large = 2*low+1;
    while (large <= high) {
        if (large < high && isLarger(v[large+1], v[large])) {
            large++;
        }
        if (isLarger(v[large], current)) {
            indexs[low] = indexs[large];
            v[low] = v[large];
            low = large;
            large = 2 * low + 1;
        } else {
            break;
        }
    }
    indexs[low] = curIndex;
    v[low] = current;
}


void Delaunay::delaunayTriangulation() {
    heapSort(points, pointIndexs);
    
}

vector<DelTriangle> Delaunay::getDelaunayTriangulation() {
    return this->delTris;
}

