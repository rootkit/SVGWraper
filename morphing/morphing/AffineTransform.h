//
//  AffineTransform.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__AffineTransform__
#define __morphing__AffineTransform__

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
    void initTransform(const vector<Point> &src, const vector<Point> &dest);
    void operator=(const AffineTransform& other);
    Point transform(Point p);
};

#endif /* defined(__morphing__AffineTransform__) */
