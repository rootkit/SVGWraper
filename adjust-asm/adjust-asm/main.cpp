//
//  main.cpp
//  adjust-asm
//
//  Created by xyz on 16/8/6.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
using namespace std;

#include <opencv2/opencv.hpp>
using namespace cv;

#include "MouseCapture.h"
#include "FiltUtil.h"
#include "BezierUtil.h"
#include "DataProc.h"
#include "DrawUtil.h"

int main(int argc, const char * argv[]) {
    vector<vector<Point>> dataFaces = FileUtil::read_all_asm_points("dataBase.txt");
    vector<vector<Point>> svgs = FileUtil::read_svg_points("allFaceSvgPoint.txt");
    
    for (int i = 1; i < 2; i++) {
        if (i == 29) {
            continue;
        }
        vector<Point> bezier = BezierUtil::get_bezier_2(svgs[i]);
        vector<Point> face = dataFaces[i];
        
        // 贝塞尔曲线边界
        int left, right, top, bottom;
        double scale;
        Point translate;
        DataProc::alignSvgToAsm(face, bezier, scale, translate);
        BezierUtil::get_bezier_border(bezier, left, right, top, bottom);
        
        Mat image(bezier[bottom].y+500, bezier[right].x+500, CV_8UC3, Scalar::all(0));
        BezierUtil::draw_bezier(image, bezier);
        
        DrawUtil::draw_points(image, face, 6);
        //imwrite(("asm" + to_string(i) + ".jpg").c_str(), image);
        
        // 调整asm点
        MouseCapture mc(image, face);
        mc.adjustPoints();
        
        face = mc.getAdjustedPoints();
        ofstream fout("data.txt");
        for (int i = 0; i < face.size(); i++) {
            fout << face[i].x << " " << face[i].y << " ";
        }
        fout.close();
        
//        image.release();
//        image.create(bezier[bottom].y+500, bezier[right].x+500, CV_8UC3);
//        image = Scalar::all(0);
//        BezierUtil::draw_bezier(image, bezier);
//        DrawUtil::draw_points(image, face, 6);
//        imwrite("adjust-asm.jpg", image);
    }
    
    
    return 0;
}
