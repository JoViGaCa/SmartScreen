#pragma once
#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;


class DetectSkin{
    private:
        // HSV system values
        int hue_Low_Threshold = 0;
        int hue_High_Threshold = 0;
        int sat_Low_Threshold = 0;
        int sat_High_Threshold = 0;
        int bri_Low_Threshold = 0;
        int bri_High_Threshold = 0;

        bool calibrated = false;

        Rect skinColor1, skinColor2;

        void calculateThresold(Mat sample1, Mat sample2);

        void performOpening(Mat binaryImg, int structuralElSamp, Point structuralElSize);

    public:

        void drawSkinColorRect(Mat input);

        void calibrate(Mat input);

        Mat getSkinMask(Mat input);

};









