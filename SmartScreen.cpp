#include <iostream>
#include <sstream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/core.hpp>
#include <QApplication>
#include <QPushButton>
#include <QImage>
#include <QPixmap>
#include "DetectSkin.h"
#include "BackRemover.h"
#include "DetectFace.h"
#include "CountFinger.h"
#include "Interface.h"

using namespace cv;
using namespace std;

#define WINDOW_WIDTH  1280
#define WINDOW_HEIGHT  720
#define BUTTON_WIDTH   150
#define BUTTON_HEIGHT   50
#define BUTTON_POS_X  1000
#define BUTTON_POS_Y   100

// convert Mat to QImage
QImage MatToQImage(const Mat &mat) {
    if (mat.type() == CV_8UC1) { // Grayscale
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_Grayscale8);
        return img.copy(); 
    } else if (mat.type() == CV_8UC3) { // BGR
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_BGR888);
        return img.copy(); 
    } else if (mat.type() == CV_8UC4) { // BGRA
        QImage img(mat.data, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return img.copy(); 
    } else {
        // Formato não suportado
        return QImage();
    }
}



int main(int argc, char** argv){
	// Start Qt app
	QApplication app(argc, argv);

	// Start the main window
	Interface main_window;
	main_window.show();


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

	// White Board
	Mat board = Mat::ones(Size(1280,720),CV_8UC1);


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

		// resize the images
		resize(frame,frame,Size(426,240));
		resize(hdMask,hdMask,Size(426,240));
		resize(fgCount,fgCount,Size(426,240));
		resize(board,board,Size(426,240));

		// set the images on the main window
		main_window.setCamImg(MatToQImage(frame));
		main_window.setProcessImg(MatToQImage(hdMask));
		main_window.setOutPutImg(MatToQImage(fgCount));
		main_window.setBoardImg(MatToQImage(board));

				
		//get the input from the keyboard
        int keyboard = waitKey(30);
        if (keyboard == 'q' || keyboard == 27)
            break;
		else if (keyboard == 98)
			bgRm.calibrate(frame);
		else if (keyboard == 115)
			detSkin.calibrate(frame);

	}


	return app.exec();
}


