//
//  Morphing.cpp
//  face1
//
//  Created by xyz on 16/7/25.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Morphing.h"
#include "DrawUtil.h"

const int Morphing::TRI_NUM = 93;
//const int Morphing::TRI_NUM = 51;
//const int Morphing::TRI_NUM = 31;
const int Morphing::POINT_NUM = 105;
//const int Morphing::POINT_NUM = 82;

//int Morphing::POINT_INDEX[31][3] = {
//    {78, 79, 2},  {81, 10, 80},
//    {2, 79, 3},   {10, 80, 9},
//    {3, 79, 4},   {9, 80, 8},
//    {4, 79, 5},   {8, 80, 7},
//    {5, 79, 6},   {7, 80, 6},
//    {78, 15, 14}, {81, 13, 14},
//    {78, 15, 2},  {81, 13, 10},
//    {15, 2, 3},   {13, 10, 9},
//    {3, 4, 59},   {9, 8, 65},
//    {59, 4, 5},   {65, 8, 7},
//    {74, 5, 6},   {74, 6, 7},
//    {15, 3, 59},  {13, 9, 65},
//    {59, 74, 5},  {74, 7, 65},
//    {15, 14, 59}, {13, 14, 65},
//    {14, 59, 74}, {14, 65, 74},
//    {78, 14, 81}
//};


//int Morphing::POINT_INDEX[51][3] = {
//    {78, 79, 0}, {81, 80, 12},
//    {79, 1, 0},  {80, 11, 12},
//    {79, 2, 1},  {80, 10, 11},
//    {79, 3, 2},  {80, 9, 10},
//    {79, 4, 3},  {80, 8, 9},
//    {79, 5, 4},  {80, 7, 8},
//    {79, 6, 5},  {80, 6, 7},
//    {78, 18, 0}, {81, 12, 25},
//    {78, 18, 15},{81, 25, 13},
//    {78, 15, 14},{81, 13, 14},
//    {59, 5, 6},  {65, 6, 7},
//    {59, 4, 5},  {65, 7, 8},
//    {59, 3, 4},  {65, 8, 9},
//    {59, 2, 3},  {65, 9, 10},
//    {2, 58, 59}, {54, 65, 10},
//    {1, 2, 58},  {11, 10, 54},
//    {58, 59, 56},{54, 65, 56},
//    {1, 38, 58}, {39, 11, 54},
//    {38, 58, 56},{39, 54, 56},
//    {0, 18, 38}, {25, 39, 12},
//    {0, 1, 38},  {39, 12, 11},
//    {18, 15, 38},{25, 13, 39},
//    {15, 14, 38},{14, 13, 39},
//    
//    {78, 81, 14},{59, 65, 6},
//    {56, 59, 65},{38, 39, 56},
//    {14, 38, 39}
//
//};

int Morphing::POINT_INDEX[TRI_NUM][3] = {
    {78,14,81},
    {78,14,96}, {81, 14,95 }, {78,96,15}, {81,95,13},
    {78,97,15}, {81, 94,13 }, {78,0, 97}, {81,94,12},
    {78,0, 98}, {81, 12,100}, {98,0, 82}, {100,12,93},
    {98,82, 1}, {100,93,11 }, {98,1, 83}, {100,11,92},
    {98,83, 2}, {100,92,10 }, {98,2, 99}, {100,10,101},
    {99,2, 84}, {101,10,91 }, {99,84,3 }, {101,91,9},
    {99,3, 85}, {101,9, 90 }, {99, 85, 79 }, {101,90,80},
    {85, 79, 4}, {90, 8, 80}, {4, 79,86}, {80,8, 89},
    {79,86, 5}, {80, 89,7  }, {79,5, 102}, {80,7, 104},
    {5, 87,102}, {7,104,88},  {102,87,103},{104,88,103},
    {87, 6,103}, {88,6,103},  {96,14,52}, {95,14,52},
    {96,15,52}, {95, 13, 52}, {97,15,38}, {13,94,39},
    {15,38,52}, {13, 39, 52}, {38,58,52}, {39,54,52},
    {97,18,38}, {94, 25, 39}, {97, 0,18}, {94,12,25},
    {18, 0,82}, {25, 12, 93}, {18,82,38}, {25,39,93},
    {38,82, 1}, {39, 93, 11}, {1, 38,58}, {39,11,54},
    {1, 58,83}, {11, 54, 92}, {58,83, 2}, {54,92,10},
    {58, 2,84}, {54, 10, 91}, {58,84, 3}, {54,91, 9},
    {58, 3,59}, {54, 91,  9}, {58, 3,59}, {54, 9,65},
    {59, 3,85}, {65,  9, 90}, {59,85, 4}, {65,90, 8},
    {59, 4,86}, {65,  8, 89}, {86, 5,59}, {65,7,89},
    {59, 5,87}, {65,88, 7},
    {59,87, 6}, {65, 88,  6}, {59,52,59}, {54,52,65},
    {52,59,65}, {59, 65,  6}

};


