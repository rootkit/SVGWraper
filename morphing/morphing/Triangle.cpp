//
//  Triangle.cpp
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Triangle.h"

Triangle::Triangle(const vector<Point> &srcTris) {
    this->srcTris = srcTris;
}

Triangle::Triangle(const Triangle& tri) {
    *this = tri;
}

Triangle::~Triangle() {
    this->srcTris.clear();
    this->destTris.clear();
}

void Triangle::operator=(const Triangle& tri) {
    if (&tri == this) {
        return;
    }
    this->srcTris = tri.srcTris;
    this->destTris = tri.destTris;
    this->affine = tri.affine;
}

void Triangle::setDestTriangle(const vector<Point> &destTris) {
    this->destTris = destTris;
    this->affine.initTransform(this->srcTris, this->destTris);
}

bool Triangle::isInTriangle(Point p) {
    Point ab(srcTris[1].x - srcTris[0].x, srcTris[1].y - srcTris[0].y);
    Point ac(srcTris[2].x - srcTris[0].x, srcTris[2].y - srcTris[0].y);
    Point ap(p.x - srcTris[0].x, p.y - srcTris[0].y);
    double u = (ac.x*ap.y - ap.x*ac.y) / (double)(ac.x*ab.y - ab.x*ac.y);
    double v = (ab.x*ap.y - ap.x*ab.y) / (double)(ab.x*ac.y - ac.x*ab.y);
    return (u >= 0 && v >= 0 && u+v <= 1);
}