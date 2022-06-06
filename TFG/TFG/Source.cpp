#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;

int iLowH = 0;
int iHighH = 15;

int iLowS = 105;
int iHighS = 255;

int iLowV = 154;
int iHighV = 255;

/**
 * Get the number of camera available
 */
int countCameras()
{
	VideoCapture temp_camera;
	int maxTested = 10;
	for (int i = 0; i < maxTested; i++) {
		VideoCapture temp_camera(i);
		bool res = (!temp_camera.isOpened());
		temp_camera.release();
		if (res)
		{
			return i;
		}
	}
	return maxTested;
}

void createControl() {
	namedWindow("Control", WINDOW_AUTOSIZE); //create a window called "Control"

	//Create trackbars in "Control" window
	createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
	createTrackbar("HighH", "Control", &iHighH, 179);

	createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
	createTrackbar("HighS", "Control", &iHighS, 255);

	createTrackbar("LowV", "Control", &iLowV, 255); //Value (0 - 255)
	createTrackbar("HighV", "Control", &iHighV, 255);
}

//int main(int argc, char** argv)
//{
//	VideoCapture cap(0); //capture the video from web cam
//
//	if (!cap.isOpened())  // if not success, exit program
//	{
//		cout << "Cannot open the web cam" << endl;
//		return -1;
//	}
//
//	createControl();
//
//	while (true)
//	{
//		Mat imgOriginal;
//
//		bool bSuccess = cap.read(imgOriginal); // read a new frame from video
//
//		if (!bSuccess || imgOriginal.empty()) //if not success, break loop
//		{
//			cout << "Cannot read a frame from video stream" << endl;
//			break;
//		}
//
//		Mat imgHSV;
//
//		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
//
//		Mat imgThresholded;
//
//		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image
//
//	   //morphological opening (remove small objects from the foreground)
//		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		//morphological closing (fill small holes in the foreground)
//		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
//
//		/* 
//		/  DETECT CENTER OF SINGLE BLOB
//		*/
//		/*
//		// find moments of the image
//		Moments m = moments(imgThresholded, true);
//		Point p(m.m10 / m.m00, m.m01 / m.m00);
//
//		// coordinates of centroid
//		cout << Mat(p) << endl;
//
//		// show the image with a point mark at the centroid
//		circle(imgOriginal, p, 5, Scalar(0, 128, 0), -1);
//		*/
//
//		/*
//		/  DETECT CENTER OF MULTIPLE BLOB
//		*/
//		Mat canny_output;
//		vector<vector<Point> > contours;
//		vector<Vec4i> hierarchy;
//
//		// detect edges using canny
//		Canny(imgThresholded, canny_output, 50, 150, 3);
//
//		// find contours
//		findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
//
//		// get the moments
//		vector<Moments> mu(contours.size());
//		for (int i = 0; i < contours.size(); i++)
//		{
//			mu[i] = moments(contours[i], false);
//		}
//
//		// get the centroid of figures.
//		vector<Point2f> mc(contours.size());
//		for (int i = 0; i < contours.size(); i++)
//		{
//			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
//		}
//
//
//		// draw contours
//		Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
//		for (int i = 0; i < contours.size(); i++)
//		{
//			Scalar color = Scalar(0, 128, 0); // B G R values
//			drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
//			circle(drawing, mc[i], 4, color, -1, 8, 0);
//			circle(imgOriginal, mc[i], 4, color, -1, 8, 0);
//			cout << mc[i] << endl;
//		}
//
//
//		//Image windows
//		//imshow("Thresholded Image", imgThresholded); //show the thresholded image
//		imshow("Contours", drawing); //show the contour image
//		imshow("Original", imgOriginal); //show the original image
//
//		if (waitKey(30) == 27) //wait for 'esc' key press for 30ms. If 'esc' key is pressed, break loop
//		{
//			cout << "esc key is pressed by user" << endl;
//			break;
//		}
//	}
//
//	return 0;
//}