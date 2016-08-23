//
//  kmeansTest.cpp
//  grabcut
//
//  Created by xyz on 16/8/23.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "FileUtil.h"

#include <stdio.h>
#include <vector>

using namespace std;

#include <opencv2/opencv.hpp>

using namespace cv;


Scalar colorTab[] = {
    Scalar(0, 0, 0),
    Scalar(255, 255, 255),
};


void color_cluster(const Mat& origin_img_rgb) {
    //  1、将图像按像素点转化为样本矩阵samples
    Mat samples = Mat(origin_img_rgb.size().width*origin_img_rgb.size().height, 1, CV_32FC3);
    int k = 0;
    for (int i = 0; i < origin_img_rgb.rows; i++) {
        for (int j = 0; j < origin_img_rgb.cols; j++) {
            samples.at<cv::Vec3f>(k, 0)[0] = origin_img_rgb.at<cv::Vec3b>(i, j)[0];
            samples.at<cv::Vec3f>(k, 0)[1] = origin_img_rgb.at<cv::Vec3b>(i, j)[1];
            samples.at<cv::Vec3f>(k, 0)[2] = origin_img_rgb.at<cv::Vec3b>(i, j)[2];
            ++k;
        }
    }
    
    //  2、聚类
    Mat labels;
    Mat centers;
    int nCuster = 2;  //聚类类别数
    
    // samples      输入样本浮点矩阵
    // nCuster      给定聚类类别数量
    // labels       每个样本对应的类别标识
    // TermCriteria 指定聚类的最大迭代次数或精度
    kmeans(samples, nCuster, labels, TermCriteria(CV_TERMCRIT_ITER, 10, 1.0), 3, KMEANS_RANDOM_CENTERS, centers);
    
    //  3、将聚类结果转换为图像显示出来
    k = 0;
    Mat img(origin_img_rgb.size(), CV_8UC3);
    for (int i = 0; i < origin_img_rgb.rows; i++) {
        for (int j = 0; j < origin_img_rgb.cols; j++) {
            int clusterIdx = labels.at<int>(k++, 0);
            circle(img, {j,i}, 2, colorTab[clusterIdx], CV_FILLED, CV_AA);
        }
    }
    imshow("originimg", origin_img_rgb);
    imshow("clusters", img);
    imwrite("clusters.png", img);
    char key = (char)waitKey();
    if (key == 27 || key == 'q' || key == 'Q') {return ;}
}


void test(Mat &inputImg, Rect &faceRect) {
    
    Rect headRect;
    headRect.x = faceRect.x - 1.0 / 3.0 * faceRect.width;
    headRect.y = faceRect.y - 2.0 / 5.0 * faceRect.height;
    headRect.width = 5.0 / 3 * faceRect.width;
    headRect.height = 9.0/5 * faceRect.height;
    // 这一步先把头像抠出来
//    namedWindow("test");
//    imshow("test", inputImg);
//    waitKey();
    Mat bgModel, fgModel;
    Mat mask;
    grabCut(inputImg, mask, headRect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
    compare(mask, GC_PR_FGD, mask, CMP_EQ);
    
    Mat srcImg;
    
    inputImg.copyTo(srcImg, mask);
    imshow("test", srcImg);
    waitKey();
    imwrite("firstGrabcut.png", srcImg);
    
    color_cluster(srcImg);
    
    
}

int main() {
    vector<vector<Point> > facePoints = FileUtil::read_all_asm_points("dataBase(old).txt");
    
    Mat image = imread("1.png");
    
    vector<Point> facePoint = facePoints[1];
    Rect faceRect(facePoint[0].x, facePoint[14].y, facePoint[12].x-facePoint[0].x, facePoint[6].y-facePoint[14].y);
    
    test(image, faceRect);
    
    
    
}