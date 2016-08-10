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

int c = 0;
int cc = 0;

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
    for (int i = 0; i < points.size(); i++) {
        if (points[i].x > maxX) {
            maxX = points[i].x;
        }
        if (points[i].x < minX) {
            minX = points[i].x;
        }
        if (points[i].y > maxY) {
            maxY = points[i].y;
        }
        if (points[i].y < minY) {
            minY = points[i].y;
        }
    }

    int dx = maxX - minX, dy = maxY - minY;
    int dmax = max(dx, dy);
    int midX = minX + dx * 0.5;
    int midY = minY + dy * 0.5;

    vertexs.push_back(Point(midX - 5*dmax, midY-dmax));
    vertexs.push_back(Point(midX, midY+5*dmax));
    vertexs.push_back(Point(midX+5*dmax, midY-dmax));
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
        
//        Mat delImage(2000, 2000, CV_8UC3, Scalar::all(0));
//        Mat tempImage = Mat(2000, 2000, CV_8UC3, Scalar::all(0));
//        
//        for (auto tri : delTris) {
//            if (tri.indexs[0] != -1 && tri.indexs[1] != -1 && tri.indexs[2] != -1) {
//                circle(delImage, tri.center, tri.radius, Scalar(0, 255, 255));
//            }
//        }
//        for (auto tri : tempTris) {
//            if (tri.indexs[0] != -1 && tri.indexs[1] != -1 && tri.indexs[2] != -1) {
//                circle(tempImage, tri.center, tri.radius, Scalar(0, 255, 255));
//            }
//        }
        
        set<Edge> edges;
        set<Edge>::iterator edgesIt;
        for (auto it = tempTris.begin(); it != tempTris.end(); ) {
            if (it->isOutterRightOfCircle(points[i])) {
                delTris.push_back(*it);
                it = tempTris.erase(it);
            } else if (it->isOutterCircleButNotRight(points[i])) {
                ++it;
            } else if (it->isInTheCircle(points[i])) {
                Edge e1(it->points[0], it->points[1], it->indexs[0], it->indexs[1]),
                e2(it->points[1], it->points[2], it->indexs[1], it->indexs[2]),
                e3(it->points[2], it->points[0], it->indexs[2], it->indexs[0]);
                
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
                
//                for (auto e : edges) {
//                    cout << e.start << " -- " << e.end << endl;
//                }
//                cout << endl;
                
                it = tempTris.erase(it);
                
            } else {
                assert(false && "this case should not happen");
            }
        }
        for (auto it = edges.begin(); it != edges.end(); it++) {
            if (DelTriangle::isThreePointsOnOneLine(it->start, it->end, points[i])) {
                continue;
            }
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
        
        
//        for (auto tri : delTris) {
//            line(delImage, tri.points[0], tri.points[1], Scalar(255, 255, 255));
//            line(delImage, tri.points[1], tri.points[2], Scalar(255, 255, 255));
//            line(delImage, tri.points[0], tri.points[2], Scalar(255, 255, 255));
//        }
//        for (int j = 0; j <= i; j++) {
//            circle(delImage, points[j], 10, Scalar(255, 255, 255));
//        }
//        imwrite(("del/del" + to_string(c++) + ".jpg").c_str(), delImage);
//        
//        for (auto tri : tempTris) {
//            line(tempImage, tri.points[0], tri.points[1], Scalar(255, 255, 255));
//            line(tempImage, tri.points[1], tri.points[2], Scalar(255, 255, 255));
//            line(tempImage, tri.points[0], tri.points[2], Scalar(255, 255, 255));
//        }
//        for (int j = 0; j <= i; j++) {
//            circle(tempImage, points[j], 10, Scalar(255, 255, 255));
//        }
//        imwrite(("temp/tmp" + to_string(cc++) + ".jpg").c_str(), tempImage);
//        
//        cout << i << ": " << points[i] << endl;
        
//        cout << "===== delTri ====" << endl;
//        for (auto tri : delTris) {
//            cout << tri.points[0] << "  " << tri.points[1] << "  " << tri.points[2] << endl;
//        }
//        cout << "===== tmpTri ====" << endl;
//        for (auto tri : tempTris) {
//            cout << tri.points[0] << "  " << tri.points[1] << "  " << tri.points[2] << endl;
//        }
        
    }
    
//    cout << "delTri size====>" << delTris.size() << endl;
    
    // 将triangles与temp triangles进行合并, 除去与超级三角形有关的三角形
    for (auto tri : tempTris) {
        if (tri.indexs[0] == -1 || tri.indexs[1] == -1 || tri.indexs[2] == -1) {
            continue;
        } else {
            delTris.push_back(tri);
        }
    }
    for (auto it = delTris.begin(); it != delTris.end();) {
        if (it->indexs[0] == -1 || it->indexs[1] == -1 || it->indexs[2] == -1) {
            it = delTris.erase(it);
        } else {
            ++it;
        }
    }
    
    Mat image(2000, 2000, CV_8UC1, Scalar::all(0));
    for (auto tri : delTris) {
        line(image, tri.points[0], tri.points[1], Scalar(255, 255, 255));
        line(image, tri.points[1], tri.points[2], Scalar(255, 255, 255));
        line(image, tri.points[0], tri.points[2], Scalar(255, 255, 255));
    }
    for (int i = 0; i < points.size(); i++) {
        circle(image, points[i], 10, Scalar(255, 255, 255));
    }
    imwrite("final.jpg", image);
    
}

list<DelTriangle> Delaunay::getDelaunayTriangulation() {
    delaunayTriangulation();
    return this->delTris;
}

