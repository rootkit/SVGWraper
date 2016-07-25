//
//  AffineTransform.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "AffineTransform.h"

AffineTransform::AffineTransform() {
    this->a00 = 0;
    this->a01 = 0;
    this->a02 = 0;
    this->a03 = 0;
    this->a04 = 0;
    this->a05 = 0;
}

AffineTransform::AffineTransform(const AffineTransform &other) {
    *this = other;
}

AffineTransform::AffineTransform(const vector<Point> &src, const vector<Point> &dest) {
    init_transform(src, dest);
}

AffineTransform::~AffineTransform() {
    
}

void AffineTransform::operator=(const AffineTransform& other) {
    if (&other == this) {
        return;
    }
    this->a00 = other.a00;
    this->a01 = other.a01;
    this->a02 = other.a02;
    this->a03 = other.a03;
    this->a04 = other.a04;
    this->a05 = other.a05;
}

void AffineTransform::init_transform(const vector<Point> &src, const vector<Point> &dest) {
    assert(src.size() == 3 && dest.size() == 3);
    Matrix3f A;
    Vector3f B;
    A << src[0].x, src[0].y, 1,
    src[1].x, src[1].y, 1,
    src[2].x, src[2].y, 1;
    B << dest[0].x, dest[1].x, dest[2].x;
    Vector3f X = A.lu().solve(B);
    this->a00 = X(0);
    this->a01 = X(1);
    this->a02 = X(2);
    
    B << dest[0].y, dest[1].y, dest[2].y;
    X = A.lu().solve(B);
    this->a03 = X(0);
    this->a04 = X(1);
    this->a05 = X(2);
}

Point AffineTransform::transform(Point p) {
    int x = p.x * a00 + p.y*a01 + a02;
    int y = p.x*a03 + p.y*a04 + a05;
    return Point(x, y);
}


