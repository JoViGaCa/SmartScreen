#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core.hpp>
#include "DetectSkin.h"
#include "BackRemover.h"
#include "DetectFace.h"
#include "CountFinger.h"

using namespace cv;
using namespace std;



int main(){
	// Open camera
	VideoCapture capture(0);

	if (!capture.isOpened()){
		cout << "Error openning the camera" << endl;
		return 0;
	}

	// Declare the Matrices
	Mat frame, foreground, hdMask, fgCount, output;

	// Skin Detector
	DetectSkin detSkin;

	// Background Remover
	BackRemover bgRm;
	
	// Face Remover
	DetectFace detFc;	

	// Finger Count
	CountFinger fgCounter;


	while(1){

		// Take a frame
		capture >> frame;

		if (frame.empty()){
			break;
		}

		// flip frame
		flip(frame,frame,1);

		// Draw the ref rectangles
		detSkin.drawSkinColorRect(frame);

		// get the foreground
		foreground = bgRm.getForeground(frame);
		
		// Remove the Face
		detFc.removeFaces(frame, foreground);

		// get the handMask
		hdMask = detSkin.getSkinMask(foreground);

		// Count the number of finger
		fgCount = fgCounter.findFingCount(hdMask, frame);


		// show the frame and the mask
		//resize(frame,frame,Size(854,480));
		imshow("Frame",frame);
		//resize(foreground,foreground,Size(854,480));
		imshow("Foreground", foreground);
		//resize(hdMask,hdMask,Size(854,480));
		imshow("HandMask", hdMask);
		//resize(fgCount,fgCount,Size(854,480));
		imshow("Output", fgCount);

				
		//get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
		else if (keyboard == 98)
			bgRm.calibrate(frame);
		else if (keyboard == 115)
			detSkin.calibrate(frame);

	}


	return 0;
}


