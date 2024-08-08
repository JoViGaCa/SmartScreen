#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class DetectFace {
    public:
        void removeFaces(Mat input, Mat output);
};