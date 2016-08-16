//
//  FileUtil.h
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#ifndef __morphing__FileUtil__
#define __morphing__FileUtil__

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

class FileUtil {
public:
    static vector<vector<Point> > readAsmPoints(string file);
    static vector<vector<Point> > readSvgPoints(string file);
    static vector<Point> readTestAsmFaceData(string file);
};

#endif /* defined(__morphing__FileUtil__) */
