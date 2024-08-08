#include "BackRemover.h"
#include <opencv2/opencv.hpp>


void BackRemover::calibrate(Mat input){
    cvtColor(input, background, COLOR_BGR2GRAY);
    calibrated = true;
}

Mat BackRemover::getForeground(Mat input){
    Mat foreMask = getForegroundMask(input);
    Mat foreground;
    input.copyTo(foreground,foreMask);
    return foreground;
}

Mat BackRemover::getForegroundMask(Mat input){
    Mat foreMask;

    if(!calibrated){
        foreMask = Mat::zeros(input.size(),CV_8UC1);
        return foreMask;
    }

    cvtColor(input, foreMask,COLOR_BGR2GRAY);
    removeBack(foreMask,background);
    return foreMask;

}


void BackRemover::removeBack(Mat input, Mat back){
    int threshold = 10;

    for(int i = 0; i < input.rows; i++){
        for(int j = 0; j < input.cols; j++){
            // get the value of the pixels of the input and the background
            uchar framePixel = input.at<uchar>(i,j);
            uchar bgPixel = back.at<uchar>(i,j);

            // compare the values and set to 255 or 0
            if(framePixel >= bgPixel - threshold && framePixel <= bgPixel + threshold){
                input.at<uchar>(i,j) = 0;
            } else {
                input.at<uchar>(i,j) = 255;
            }
        }
    }

}