Morphing::Morphing() {
    this->isInit = false;
}


Morphing::Morphing(vector<Point> &srcPoints, vector<Point> &destPoints) {
    init(srcPoints, destPoints);
}


/**
 * The total size of points should be 82.
 * The first 78 points are asm points.
 * The last 4 points are four vertexes of boundary and
 * the order should be tl, bl, br, tr.
 * What's more, the position of points are better to be normalized
 */
void Morphing::init(vector<Point> &srcPoints, vector<Point> &destPoints) {
    assert(srcPoints.size() == POINT_NUM && destPoints.size() == POINT_NUM);
    for (int i = 0; i < Morphing::TRI_NUM; i++) {
        vector<Point> src;
        src.push_back(srcPoints[POINT_INDEX[i][0]]);
        src.push_back(srcPoints[POINT_INDEX[i][1]]);
        src.push_back(srcPoints[POINT_INDEX[i][2]]);
        Triangular srcTri(src);
        
        vector<Point> dest;
        dest.push_back(destPoints[POINT_INDEX[i][0]]);
        dest.push_back(destPoints[POINT_INDEX[i][1]]);
        dest.push_back(destPoints[POINT_INDEX[i][2]]);
        Triangular destTri(dest);
        
        srcTri.set_dest_points(dest);
        destTri.set_dest_points(src);
        
        this->srcTris.push_back(srcTri);
        this->destTris.push_back(destTri);
    }
    this->isInit = true;
}

bool Morphing::findSrcTri(Point p, Triangular &t) {
    assert(isInit == true && "Morphing should init first");
    for (auto tri : this->srcTris) {
        if (tri.is_in_triangular(p)) {
            t = tri;
            return true;
        }
    }
    return false;
}

bool Morphing::findDestTri(Point p, Triangular &t) {
    assert(isInit == true && "Morphing should init first");
    for (auto tri : this->destTris) {
        if (tri.is_in_triangular(p)) {
            t = tri;
            return true;
        }
    }
    return false;
}

Mat Morphing::scale_mat(Mat &mat, vector<Point> &points) {
    assert(points.size() == POINT_NUM);
    double width = points[81].x - points[78].x;
    double height = points[79].y - points[78].y;
    
    double scaleW = width / mat.cols;
    double scaleH = height / mat.rows;
    //    double minScale = scaleH <= scaleW ? scaleH : scaleW;
    
    Mat scaleMat;
    
    if (scaleW <= scaleH) {
        resize(mat, scaleMat, Size(mat.cols*width / mat.cols, mat.rows*width / mat.cols));
    } else {
        resize(mat, scaleMat, Size(mat.cols*height / mat.rows, mat.rows*height / mat.rows));
    }
    
    return scaleMat;
}

Mat Morphing::morphing_img(Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints) {
    assert(isInit == true && "Morphing should init first");
    assert(srcPoints.size() == POINT_NUM && destPoints.size() == POINT_NUM);
    //Mat faceMat(destPoints[80].y+100, destPoints[80].x+100, mat.type(), Scalar::all(0));
    Mat faceMat(srcPoints[80].y+100, srcPoints[80].x+100, mat.type(), Scalar::all(0));
    //Mat faceMat(4000, 4000, mat.type(), Scalar::all(0));
    
    Mat scaleMat = scale_mat(mat, srcPoints);

    // 将头像调整到与srcPoints一致的位置
    scaleMat.copyTo(faceMat(Rect(srcPoints[78].x, srcPoints[78].y, scaleMat.cols, scaleMat.rows)));
    
//    for (auto t : this->destTris) {
//        DrawUtil::draw_triangular(faceMat, t, Point(0, 0));
//    }
    
    cvtColor(faceMat, faceMat, CV_BGR2GRAY);
//    namedWindow("scale");
//    imshow("scale", faceMat);
//    imwrite("scale.jpg", faceMat);
//    waitKey();
    
    Mat morphingMat(destPoints[80].y+100, destPoints[80].x+100, faceMat.type(), Scalar::all(0));
    //Mat morphingMat(3000, 3000, faceMat.type(), Scalar::all(0));
    for (int row = destPoints[78].y; row <= destPoints[79].y; row++) {
        for (int col = destPoints[78].x; col <= destPoints[81].x; col++) {
            Triangular t;
            Point p(col, row);
            if (this->findDestTri(p, t)) {
                Point srcP = t.affine.transform(p);
                morphingMat.at<uchar>(row, col) = faceMat.at<uchar>(srcP.y,srcP.x);
            }
        }
    }
    return morphingMat;
    
}

void Morphing::morph_bezier(vector<Point> &bezier) {
    assert(isInit == true && "Morphing should init first");
    for (int i = 0; i < bezier.size(); i++) {
        Triangular t;
        if (findSrcTri(bezier[i], t)) {
            bezier[i] = t.affine.transform(bezier[i]);
            //cout << "(" << bezier[i].x << "," << bezier[i].y << ")" << endl;
        } else {
            cout << "not in tri: " << bezier[i] << endl;
        }
    }
    
}
