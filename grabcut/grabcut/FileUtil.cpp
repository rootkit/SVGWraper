//
//  FileUtil.cpp
//  grabcut
//
//  Created by xyz on 16/8/19.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "FileUtil.h"

vector<vector<Point> > FileUtil::read_all_asm_points(string file) {
    ifstream fin(file);
    vector<vector<Point> > data;
    vector<Point> points;
    double x, y;
    string temp;
    for (int i = 0; i < 62; i++) {
        //getline(fin, temp);
        for (int j = 0; j < 77; j++) {
            fin >> x >> y;
            points.push_back(Point((int)x, (int)y));
        }
        
        //预设头顶的坐标
        int headPointY = (int) (points[14].y - (points[6].y - points[14].y) * 0.25);
        int headPointX = points[14].x;
        points.push_back(Point(headPointX, headPointY));
        
        data.push_back(points);
        points.clear();
    }
    fin.close();
    return data;
}