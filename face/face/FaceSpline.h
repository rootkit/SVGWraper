//
//  FaceSpline.h
//  face
//
//  Created by xyz on 16/8/10.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face__FaceSpline__
#define __face__FaceSpline__

#include <stdio.h>

class FaceSpline {
public:
    static int* getSplinePoints(double *x, double *y, bool basedOnX, int size);
};

#endif /* defined(__face__FaceSpline__) */
