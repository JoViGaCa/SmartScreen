#include "DetectFace.h"
#include <opencv2/opencv.hpp>

String classifierFile = "ref/haarcascade_frontalface_alt.xml";
CascadeClassifier faceCascadeClassifier;

void DetectFace::removeFaces(Mat input, Mat output){
    if (!faceCascadeClassifier.load(classifierFile)){
		  throw runtime_error("can't load file " + classifierFile);
    }
    
    vector<Rect> faces;
    Mat frame_gray;

    cvtColor(input, frame_gray, COLOR_BGR2GRAY);
    equalizeHist(frame_gray,frame_gray);

    faceCascadeClassifier.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(120, 120));
    // draw rectangles
    for (int i = 0; i < faces.size(); i++){
        rectangle(output, Point(faces[i].x,faces[i].y), Point(faces[i].x + faces[i].width,faces[i].y + faces[i].height), Scalar(0,0,0), -1);
    }

}


