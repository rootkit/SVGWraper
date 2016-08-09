//
//  Delaunay.cpp
//  face
//
//  Created by xyz on 16/8/7.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Delaunay.h"
#include <set>
#include <map>

using namespace std;


Delaunay::Delaunay(vector<Point> points, vector<int> pointIndexs, vector<Point> vertexs):
points(points), pointIndexs(pointIndexs), vertexs(vertexs) {
    assert(points.size() == pointIndexs.size());
    assert(vertexs.size() == 3);
    vector<int> tmpIndexs(3, -1);
    tempTris.push_back(DelTriangle(vertexs, tmpIndexs));
}

Delaunay::Delaunay(vector<Point> points, vector<int> pointIndexs):
points(points), pointIndexs(pointIndexs) {
    
    int maxX = -100000, maxY = -100000, minX = 100000, minY = 100000;
    int left = 0, right = 0, top = 0, bottom = 0;
    for (int i = 0; i < points.size(); i++) {
        if (points[i].x > maxX) {
            maxX = points[i].x;
            right = i;
        }
        if (points[i].x < minX) {
            minX = points[i].x;
            left = i;
        }
        if (points[i].y > maxY) {
            maxY = points[i].y;
            bottom = i;
        }
        if (points[i].y < minY) {
            minY = points[i].y;
            top = i;
        }
    }

    int dx = maxX - minX, dy = maxY - minY;
    int dmax = max(dx, dy);
    int midX = minX + dx * 0.5;
    int midY = minY + dy * 0.5;

    vertexs.push_back(Point(midX - 20*dmax, midY-dmax));
    vertexs.push_back(Point(midX, midY+20*dmax));
    vertexs.push_back(Point(midX+20*dmax, midY-dmax));
    vector<int> tmpIndexs(3, -1);
    tempTris.push_back(DelTriangle(vertexs, tmpIndexs));
    
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
    
    for (int i = 0; i < points.size(); i++) {
        set<Edge> edges;
        set<Edge>::iterator edgesIt;
        for (auto it = tempTris.begin(); it != tempTris.end(); ) {
            if (it->isOutterRightOfCircle(points[i])) {
                delTris.push_back(*it);
                it = tempTris.erase(it);
            } else if (it->isOutterCircleButNotRight(points[i])) {
                cout << "isOutterCircleButNotRight" << endl;
                cout << it->points[0] << " " << it->points[1] << " " << it->points[2] << endl;
                ++it;
            } else if (it->isInTheCircle(points[i])) {
                Edge e1(it->points[0], it->points[1], it->indexs[0], it->indexs[1]),
                e2(it->points[1], it->points[2], it->indexs[1], it->indexs[2]),
                e3(it->points[2], it->points[0], it->indexs[2], it->indexs[0]);
//                edges.insert(Edge(it->points[0], it->points[1], it->indexs[0], it->indexs[1]));
//                edges.insert(Edge(it->points[1], it->points[2], it->indexs[1], it->indexs[2]));
//                edges.insert(Edge(it->points[2], it->points[0], it->indexs[2], it->indexs[0]));
                
                if ((edgesIt = edges.find(e1)) != edges.end()) {
                    edges.erase(edgesIt);
                } else {
                    edges.insert(e1);
                }
                if ((edgesIt = edges.find(e2)) != edges.end()) {
                    edges.erase(edgesIt);
                } else {
                    edges.insert(e2);
                }
                if ((edgesIt = edges.find(e3)) != edges.end()) {
                    edges.erase(edgesIt);
                } else {
                    edges.insert(e3);
                }
                
                it = tempTris.erase(it);
                
                cout << "edges size after delete===>" << edges.size() << endl;
                for (auto e : edges) {
                    cout << e.start << "(" << e.startIndex << ")" << ", " << e.end << "(" << e.endIndex << ")" << endl;
                }
            } else {
                assert(false && "this case should not happen");
            }
        }
        for (auto it = edges.begin(); it != edges.end(); it++) {
            vector<Point> tmpPoints;
            tmpPoints.push_back(it->start);
            tmpPoints.push_back(it->end);
            tmpPoints.push_back(points[i]);
            vector<int> tmpIndexs;
            tmpIndexs.push_back(it->startIndex);
            tmpIndexs.push_back(it->endIndex);
            tmpIndexs.push_back(pointIndexs[i]);
            tempTris.push_back(DelTriangle(tmpPoints, tmpIndexs));
        }
        
        cout << "===== delTri ====" << endl;
        for (auto tri : delTris) {
            cout << tri.points[0] << "  " << tri.points[1] << "  " << tri.points[2] << endl;
        }
        cout << "===== tmpTri ====" << endl;
        for (auto tri : tempTris) {
            cout << tri.points[0] << "  " << tri.points[1] << "  " << tri.points[2] << endl;
        }
        
    }
    
    cout << "delTri size====>" << delTris.size() << endl;
    
    // 将triangles与temp triangles进行合并, 除去与超级三角形有关的三角形
    for (auto tri : tempTris) {
//        if (tri.indexs[0] == -1 || tri.indexs[1] == -1 || tri.indexs[2] == -1) {
//            continue;
//        } else {
            delTris.push_back(tri);
        //}
    }
//    for (auto it = delTris.begin(); it != delTris.end();) {
//        if (it->indexs[0] == -1 || it->indexs[1] == -1 || it->indexs[2] == -1) {
//            it = delTris.erase(it);
//        } else {
//            ++it;
//        }
//    }
    
    cout << "delTris" << endl;
    for (auto tri : delTris) {
        for (auto p : tri.points) {
            cout << p << " ";
        }
        cout << endl;
    }
    
}

list<DelTriangle> Delaunay::getDelaunayTriangulation() {
    delaunayTriangulation();
    return this->delTris;
}

