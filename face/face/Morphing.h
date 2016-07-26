//
//  Morphing.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__Morphing__
#define __face1__Morphing__

#include <stdio.h>
#include <vector>
#include "Triangular.h"
using namespace std;

class Morphing {
public:
    Morphing();
    Morphing(vector<Point> &srcPoints, vector<Point> &destPoints);
    void init(vector<Point> &srcPoints, vector<Point> &destPoints);
    bool findSrcTri(Point p, Triangular &t);
    bool findDestTri(Point p, Triangular &t);
    vector<Triangular> srcTris;
    vector<Triangular> destTris;
    Mat morphing_img(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints);
    void morph_bezier(vector<Point> &bezier);
private:
    static int POINT_INDEX[31][3];
    Mat scale_mat(Mat &mat, vector<Point> &points);
    bool isInit;
};

#endif /* defined(__face1__Morphing__) */
