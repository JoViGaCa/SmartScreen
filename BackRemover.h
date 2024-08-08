#pragma once

#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;


class BackRemover{
    private:
        Mat background;
        bool calibrated = false;

        Mat getForegroundMask(Mat input);
        void removeBack(Mat input, Mat back);

    public:
        void calibrate(Mat input);
        Mat getForeground(Mat input);

};


