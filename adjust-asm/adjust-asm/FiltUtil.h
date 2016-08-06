//
//  FiltUtil.h
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __face1__FiltUtil__
#define __face1__FiltUtil__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class FileUtil {
public:
    static vector<vector<Point> > read_all_asm_points(string file);
    static vector<vector<Point> > read_svg_points(string file);
};

#endif
