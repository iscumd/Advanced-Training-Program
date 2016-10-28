#include <iostream> // for standard I/O
#include <string>   // for strings
#include <iomanip>  // for controlling float print precision
#include <sstream>  // string to number conversion
#include <opencv2/core/core.hpp>        // Basic OpenCV structures (cv::Mat, Scalar)
#include <opencv2/imgproc/imgproc.hpp>  // Gaussian Blur
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>  // OpenCV window I/O
using namespace std;
using namespace cv;
#define TEST

void IMSHOW(string name, Mat mat)
{
#ifdef TEST 
	imshow(name, mat);
#endif
}
//#define RPICAM


void createTrackbars();
void tresh_update();

void on_trackbar1(int, void*)
{
	tresh_update();
}; // this function gets called whenever a trackbar position is changed

const int H_MAX = 255;

int H_threshold_MIN = 23; // starting values
int S_threshold_MIN = 72;
int V_threshold_MIN = 98;
int H_threshold_MAX = 169;
int S_threshold_MAX = 255;
int V_threshold_MAX = 215;

bool largestcontour(vector<Point> i, vector<Point> j) { return (i.size()<j.size()); }


const string trackbarwindowname1 = "trackbars";
///////////////////////////////////

double maxValue_max = 255;
double C_max = 200;
int maxValue_slider = 200;
int C_slider = 50;
double maxValue = 200;
double C = 10;

void on_trackbar(int, void*)
{
	maxValue = (double)maxValue_slider;
	C = (double)C_slider;
}
int main(int argc, char *argv[])
{
	//help();
	//sprintf(TrackbarName, "Alpha x %d", maxValue_max);
#ifdef TEST
	namedWindow("Window", 0);
	createTrackbar("Max Value   ", "Window", &maxValue_slider, maxValue_max, on_trackbar);
	createTrackbar("C Value   ", "Window", &C_slider, C_max, on_trackbar);
#endif

	const string sourceReference = "ballwithcones.mp4";// argv[1], sourceCompareWith = argv[2];
	int delay = 15;

	char c;
	int frameNum = -1;          // Frame counter

#ifndef RPICAM
	VideoCapture captRefrnc(sourceReference);// , captUndTst(sourceCompareWith);
#endif	
	if (!captRefrnc.isOpened())
	{
		cout << "Could not open reference " << sourceReference << endl;
		getchar();
		return -1;
	}
#ifdef TEST
	createTrackbars();
#endif

	Size refS = Size((int)captRefrnc.get(CAP_PROP_FRAME_WIDTH),
		(int)captRefrnc.get(CAP_PROP_FRAME_HEIGHT));

#ifdef TEST
	const char* WIN_UT = "Under Test";
	namedWindow(WIN_UT, WINDOW_AUTOSIZE);
	moveWindow(WIN_UT, refS.width, 0);         //1500, 2
#endif
	cout << "Reference frame resolution: Width=" << refS.width << "  Height=" << refS.height << endl;
	
	for (;;) //Show the image captured in the window and repeat
	{
		Mat rgbPic, grayPic, resizePic, hsvPic, HSVthreshold;
		captRefrnc.read(rgbPic);
		if (rgbPic.empty() == true)// || frameUnderTest.empty())
		{
			cout << rgbPic.rows << " < < <  Game over!  > > > ";
			break;
		}
		++frameNum;
		cout << "Frame: " << frameNum << "# " << endl;


		cv::resize(rgbPic, resizePic, cv::Size(640,480));
		// READ RGB color image and convert it to Lab
		cv::Mat bgr_image;
		resizePic.copyTo(bgr_image);
		//rgbPic.copyTo(bgr_image);
		//IMSHOW("image original", bgr_image);

		cv::Mat lab_image;
		cv::cvtColor(bgr_image, lab_image, CV_BGR2Lab);

		// Extract the L channel
		std::vector<cv::Mat> lab_planes(3);
		cv::split(lab_image, lab_planes);  // now we have the L image in lab_planes[0]

		// apply the CLAHE algorithm to the L channel
		cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
		clahe->setClipLimit(3);
		clahe->setTilesGridSize(cv::Size(20, 20));
		cv::Mat dst;
		clahe->apply(lab_planes[0], dst);

		// Merge the the color planes back into an Lab image
		dst.copyTo(lab_planes[0]);
		cv::merge(lab_planes, lab_image);

		// convert back to RGB
		cv::Mat image_clahe;
		cv::cvtColor(lab_image, image_clahe, CV_Lab2BGR);
		// display the results  (you might also want to see lab_planes[0] before and after).
		//cv::IMSHOW("image CLAHE", image_clahe);


	//	cv::resize(image_clahe, resizePic, cv::Size(640, 480));
		//cv::IMSHOW("resize", resizePic);

		cvtColor(rgbPic, hsvPic, COLOR_BGR2HSV);
		//cvtColor(resizePic, hsvPic, COLOR_BGR2HSV);
	
		// Compute Threshold Image based on threshold values
		cv::inRange(hsvPic, cv::Scalar(H_threshold_MIN, S_threshold_MIN, V_threshold_MIN), cv::Scalar(H_threshold_MAX, S_threshold_MAX, V_threshold_MAX), HSVthreshold);
		//IMSHOW("RGBthreshold", RGBthreshold);
		IMSHOW("HSVthreshold", HSVthreshold);

		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
		findContours(HSVthreshold, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		/// Draw contours
		Mat drawing = Mat::zeros(HSVthreshold.size(), CV_8UC3);
		RNG rng(12345);
		std::sort(contours.begin(), contours.end(), largestcontour);
		//for (int i = 0; i < contours.size(); i++)
		//{
		//	Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		//	drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		//}
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
	//	drawContours(drawing, contours, contours.size() - 1 , color, 2, 8, hierarchy, 0, Point());

		if (contours.size() == 0)
			break;
		cv::Moments moment = moments(contours[contours.size() - 1], false);
		double area = moment.m00;
		cv::Point2f center;
		float radius;
		cv::minEnclosingCircle(contours[contours.size() - 1], center, radius);
		circle(drawing, center, radius, color);
		/// Show in a window
		IMSHOW("Contours", drawing);

		cv::cvtColor(rgbPic, grayPic, cv::COLOR_BGR2GRAY);
		Mat adaptPic;
		const int blocksize = 21;
		cv::adaptiveThreshold(grayPic, adaptPic, maxValue, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, blocksize, C);
		//IMSHOW("AdaptPic",adaptPic);

	//	IMSHOW(WIN_RF, frameReference);
		//IMSHOW(WIN_UT, frameUnderTest);
		c = (char)waitKey(delay);
		if (c == 27) break;
	}
	getchar();
	return 0;
}


//	char key = 0;
//	int testkey = 0;
//	// main loop
//	while (key != 'q') { // press 'q' to quit
//
//		// keyboard interface
//		testkey = cv::waitKey(10);  // returns -1 if no key was pressed
//		if (testkey != -1)
//			key = testkey;

void tresh_update()
{
	H_threshold_MIN  = cv::getTrackbarPos("H_MIN", trackbarwindowname1);
	H_threshold_MAX = cv::getTrackbarPos("H_MAX", trackbarwindowname1);
	S_threshold_MIN = cv::getTrackbarPos("S_MIN", trackbarwindowname1);
	S_threshold_MAX = cv::getTrackbarPos("S_MAX", trackbarwindowname1);
	V_threshold_MIN = cv::getTrackbarPos("V_MIN", trackbarwindowname1);
	V_threshold_MAX = cv::getTrackbarPos("V_MAX", trackbarwindowname1);
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

}