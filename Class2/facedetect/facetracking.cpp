#include <ctime>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <time.h>
#include <JHPWMPCA9685_RPI.h>
#include <raspicam/raspicam_cv.h>
#include <opencv2/opencv.hpp>
#include <wiringPi.h>
#include <wiringPiI2C.h>
#include <errno.h>
#include <iostream>
#include <stdio.h>

using namespace std;
using namespace cv;
int amap(int x, int in_min, int in_max, int out_min, int out_max) {
	int toReturn = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
	// For debugging:
	// printf("MAPPED %d to: %d\n", x, toReturn);
	return toReturn;
}
#define slaveaddress 0x40
/** Function Headers */
Mat detectAndDisplay(Mat frame);

/** Global variables */
String face_cascade_name = "haarcascade_frontalface_alt.xml";
String eyes_cascade_name = "haarcascade_eye_tree_eyeglasses.xml";
CascadeClassifier face_cascade;
CascadeClassifier eyes_cascade;
String window_name = "Capture - Face detection";

/** @function main */
int main(void)
{
	Mat frame;
	//set camera params
/*	wiringPiSetup();
	PCA9685 *pca9685 = new PCA9685(slaveaddress);
	int err = pca9685->openPCA9685();
	if (err < 0){
		printf("Error: %d", pca9685->error);
	}
	else {
		printf("PCA9685 Device Address: 0x%02X\n", pca9685->kI2CAddress);
		pca9685->setAllPWM(0, 0);
		pca9685->reset();
		pca9685->setPWMFrequency(600);
		// 27 is the ESC key
		printf("Hit ESC key to exit\n");
	}
*/
	raspicam::RaspiCam_Cv capture;
	capture.set(CV_CAP_PROP_FORMAT, CV_8UC3);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	capture.set(CV_CAP_PROP_FRAME_WIDTH,  640);
	//Open camera
	cout << "Opening Camera..." << endl;
	if (!capture.open()) { cerr << "Error opening the camera" << endl; return -1; }
	//-- 1. Load the cascades
	if (!face_cascade.load(face_cascade_name)){ printf("--(!)Error loading face cascade\n"); return -1; };
	if (!eyes_cascade.load(eyes_cascade_name)){ printf("--(!)Error loading eyes cascade\n"); return -1; };


	char key = 0;
	int testkey = 0;
	Mat drawimage;
	namedWindow("Hello");
	while (key != 'q')
	{
		testkey = waitKey(20);
		if (testkey != -1)
			key = testkey;

		capture.grab(); // get image from camera into memory
		capture.retrieve(frame); // get image from memory into Mat container
		if (frame.empty())
		{
			printf(" --(!) No captured frame -- Break!");
			break;
		}

		//-- 3. Apply the classifier to the frame
		detectAndDisplay(frame).copyTo(drawimage);
		//Mat A;
	//	A = Mat::zeros(640,480, CV_32F);
		imshow("Hello",  drawimage);

	}
	return 0;
}

/** @function detectAndDisplay */
Mat detectAndDisplay(Mat frame)
{
	std::vector<Rect> faces;
	Mat frame_gray;

	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	//-- Detect faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	for (size_t i = 0; i < faces.size(); i++)
	{
		Point center(faces[i].x + faces[i].width / 2, faces[i].y + faces[i].height / 2);
		ellipse(frame, center, Size(faces[i].width / 2, faces[i].height / 2), 0, 0, 360, Scalar(255, 0, 255), 4, 8, 0);

		Mat faceROI = frame_gray(faces[i]);
		std::vector<Rect> eyes;

		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale(faceROI, eyes, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

		for (size_t j = 0; j < eyes.size(); j++)
		{
			Point eye_center(faces[i].x + eyes[j].x + eyes[j].width / 2, faces[i].y + eyes[j].y + eyes[j].height / 2);
			int radius = cvRound((eyes[j].width + eyes[j].height)*0.25);
			circle(frame, eye_center, radius, Scalar(255, 0, 0), 4, 8, 0);
		}
	}
	//-- Show what you got
	return frame;
}
