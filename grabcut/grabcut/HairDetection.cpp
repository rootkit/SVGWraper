//
//  HairDetection.cpp
//  grabcut
//
//  Created by xyz on 16/8/18.
//  Copyright (c) 2016年 xyz. All rights reserved.
//

#include "HairDetection.h"


HairDetection::HairDetection(Mat &inputImg, Rect &faceRect):
faceRect(faceRect), inputImg(inputImg) {
    
    assert(inputImg.type() == CV_8UC3);
    
    this->width = inputImg.cols;
    this->height = inputImg.rows;
    this->headRect.x = faceRect.x - 1.0 / 3.0 * faceRect.width;
    this->headRect.y = faceRect.y - 2.0 / 5.0 * faceRect.height;
    this->headRect.width = 5.0 / 3 * faceRect.width;
    this->headRect.height = 9.0/5 * faceRect.height;
    
    // 检测颜色时，设置在头顶的矩形
    this->topRect.x = faceRect.x;
    this->topRect.y = faceRect.y - faceRect.height / 4.0;
    this->topRect.width = faceRect.width;
    this->topRect.height = faceRect.height / 4.0;
    
    // 检测矩形框
    Mat temp(height, width, CV_8UC3, Scalar::all(0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            temp.at<Vec3b>(i, j) = inputImg.at<Vec3b>(i, j);
        }
    }
    rectangle(temp, headRect, Scalar(0, 255, 255));
    rectangle(temp, faceRect, Scalar(255, 255, 0));
    rectangle(temp, topRect, Scalar(255, 0, 255));
    imwrite("rect.jpg", temp);
    
    
    // 这一步先把头像抠出来
    // ....
    Mat bgModel, fgModel;
    imshow("test", inputImg);
    waitKey();
    grabCut(inputImg, firstMask, headRect, bgModel, fgModel, 1, GC_INIT_WITH_RECT);
    compare(firstMask, GC_PR_FGD, firstMask, CMP_EQ);

    inputImg.copyTo(srcImg, firstMask);
    imshow("test", srcImg);
    waitKey();
    imwrite("firstGrabcut.png", srcImg);
    
    grayImg = new float[width*height];
    colorMask = new unsigned char[width*height];
    fusionMask = new unsigned char[width*height];
    frequentialMask = new unsigned char[width*height];
    
    
    calFrequentialMask();
    calColorMask();
    calFusionMask();
    mattingProcess();
}


HairDetection::~HairDetection() {
    delete [] frequentialMask;
    delete [] colorMask;
    delete [] fusionMask;
    delete [] grayImg;
    srcImg.release();
    frequentialMask = NULL;
    colorMask = NULL;
    fusionMask = NULL;
    grayImg = NULL;
}

bool HairDetection::isInRect(Rect rect, int x, int y) {
    return x >= rect.x && x <= rect.x+rect.width && y >= rect.y && y <= rect.y+rect.height;
}



bool HairDetection::isInFrequentialMask(int index) {
    return this->frequentialMask[index] != 0x00;
}


bool HairDetection::isInColorMask(int index) {
    return this->colorMask[index] != 0x00;
}



Mat HairDetection::gaussianBlurFor(Mat img) {
    Mat dest;
    GaussianBlur(img, dest, Size(5, 5), 1.0, 1.0);
    return dest;
}


void HairDetection::calGaussianMeanAndStandardDeviation(Mat &gaussianImg) {
    float sum = 0.0;
    int num = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            grayImg[num] = 0.299*gaussianImg.at<Vec3b>(i, j)[0]+
            0.587*gaussianImg.at<Vec3b>(i, j)[1]+0.114*gaussianImg.at<Vec3b>(i, j)[2];
            sum += grayImg[num];
            num++;
        }
    }
    gaussianMean = sum / (width*height);
    for (int i = 0; i < num; i++) {
        gaussianStandardDeviation += (grayImg[i]-gaussianMean)*(grayImg[i]-gaussianMean);
    }
    gaussianStandardDeviation /= num;
    gaussianStandardDeviation = sqrt(gaussianStandardDeviation);
    
    Mat gua(height, width, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            gua.at<uchar>(i, j) = grayImg[i*width+j];
        }
    }
    imwrite("grayImg.png", gua);
    
}


