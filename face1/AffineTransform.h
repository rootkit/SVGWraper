//
//  AffineTransform.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__AffineTransform__
#define __face1__AffineTransform__

#include <stdio.h>
#include <cassert>
#include <vector>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "Eigen/Dense"
#include "Eigen/LU"

using namespace Eigen;

class AffineTransform {
private:
    float a00, a01, a02, a03, a04, a05;
public:
    AffineTransform();
    AffineTransform(const AffineTransform &other);
    AffineTransform(const vector<Point> &src, const vector<Point> &dest);
    ~AffineTransform();
    void init_transform(const vector<Point> &src, const vector<Point> &dest);
    void operator=(const AffineTransform& other);
    Point transform(Point p);
};

#endif /* defined(__face1__AffineTransform__) */
