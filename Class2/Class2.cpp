#include <opencv2/opencv.hpp>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <stdlib.h>
#include <ctime>
#include <string>
#include <sstream>

using namespace std;
using namespace cv;
// prototypes for main
void createTrackbars();
void tresh_update();
void on_trackbar1(int, void*)
{
	tresh_update();
}; // this function gets called whenever a trackbar position is changed
Mat drawimage; // image we can draw on and refresh

static void onMouse(int event, int x, int y, int, void*)
{
	if (event != EVENT_LBUTTONDOWN)
		return;

	Point pointClicked = Point(x, y);
	int radius = 4;
	circle(drawimage, pointClicked, radius, Scalar(0, 255, 0), -1, 8, 0);
}
template <typename T>
string to_string(T val)
{
stringstream stream;
stream << val;
return stream.str();
}
const int H_MIN = 0;
const int H_MAX = 255;
const int S_MIN = 0;
const int S_MAX = 255;
const int V_MIN = 0;
const int V_MAX = 255;
int H_threshold_MIN = 0;
int S_threshold_MIN = 0;
int V_threshold_MIN = 0;
int H_threshold_MAX = 255;
int S_threshold_MAX = 255;
int V_threshold_MAX = 255;
const int R_MIN = 0;
const int R_MAX = 255;
const int G_MIN = 0;
const int G_MAX = 255;
const int B_MIN = 0;
const int B_MAX = 255;
int R_threshold_MIN = 0;
int G_threshold_MIN = 0;
int B_threshold_MIN = 0;
int R_threshold_MAX = 255;
int G_threshold_MAX = 255;
int B_threshold_MAX = 255;

const string trackbarwindowname1 = "trackbars";

const static int SENSITIVITY_VALUE = 30;
// size of blur used to smooth the intensity image output from absdiff() function
const static cv::Size BLUR_SIZE = cv::Size(10, 10);
#define SHOWIMAGE(a) cv::imshow(#a,a); cv::waitKey(1);

#define numofimages 50

int main()
{
	int imageid = 1;
	Mat rgbimage; // loaded image

	Mat grayImage;
	Mat HSVimage;
	cv::Mat RGBthreshold;     // binary
	cv::Mat blurImage;       // grayscale
	cv::Mat HSVthreshold; // binary
	cv::Mat ANDthreshold;
	namedWindow("Draw Image", 1);

	//set the callback function for any mouse event
	setMouseCallback("Draw Image", onMouse, NULL);
	
	string imagedirectory = "../images/";
	string imgbasename = "fruit";
	cout << "before image read" << endl;
	rgbimage = imread(imagedirectory + imgbasename + to_string(imageid) + ".jpg", CV_LOAD_IMAGE_COLOR); // load the first image
	if (!rgbimage.data)                                                            // check for invalid input
	{
		cout << "could not open or find the image" << std::endl;
		return -1;
	}
	cout << "before color convert" << endl;
	cvtColor(rgbimage, grayImage, COLOR_BGR2GRAY);
	cvtColor(rgbimage, HSVimage, COLOR_BGR2HSV);
	rgbimage.copyTo(drawimage);
	resize(drawimage, drawimage, Size(640, 480));
	createTrackbars();

	char key = 0;
	int testkey = 0;
	// main loop
	cout << "before while loop" << endl;
	while (imageid > 0 && imageid < numofimages && key != 'q') {

		// keyboard interface
		 testkey = waitKey(20);
		 if (testkey != -1)
			 key = testkey;
		switch (key) {
		case 'n': // load the next image
			imageid++;
			rgbimage = imread(imagedirectory + imgbasename + to_string(imageid) + ".jpg", CV_LOAD_IMAGE_COLOR); // load the next image
			if (!rgbimage.data)                                                            // check for invalid input
			{
				cout << "could not open or find the image" << std::endl;
				return -1;
			}
			cvtColor(rgbimage, grayImage, COLOR_BGR2GRAY);
			cvtColor(rgbimage, HSVimage, COLOR_BGR2HSV);
			rgbimage.copyTo(drawimage);
			resize(drawimage, drawimage, Size(640, 480));
			key = 0;
			break;
		case 'p': // load the previous image
			imageid--;
			rgbimage = imread(imagedirectory + imgbasename + to_string(imageid) + ".jpg", CV_LOAD_IMAGE_COLOR); // load the next image
			if (!rgbimage.data)                                                            // check for invalid input
			{
				cout << "could not open or find the image" << std::endl;
				return -1;
			}
			cvtColor(rgbimage, grayImage, COLOR_BGR2GRAY);
			cvtColor(rgbimage, HSVimage, COLOR_BGR2HSV);
			rgbimage.copyTo(drawimage);
			resize(drawimage, drawimage, Size(640, 480));

			key = 0;
			break;
		case 'r': // refresh the draw image
			
			rgbimage.copyTo(drawimage);
			resize(drawimage, drawimage, Size(640, 480));
			key = 0;
			break;
		case 'h':
			cv::inRange(HSVimage, cv::Scalar(H_threshold_MIN, S_threshold_MIN, V_threshold_MIN), cv::Scalar(H_threshold_MAX, S_threshold_MAX, V_threshold_MAX), HSVthreshold);
			SHOWIMAGE(HSVthreshold);
			break;
		case 'g' :
			cv::inRange(rgbimage, cv::Scalar(R_threshold_MIN, G_threshold_MIN, B_threshold_MIN), cv::Scalar(R_threshold_MAX, G_threshold_MAX, B_threshold_MAX), RGBthreshold);
			SHOWIMAGE(RGBthreshold);
			break;
		case 'd' :
			imshow("Draw Image", drawimage); waitKey(1);
			break;
		case 'o' :
		bitwise_or(HSVthreshold, RGBthreshold, ANDthreshold);
		SHOWIMAGE(ANDthreshold);
break;
		}

		// blur the image to get rid of the noise. This will output an intensity image
	/*	cv::blur(grayImage, blurImage, BLUR_SIZE);
		cv::Mat erodeElement = getStructuringElement(MORPH_ELLIPSE, cv::Size(3, 3));
		// dilate with larger element so make sure object is nicely visible
		cv::Mat dilateElement = getStructuringElement(MORPH_ELLIPSE, cv::Size(8, 8));
		cv::erode(HSVthreshold, HSVthreshold, erodeElement);
		cv::erode(HSVthreshold, HSVthreshold, erodeElement);
		cv::dilate(HSVthreshold, HSVthreshold, dilateElement);
		cv::dilate(HSVthreshold, HSVthreshold, dilateElement);
*/
		//	SHOWIMAGE(BGthreshold);
		//bitwise_and(HSVthreshold, BGthreshold, );
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

	sprintf(trackbarname, "ab");
	sprintf(trackbarname, "min_dist");
	sprintf(trackbarname, "param_1");
	sprintf(trackbarname, "param_2");
	sprintf(trackbarname, "min_radius");
	sprintf(trackbarname, "max_radius");

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