void HairDetection::calFrequentialMask() {
    Mat frequentialMap = gaussianBlurFor(inputImg);
    calGaussianMeanAndStandardDeviation(frequentialMap);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            if (firstMask.at<uchar>(i, j) == 0x00) {
                frequentialMask[index] = 0x00;
                continue;
            }

            if (grayImg[i*width+j] <= gaussianMean - gaussianStandardDeviation) {
                frequentialMask[index] = 0xff;
            } else {
                frequentialMask[index] = 0x00;
            }
        }
    }
    
    Mat frequential(height, width, CV_8UC1);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            frequential.at<uchar>(i, j) = frequentialMask[index];
        }
    }
    imwrite("frequential.jpg", frequential);
    
}


float HairDetection::getStandardDeviation(float* array, float mean, int size) {
    float deviation = 0.0;
    for (int i = 0; i < size; i++) {
        deviation += (array[i]-mean)*(array[i]-mean);
    }
    deviation /= size;
    return sqrt(deviation);
}



void HairDetection::calHairColorMeanAndStandardDeviation() {
    float sumY = 0.0;
    float sumCB = 0.0;
    float sumCR = 0.0;
    
    float *Y = new float[width*height];
    float *Cb = new float[width*height];
    float *Cr = new float[width*height];
    int num = 0;
    
    for (int i = topRect.y; i <= topRect.y+topRect.height; i++) {
        for (int j = topRect.x; j <= topRect.x + topRect.width; j++) {
            float cb = 0.564 * (srcImg.at<Vec3b>(i, j)[0]-grayImg[i*width+j]);
            float cr = 0.713 * (srcImg.at<Vec3b>(i, j)[2]-grayImg[i*width+j]);
            if (!isInFrequentialMask(i*width+j)) {
                continue;
            }
            sumY += grayImg[i*width+j];
            sumCB += cb;
            sumCR += cr;
            Y[num] = grayImg[i*width+j];
            Cb[num] = cb;
            Cr[num] = cr;
            num++;
        }
    }
    
    // 求平均值
    hairColorYMean = sumY / num;
    hairColorYStandardDeviation = getStandardDeviation(Y, hairColorYMean, num);
    hairColorBMean = sumCB / num;
    hairColorBStandardDeviation = getStandardDeviation(Cb, hairColorBMean, num);
    hairColorRMean = sumCR / num;
    hairColorRStandardDeviation = getStandardDeviation(Cr, hairColorRMean, num);
    
    delete [] Y;
    delete [] Cb;
    delete [] Cr;
    Y = NULL;
    Cb = NULL;
    Cr = NULL;
}


void HairDetection::calColorMask() {
    calHairColorMeanAndStandardDeviation();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i * width + j;
            if (isInRect(headRect, j, i)) {
                float y = srcImg.at<Vec3b>(i,j)[0]*0.299 + srcImg.at<Vec3b>(i,j)[1]*0.587
                +srcImg.at<Vec3b>(i,j)[2]*0.114;
                float cb = 0.564 * (srcImg.at<Vec3b>(i,j)[0]-y);
                float cr = 0.713 * (srcImg.at<Vec3b>(i,j)[2]-y);
                if (y >= hairColorYMean - hairColorYStandardDeviation &&
                    y <= hairColorYMean + hairColorYStandardDeviation &&
                    cb >= hairColorBMean - hairColorBStandardDeviation &&
                    cb <= hairColorBMean + hairColorBStandardDeviation &&
                    cr >= hairColorRMean - hairColorRStandardDeviation &&
                    cr <= hairColorRMean + hairColorRStandardDeviation) {
                    colorMask[index] = 0xff;
                    continue;
                }
            }
            colorMask[index] = 0x00;
        }
    }
    
    Mat colorImg(height, width, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            colorImg.at<uchar>(i, j) = colorMask[index];
        }
    }
    imwrite("colorImg.jpg", colorImg);
}



