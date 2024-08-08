#include "CountFinger.h"
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#define LIMIT_ANGLE_SUP 60
#define LIMIT_ANGLE_INF 5
#define BOUNDING_RECT_FINGER_SIZE_SCALING 0.3
#define BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING 0.05

Mat CountFinger::findFingCount(Mat input, Mat frame){
	Mat contours_img = Mat::zeros(input.size(),CV_8UC1);

	// verify if the image isn't empty
	if(input.empty()){
		return contours_img;
	}
	
	// check the channels of the image
	if(input.channels() != 1){
		return contours_img;
	}

	// get the image contours (contorno)
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	findContours(input,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_NONE);

	if (contours.size() <= 0){
		return contours_img;
	}

	// get the biggest contour
	int biggest_index = -1;
	double biggest_area = 0.0;

	for(int i = 0; i < contours.size(); i++){
		double area = contourArea(contours[i],false);

		if (area > biggest_area){
			biggest_area = area;
			biggest_index = i;
		}
	}

	if(biggest_index < 0){
		return contours_img;
	}

	// get the convex hull (envoltório convexo)
	vector<Point> hull_points;
	vector<int> hull_ints;
	// for the bounding rectangle
	convexHull(Mat(contours[biggest_index]), hull_points, true);
	// for the defects
	convexHull(Mat(contours[biggest_index]), hull_ints, false);

	// verify if there is at least 3 points
	vector<Vec4i> defects;
	if (hull_ints.size() > 3){
		convexityDefects(Mat(contours[biggest_index]), hull_ints, defects);
	} else {
		return contours_img;
	}

	// bound a rectangle and find its center
	Rect bounding_rect = boundingRect(Mat(hull_points));
	Point rect_center((bounding_rect.tl().x + bounding_rect.br().x)/2, (bounding_rect.tl().y + bounding_rect.br().y)/2);

	// separete the defects
	vector<Point> start_points;
	vector<Point> far_points;
	for(int i = 0; i < defects.size(); i++){
		start_points.push_back(contours[biggest_index][defects[i].val[0]]);
		// filter the far points based on the distance from the center of the rect
		if(findPDistance(contours[biggest_index][defects[i].val[2]], rect_center) < bounding_rect.height*BOUNDING_RECT_FINGER_SIZE_SCALING){
			far_points.push_back(contours[biggest_index][defects[i].val[2]]);
		}
	}

	// compact on eht median
	vector<Point> filtered_start_points = compactNeiMedian(start_points, bounding_rect.height*BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING);
	vector<Point> filtered_far_points = compactNeiMedian(far_points, bounding_rect.height*BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING);

	// Find fingers
	vector<Point> filtered_fingers_points;

	if(filtered_far_points.size() > 1){
		vector<Point> finger_points;

		for(int i = 0; i < filtered_start_points.size(); i++){
			vector<Point> closest_point = findClosestX(filtered_fingers_points, filtered_start_points[i]);

			if(isFinger(closest_point[0], filtered_start_points[i],closest_point[1],LIMIT_ANGLE_INF,LIMIT_ANGLE_SUP, rect_center, bounding_rect.height*BOUNDING_RECT_FINGER_SIZE_SCALING)){
				finger_points.push_back(filtered_start_points[i]);
			}
		}

		if(finger_points.size() > 0){
			if(finger_points.size() > 5){
				finger_points.pop_back();
			}

			// filter the finger too close
			for (int i = 0; i < finger_points.size() - 1; i++){
				if(findPointDistX(finger_points[i],finger_points[i+1]) > bounding_rect.height*BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING*1.5){
					filtered_fingers_points.push_back(finger_points[i]);
				}
			}

			if(finger_points.size() > 2){
				if(findPointDistX(finger_points[0],finger_points[finger_points.size()-1]) > bounding_rect.height*BOUNDING_RECT_NEIGHBOR_DISTANCE_SCALING*1.5){
					filtered_fingers_points.push_back(finger_points[finger_points.size() -1]);
				}
			} else {
				filtered_fingers_points.push_back(finger_points[finger_points.size() -1]);
			}
		}
	}

	// draw the found on the image
	drawContours(contours_img,contours,biggest_index, color_green, 2, 8, hierarchy);
	polylines(contours_img, hull_points, true, color_blue);
	rectangle(contours_img, bounding_rect.tl(), bounding_rect.br(), color_red, 2, 8, 0);
	circle(contours_img, rect_center, 5, color_purple, 2, 8);
	drawVectorPoints(contours_img, filtered_start_points, color_blue, true);
	drawVectorPoints(contours_img, filtered_far_points, color_red, true);
	drawVectorPoints(contours_img, filtered_fingers_points, color_yellow, false);
	putText(contours_img, to_string(filtered_fingers_points.size()), rect_center, FONT_HERSHEY_PLAIN, 3, color_purple);

	// draw on handmask
	drawContours(input,contours,biggest_index, color_green, 2, 8, hierarchy);
	polylines(input, hull_points, true, color_blue);
	rectangle(input, bounding_rect.tl(), bounding_rect.br(), color_red, 2, 8, 0);
	circle(input, rect_center, 5, color_purple, 2, 8);
	drawVectorPoints(input, filtered_start_points, color_blue, true);
	drawVectorPoints(input, filtered_far_points, color_red, true);
	drawVectorPoints(input, filtered_fingers_points, color_yellow, false);
	putText(input, to_string(filtered_fingers_points.size()), rect_center, FONT_HERSHEY_PLAIN, 3, color_purple);
	

	// and on the starting frame
	drawContours(frame, contours, biggest_index, color_green, 2, 8, hierarchy);
	circle(frame, rect_center, 5, color_purple, 2, 8);
	drawVectorPoints(frame, filtered_fingers_points, color_yellow, false);
	putText(frame, to_string(filtered_fingers_points.size()), rect_center, FONT_HERSHEY_PLAIN, 3, color_purple);

	return contours_img;
}

