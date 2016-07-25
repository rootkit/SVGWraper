
//
//  Triangular.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__Triangular__
#define __face1__Triangular__

#include <stdio.h>
#include <vector>
#include "AffineTransform.h"

using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class Triangular {
public:
    vector<Point> srcTris;
    vector<Point> destTris;
    AffineTransform affine;
public:
    Triangular() {}
    Triangular(const vector<Point> &srcTris);
    Triangular(const Triangular& tri);
    ~Triangular();
    void operator=(const Triangular& tri);
    void set_dest_points(const vector<Point> &destTris);
    bool is_in_triangular(Point p);
};

#endif /* defined(__face1__Triangular__) */
