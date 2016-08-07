//
//  main.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <iostream>
#include <vector>
#include "FiltUtil.h"
#include "DataProc.h"
#include "Morphing.h"
#include "DrawUtil.h"
#include "BezierUtil.h"
#include "MySpline.h"
#include "SVGWrap.h"

#include <opencv2/opencv.hpp>
using namespace cv;

int main(int argc, const char * argv[]) {
    vector<vector<Point>> faces = FileUtil::read_all_asm_points("dataBase(old).txt");
    vector<vector<Point>> svgs = FileUtil::read_svg_points("allFaceSvgPoint.txt");
    
    vector<Point> face = FileUtil::read_asm_point("data.txt");
    vector<Point> targetFace = faces[0];
    
    SVGWrap svgWrap(face, targetFace, svgs[3]);
    
    
}