double CountFinger::findPDistance(Point a, Point b){
	Point dif = a-b;
	return sqrt(dif.ddot(dif));
}

vector<Point> CountFinger::compactNeiMedian(vector<Point> points, double max_nei_dist){
	vector<Point> median_points;
	
	if (points.size() == 0)		
		return median_points;

	if (max_nei_dist <= 0)
		return median_points;

	// we start with the first point
	Point reference = points[0];
	Point median = points[0];

	for (int i = 1; i < points.size(); i++) {
		if (findPDistance(reference, points[i]) > max_nei_dist){
			
			// the point is not in range, we save the median
			median_points.push_back(median);

			// we swap the reference
			reference = points[i];
			median = points[i];
		}
		else
			median = (points[i] + median) / 2;
	}

	// last median
	median_points.push_back(median);

	return median_points;

}
double CountFinger::findAng(Point a, Point b, Point c){
	double ab = findPDistance(a, b);
	double bc = findPDistance(b, c);
	double ac = findPDistance(a, c);
	// Lei dos cossenos
	return acos((ab * ab + bc * bc - ac * ac) / (2 * ab * bc)) * 180 / CV_PI;

}
bool CountFinger::isFinger(Point a, Point b, Point c, double limit_angle_inf, double limit_angle_sup, Point palm_center, double distance_tol){
	double angle = findAng(a, b, c);
	if (angle > limit_angle_sup || angle < limit_angle_inf)
		return false;

	// the finger point should not be under the two far points
	int delta_y_1 = b.y - a.y;
	int delta_y_2 = b.y - c.y;
	if (delta_y_1 > 0 && delta_y_2 > 0)
		return false;

	// the two far points should not be both under the center of the hand
	int delta_y_3 = palm_center.y - a.y;
	int delta_y_4 = palm_center.y - c.y;
	if (delta_y_3 < 0 && delta_y_4 < 0)
		return false;

	double distance_from_palm = findPDistance(b, palm_center);
	if (distance_from_palm < distance_tol)
		return false;
	
	// this should be the case when no fingers are up
	double distance_from_palm_far_1 = findPDistance(a, palm_center);
	double distance_from_palm_far_2 = findPDistance(c, palm_center);
	if (distance_from_palm_far_1 < distance_tol / 4 || distance_from_palm_far_2 < distance_tol / 4)
		return false;

	return true;
}
vector<Point> CountFinger::findClosestX(vector<Point> points, Point pivot){
	vector<Point> to_return(2);

	if (points.size() == 0)
		return to_return;

	double distance_x_1 = DBL_MAX;
	double distance_1 = DBL_MAX;
	double distance_x_2 = DBL_MAX;
	double distance_2 = DBL_MAX;
	int index_found = 0;

	for (int i = 0; i < points.size(); i++) {
		double distance_x = findPointDistX(pivot, points[i]);
		double distance = findPDistance(pivot, points[i]);

		if (distance_x < distance_x_1 && distance_x != 0 && distance <= distance_1) {
			distance_x_1 = distance_x;
			distance_1 = distance;
			index_found = i;
		}
	}

	to_return[0] = points[index_found];

	for (int i = 0; i < points.size(); i++) {
		double distance_x = findPointDistX(pivot, points[i]);
		double distance = findPDistance(pivot, points[i]);

		if (distance_x < distance_x_2 && distance_x != 0 && distance <= distance_2 && distance_x != distance_x_1) {
			distance_x_2 = distance_x;
			distance_2 = distance;
			index_found = i;
		}
	}

	to_return[1] = points[index_found];

	return to_return;
}
double CountFinger::findPointDistX(Point a, Point b){
	double to_return = 0.0;

	if (a.x > b.x)
		to_return = a.x - b.x;
	else
		to_return = b.x - a.x;

	return to_return;

}
void CountFinger::drawVectorPoints(Mat input, vector<Point> points, Scalar color, bool with_num){
	for (int i = 0; i < points.size(); i++) {
		circle(input, points[i], 5, color, 2, 8);
		if(with_num)
			putText(input, to_string(i), points[i], FONT_HERSHEY_PLAIN, 3, color);
	}

}
