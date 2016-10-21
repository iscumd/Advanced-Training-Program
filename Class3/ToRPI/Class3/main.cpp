#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;
// prototypes for main
void createTrackbars();
void tresh_update();

void on_trackbar1(int, void*)
{
	tresh_update();
}; // this function gets called whenever a trackbar position is changed

const int H_MAX = 255;

int H_threshold_MIN = 0; // starting values
int S_threshold_MIN = 0;
int V_threshold_MIN = 0;
int H_threshold_MAX = 255;
int S_threshold_MAX = 255;
int V_threshold_MAX = 255;
int R_threshold_MIN = 0;
int G_threshold_MIN = 0;
int B_threshold_MIN = 0;
int R_threshold_MAX = 255;
int G_threshold_MAX = 255;
int B_threshold_MAX = 255;

const string trackbarwindowname1 = "trackbars";

int main()
{
	Mat rgbimage = Mat::zeros(1, 1, CV_8UC3); // loaded image
	Mat HSVimage = Mat::zeros(1, 1, CV_8UC3);
	cv::Mat RGBthreshold = Mat::zeros(1, 1, CV_8UC3);     // binary
	cv::Mat HSVthreshold = Mat::zeros(1, 1, CV_8UC3); // binary

	string imagedirectory = ""; // where are the pictures located?
	string imgname = "fruit6.jpg"; //picture name
	rgbimage = imread(imagedirectory + imgname, CV_LOAD_IMAGE_COLOR); // load the  image
	if (!rgbimage.data) // check for invalid input
	{
		cout << "could not open or find the image" << std::endl;
		return -1;
	}
	cvtColor(rgbimage, HSVimage, COLOR_BGR2HSV);

	createTrackbars(); // initialize trackbars
	char key = 0;
	int testkey = 0;
	// main loop
	while ( key != 'q') { // press 'q' to quit

		// keyboard interface
		testkey = cv::waitKey(10);  // returns -1 if no key was pressed
		if (testkey != -1)
			key = testkey;

		// Compute Threshold Image based on threshold values
		cv::inRange(HSVimage, cv::Scalar(H_threshold_MIN, S_threshold_MIN, V_threshold_MIN), cv::Scalar(H_threshold_MAX, S_threshold_MAX, V_threshold_MAX), HSVthreshold);
		cv::inRange(rgbimage, cv::Scalar(R_threshold_MIN, G_threshold_MIN, B_threshold_MIN), cv::Scalar(R_threshold_MAX, G_threshold_MAX, B_threshold_MAX), RGBthreshold);
		imshow("rgbimage", rgbimage);
		imshow("RGBthreshold", RGBthreshold);
		imshow("HSVthreshold", HSVthreshold);
	}
	return 0;
}
void tresh_update()
{
	int temp;

	temp = cv::getTrackbarPos("H_MIN", trackbarwindowname1);
	H_threshold_MIN = temp;
	temp = cv::getTrackbarPos("H_MAX", trackbarwindowname1);
	H_threshold_MAX = temp;
	temp = cv::getTrackbarPos("S_MIN", trackbarwindowname1);
	S_threshold_MIN = temp;
	temp = cv::getTrackbarPos("S_MAX", trackbarwindowname1);
	S_threshold_MAX = temp;
	temp = cv::getTrackbarPos("V_MIN", trackbarwindowname1);
	V_threshold_MIN = temp;
	temp = cv::getTrackbarPos("V_MAX", trackbarwindowname1);
	V_threshold_MAX = temp;

	temp = cv::getTrackbarPos("R_MIN", trackbarwindowname1);
	R_threshold_MIN = temp;
	temp = cv::getTrackbarPos("R_MAX", trackbarwindowname1);
	R_threshold_MAX = temp;
	temp = cv::getTrackbarPos("G_MIN", trackbarwindowname1);
	G_threshold_MIN = temp;
	temp = cv::getTrackbarPos("G_MAX", trackbarwindowname1);
	S_threshold_MAX = temp;
	temp = cv::getTrackbarPos("B_MIN", trackbarwindowname1);
	B_threshold_MIN = temp;
	temp = cv::getTrackbarPos("B_MAX", trackbarwindowname1);
	B_threshold_MAX = temp;
}

void createTrackbars()
{ // create window for trackbars
	char trackbarname[200];

	// create trackbars window
	namedWindow(trackbarwindowname1, WINDOW_NORMAL);

	// create trackbars and insert them into window
	createTrackbar("H_MIN", trackbarwindowname1, &H_threshold_MIN, H_MAX, on_trackbar1);
	createTrackbar("H_MAX", trackbarwindowname1, &H_threshold_MAX, H_MAX, on_trackbar1);
	createTrackbar("S_MIN", trackbarwindowname1, &S_threshold_MIN, H_MAX, on_trackbar1);
	createTrackbar("S_MAX", trackbarwindowname1, &S_threshold_MAX, H_MAX, on_trackbar1);
	createTrackbar("V_MIN", trackbarwindowname1, &V_threshold_MIN, H_MAX, on_trackbar1);
	createTrackbar("V_MAX", trackbarwindowname1, &V_threshold_MAX, H_MAX, on_trackbar1);
	createTrackbar("R_MIN", trackbarwindowname1, &R_threshold_MIN, H_MAX, on_trackbar1);
	createTrackbar("R_MAX", trackbarwindowname1, &R_threshold_MAX, H_MAX, on_trackbar1);
	createTrackbar("G_MIN", trackbarwindowname1, &G_threshold_MIN, H_MAX, on_trackbar1);
	createTrackbar("G_MAX", trackbarwindowname1, &G_threshold_MAX, H_MAX, on_trackbar1);
	createTrackbar("B_MIN", trackbarwindowname1, &B_threshold_MIN, H_MAX, on_trackbar1);
	createTrackbar("B_MAX", trackbarwindowname1, &B_threshold_MAX, H_MAX, on_trackbar1);
}
