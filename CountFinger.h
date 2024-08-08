#pragma once

#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

class CountFinger{
	private:
		Scalar color_blue = Scalar(255,0,0);
		Scalar color_green = Scalar(0,255,0);
		Scalar color_red = Scalar(0,0,255);
		Scalar color_black = Scalar(0,0,0);
		Scalar color_white = Scalar(255,255,255);
		Scalar color_yellow = Scalar(0,255,255);
		Scalar color_purple = Scalar(255,0,255);
		double findPDistance(Point a, Point b);
		vector<Point> compactNeiMedian(vector<Point> points, double max_nei_dist);
		double findAng(Point a, Point b, Point c);
		bool isFinger(Point a, Point b, Point c, double limit_angle_inf, double limit_angle_sup, Point palm_center, double distance_tol);
		vector<Point> findClosestX(vector<Point> points, Point pivot);
		double findPointDistX(Point a, Point b);
		void drawVectorPoints(Mat input, vector<Point> points, Scalar color, bool with_num);

	public:
		Mat findFingCount(Mat input, Mat frame);





};