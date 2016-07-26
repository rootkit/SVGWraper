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
    int x, y;
    string temp;
    for (int i = 0; i < 29; i++) {
        getline(fin, temp);
        for (int j = 0; j < 78; j++) {
            fin >> x >> y;
            points.push_back(Point(x, y));
        }
        char c;
        fin >> c;
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
        //cout << c;
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