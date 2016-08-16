//
//  Morphing.cpp
//  morphing
//
//  Created by xyz on 16/8/15.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "Morphing.h"
#include "DrawUtil.h"
#include "Delaunay.h"

int Morphing::POINT_INDEX[CONTROL_POINT_NUM] = {
    0,1,2,3,4,5,6,7,8,9,10,11,12,38,39,52,54,58, 77, 78,79,80,81,82,83,84,85
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
 * What's more, the position of points are better to be normalized.
 */
void Morphing::init(vector<Point> &srcPoints, vector<Point> &destPoints) {
    assert(srcPoints.size() == POINT_NUM && destPoints.size() == POINT_NUM);
    //    for (int i = 0; i < Morphing::TRI_NUM; i++) {
    //        vector<Point> src;
    //        src.push_back(srcPoints[POINT_INDEX[i][0]]);
    //        src.push_back(srcPoints[POINT_INDEX[i][1]]);
    //        src.push_back(srcPoints[POINT_INDEX[i][2]]);
    //        Triangular srcTri(src);
    //
    //        vector<Point> dest;
    //        dest.push_back(destPoints[POINT_INDEX[i][0]]);
    //        dest.push_back(destPoints[POINT_INDEX[i][1]]);
    //        dest.push_back(destPoints[POINT_INDEX[i][2]]);
    //        Triangular destTri(dest);
    //
    //        srcTri.set_dest_points(dest);
    //        destTri.set_dest_points(src);
    //
    //        this->srcTris.push_back(srcTri);
    //        this->destTris.push_back(destTri);
    //    }
    vector<Point> points;
    vector<int> pointIndexs;
    for (int i = 0; i < Morphing::CONTROL_POINT_NUM; i++) {
        points.push_back(srcPoints[Morphing::POINT_INDEX[i]]);
        pointIndexs.push_back(Morphing::POINT_INDEX[i]);
    }
    
    Delaunay delaunay(points, pointIndexs);
    list<DelTriangle> delTris = delaunay.getDelaunayTriangulation();
    for (auto tri : delTris) {
        vector<Point> src;
        src.push_back(srcPoints[tri.indexs[0]]);
        src.push_back(srcPoints[tri.indexs[1]]);
        src.push_back(srcPoints[tri.indexs[2]]);
        Triangle srcTri(src);
        
        vector<Point> dest;
        dest.push_back(destPoints[tri.indexs[0]]);
        dest.push_back(destPoints[tri.indexs[1]]);
        dest.push_back(destPoints[tri.indexs[2]]);
        Triangle destTri(dest);
        
        srcTri.setDestTriangle(dest);
        destTri.setDestTriangle(src);
        
        this->srcTris.push_back(srcTri);
        this->destTris.push_back(destTri);
    }
    this->isInit = true;
}

bool Morphing::findSrcTri(Point p, Triangle &t) {
    assert(isInit == true && "Morphing should init first");
    for (auto tri : this->srcTris) {
        if (tri.isInTriangle(p)) {
            t = tri;
            return true;
        }
    }
    return false;
}

bool Morphing::findDestTri(Point p, Triangle &t) {
    assert(isInit == true && "Morphing should init first");
    for (auto tri : this->destTris) {
        if (tri.isInTriangle(p)) {
            t = tri;
            return true;
        }
    }
    return false;
}


Mat Morphing::morphImg(const Mat &mat, vector<Point> &srcPoints, vector<Point> &destPoints) {
    assert(isInit == true && "Morphing should init first");
    assert(srcPoints.size() == POINT_NUM && destPoints.size() == POINT_NUM);
    Mat faceMat(mat);
    
    //cvtColor(faceMat, faceMat, CV_BGR2GRAY);
    
    Mat morphingMat(destPoints[80].y+100, destPoints[80].x+100, faceMat.type(), Scalar::all(0));
    for (int row = destPoints[78].y; row <= destPoints[79].y; row++) {
        for (int col = destPoints[78].x; col <= destPoints[81].x; col++) {
            Triangle t;
            Point p(col, row);
            if (this->findDestTri(p, t)) {
                Point srcP = t.affine.transform(p);
                morphingMat.at<Vec3b>(row, col) = faceMat.at<Vec3b>(srcP.y,srcP.x);
                //morphingMat.at<uchar>(row, col) = faceMat.at<uchar>(srcP.y,srcP.x);
            }
        }
    }
    return morphingMat;
    
}

void Morphing::morphBezier(vector<Point> &bezier) {
    assert(isInit == true && "Morphing should init first");
    for (int i = 0; i < bezier.size(); i++) {
        Triangle t;
        if (findSrcTri(bezier[i], t)) {
            bezier[i] = t.affine.transform(bezier[i]);
        } else {
            cout << "not in tri: " << bezier[i] << endl;
        }
    }
    
}