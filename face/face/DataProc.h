//
//  DataProc.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__DataProc__
#define __face1__DataProc__

#include <stdio.h>

#include <opencv2/opencv.hpp>
using namespace cv;

class DataProc {
public:
    static void normalize_face_data(vector<Point> &srcPoints, vector<Point> &targetPoints);
};
#endif /* defined(__face1__DataProc__) */
