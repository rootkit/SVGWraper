//
//  FiltUtil.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "FiltUtil.h"


vector<vector<Point> > FileUtil::read_all_asm_points(string file) {
    ifstream fin(file);
    vector<vector<Point> > data;
    vector<Point> points;
    double x, y;
    string temp;
    for (int i = 0; i < 46; i++) {
        //getline(fin, temp);
        for (int j = 0; j < 77; j++) {
            fin >> x >> y;
            points.push_back(Point((int)x, (int)y));
        }
        
        //预设头顶的坐标
//        int headPointY = (int) (points[14].y - (points[6].y - points[14].y) * 0.25);
//        int headPointX = points[14].x;
//        points.push_back(Point(headPointX, headPointY));
        
        data.push_back(points);
        points.clear();
    }
    fin.close();
    return data;
}

vector<vector<Point> > FileUtil::read_svg_points(string file) {
    ifstream fin(file);
    vector<vector<Point> > svgs;
    string tmp;
    int index;
    stringstream ss;
    while (fin >> index) {
        char c = fin.get();
        getline(fin, tmp);
        vector<Point> svg;
        int x, y;
        ss << tmp;
        while (ss >> x >> y) {
            svg.push_back(Point(x, y));
        }
        ss.clear();
        
        while (svgs.size() < index) {
            svgs.push_back(vector<Point>());
        }
        svgs.push_back(svg);
        
    }
    fin.close();
    return svgs;
}

vector<Point> FileUtil::read_test_data(string file) {
    ifstream fin(file);
    string t;
    vector<Point> ret;
    int x, y;
    for (int i = 0; i < 78; i++) {
        fin >> t;
        fin >> t;
        fin >> t;
        fin >> t;
        fin >> t;
        fin >> x;
        fin >> y;
        ret.push_back(Point(x, y));
    }
    //Point p(ret[14].x, ret[14].y-(ret[6].y-ret[14].y) * 0.25);
    //ret.push_back(p);
    fin.close();
    return ret;
}

vector<Point> FileUtil::read_asm_point(string file) {
    ifstream fin(file);
    vector<Point> face;
    int x, y;
    for (int i = 0; i < 77; i++) {
        fin >> x >> y;
        face.push_back(Point(x, y));
    }
    fin.close();
    return face;
}

