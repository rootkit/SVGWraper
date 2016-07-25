//
//  Triangular.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Triangular.h"



Triangular::Triangular(const vector<Point> &srcTris) {
    this->srcTris = srcTris;
}

Triangular::Triangular(const Triangular& tri) {
    *this = tri;
}

Triangular::~Triangular() {
    this->srcTris.clear();
    this->destTris.clear();
}

void Triangular::operator=(const Triangular& tri) {
    if (&tri == this) {
        return;
    }
    this->srcTris = tri.srcTris;
    this->destTris = tri.destTris;
    this->affine = tri.affine;
}

void Triangular::set_dest_points(const vector<Point> &destTris) {
    this->destTris = destTris;
    this->affine.init_transform(this->srcTris, this->destTris);
}

bool Triangular::is_in_triangular(Point p) {
    Point ab(srcTris[1].x - srcTris[0].x, srcTris[1].y - srcTris[0].y);
    Point ac(srcTris[2].x - srcTris[0].x, srcTris[2].y - srcTris[0].y);
    Point ap(p.x - srcTris[0].x, p.y - srcTris[0].y);
    double u = (ac.x*ap.y - ap.x*ac.y) / (double)(ac.x*ab.y - ab.x*ac.y);
    double v = (ab.x*ap.y - ap.x*ab.y) / (double)(ab.x*ac.y - ac.x*ab.y);
    return (u >= 0 && v >= 0 && u+v <= 1);
}