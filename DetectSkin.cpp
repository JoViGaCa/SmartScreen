#include "DetectSkin.h"
#include <opencv2/opencv.hpp>

void DetectSkin::drawSkinColorRect(Mat input){
    // get input frame size
    int frame_width = input.size().width;
    int frame_height = input.size().height;

    // Rectangle variables
    int rect_size = 20;
    Scalar rect_color = Scalar(255,0,0);

    // Create rect for sampler 1 and 2
    skinColor1 = Rect(frame_width/5, frame_height/2, rect_size, rect_size);
    skinColor2 = Rect(frame_width/5, frame_height/3, rect_size, rect_size);
    rectangle(input, skinColor1,rect_color);
    rectangle(input, skinColor2,rect_color);

}

void DetectSkin::calibrate(Mat input){
    Mat hsvInput;
    // converter para HSv
    cvtColor(input,hsvInput,COLOR_BGR2HSV);

    Mat sample1 = Mat(hsvInput, skinColor1);
    Mat sample2 = Mat(hsvInput, skinColor2);

    calculateThresold(sample1, sample2);

    calibrated = true;
}



Mat DetectSkin::getSkinMask(Mat input){
    Mat skinMask;

    // Verify if the camera is calibrated
    if (!calibrated){
        skinMask = Mat::zeros(input.size(),CV_8UC1);
        return skinMask;
    }

    Mat hsvInput;
    // converter para HSv
    cvtColor(input,hsvInput,COLOR_BGR2HSV);

    // Perform an Threshikd operation
    inRange(hsvInput, Scalar(hue_Low_Threshold,sat_Low_Threshold,bri_Low_Threshold),Scalar(hue_High_Threshold, sat_High_Threshold, bri_High_Threshold),skinMask);

    performOpening(skinMask,MORPH_ELLIPSE, {3,3});
    dilate(skinMask,skinMask, Mat(), Point(-1,-1),3);

    return skinMask;
}

void DetectSkin::calculateThresold(Mat sample1, Mat sample2){
    // declare offsets
    int offset_Low_Threshold = 50;
    int offset_High_Threshold = 50;

    // create the means values
    Scalar hsv_mean_sample1 = mean(sample1);
    Scalar hsv_mean_sample2 = mean(sample2);

    // define the low and high values
    hue_Low_Threshold = min(hsv_mean_sample1[0],hsv_mean_sample2[0]) - offset_Low_Threshold;
    hue_High_Threshold = max(hsv_mean_sample1[0],hsv_mean_sample2[0]) + offset_High_Threshold;

    sat_Low_Threshold = min(hsv_mean_sample1[1],hsv_mean_sample2[1]) - offset_Low_Threshold;
    sat_High_Threshold = max(hsv_mean_sample1[1],hsv_mean_sample2[1]) + offset_High_Threshold;

    bri_Low_Threshold = min(hsv_mean_sample1[2],hsv_mean_sample2[2]) - offset_Low_Threshold;
    bri_High_Threshold = max(hsv_mean_sample1[2],hsv_mean_sample2[2]) + offset_High_Threshold;


}

void DetectSkin::performOpening(Mat binaryImg, int structuralElSamp, Point structuralElSize){
    Mat strucuringElement = getStructuringElement(structuralElSamp, structuralElSize);
    morphologyEx(binaryImg, binaryImg, MORPH_OPEN, strucuringElement);
}


