//
//  FileUtil.h
//  grabcut
//
//  Created by xyz on 16/8/19.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __grabcut__FileUtil__
#define __grabcut__FileUtil__

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
};

#endif /* defined(__grabcut__FileUtil__) */