void HairDetection::calFusionMask() {
//    for (int i = 0; i < height; i++) {
//        for (int j = 0; j < width; j++) {
//            int index = (i*width+j)*3;
//            if (isInFrequentialMask(index) && isInColorMask(index)) {
//                fusionMask[index] = 0xff;
//                fusionMask[index+1] = 0xff;
//                fusionMask[index+2] = 0xff;
//            
//            } else if (!isInFrequentialMask(index)) {
//                fusionMask[index] = 0x00;
//                fusionMask[index+1] = 0x00;
//                fusionMask[index+2] = 0x00;
//            }
//            else {
//                fusionMask[index] = srcImg.at<Vec3b>(i, j)[0];
//                fusionMask[index+1] = srcImg.at<Vec3b>(i, j)[1];
//                fusionMask[index+2] = srcImg.at<Vec3b>(i, j)[2];
//            }
////            else {
////                fusionMask[index] = 0x00;
////                fusionMask[index+1] = 0x00;
////                fusionMask[index+2] = 0x00;
////            }
//        }
//    }
    
    int num = 0;
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            fusionMask[num] = frequentialMask[num] & colorMask[num];
            num++;
        }
    }
    
    Mat fusionImg(height, width, CV_8UC1, Scalar::all(0));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            fusionImg.at<uchar>(i, j) = fusionMask[index];
        }
    }
    imwrite("fusionImg.png", fusionImg);
}


void HairDetection::mattingProcess() {
    //Mat inputImg(srcImg);
    Mat bgModel, fgModel;
    Mat mask(height, width, CV_8UC1, Scalar::all(GC_PR_BGD));
    
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            if (fusionMask[index] == 0xff) {
                mask.at<uchar>(i, j) = 0xff;
            }
//            else if (!isInFrequentialMask(index) || (isInFrequentialMask(index) && !isInRect(headRect, j, i))) {
//                mask.at<uchar>(i, j) = 0x66;
//            }
            else if (isInFrequentialMask(index) && isInRect(faceRect, j, i)) {
                mask.at<uchar>(i, j) = 0x66;
            }
            
        }
    }
    
    
    imwrite("mask1.png", mask);
    mask = Scalar::all(GC_PR_BGD);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int index = i*width+j;
            if (fusionMask[index] == 0xff) {
                mask.at<uchar>(i, j) = GC_FGD;
            }
//            else if (!isInFrequentialMask(index) || (isInFrequentialMask(index) && !isInRect(headRect, j, i))) {
//                mask.at<uchar>(i, j) = GC_BGD;
//            }
            else if (isInFrequentialMask(index) && isInRect(faceRect, j, i)) {
                mask.at<uchar>(i, j) = GC_BGD;
            }

        }
    }
    
    grabCut(srcImg, mask, headRect, bgModel, fgModel, 5, GC_INIT_WITH_MASK);
    
    //compare(mask, GC_FGD|GC_PR_FGD, mask, CMP_EQ);
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (mask.at<uchar>(i, j) == GC_PR_FGD || mask.at<uchar>(i, j) == GC_FGD) {
                mask.at<uchar>(i, j) = 255;
            } else {
                mask.at<uchar>(i, j) = 0;
            }
        }
    }
    
    imwrite("mask2.png", mask);

    srcImg.copyTo(hairImg, mask);
    
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (mask.at<uchar>(i, j) > 0) {
                inputImg.at<Vec3b>(i, j) = Vec3b(0, 255, 255);
            }
        }
    }
    imwrite("hair.png", inputImg);
    
}











