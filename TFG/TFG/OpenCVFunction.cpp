#include "OpenCVFunction.h"

VideoCapture cap;

int state = 0;
Point actual = Point(0,0);

//Screen Values
int maxX = 640;
int maxY = 480;
int midX = 320;
int midY = 240;

//HSV values
int iLowH = 31;
int iHighH = 66;

int iLowS = 66;
int iHighS = 255;

int iLowV = 95;
int iHighV = 255;

int largest_area = 0;
int largest_contour_index = 0;

//----------------------------- OPENCV FUNCTIONS -------------------------------
void openCamera() {
	cap = VideoCapture(0); //capture the video from web cam

	if (!cap.isOpened())  // if not success, exit program
	{
		cout << "Cannot open the web cam" << endl;
	}
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

	colorCalibration("Control");
}

int scale(int position, int min, int max) {
	if (position < min) { return 127; }
	else if (position > max) { return 0; }
	position = position - min;
	return 127 - (position * 127 / (max - min));
}

int getY(int position) {
	if (position > 400) { return 0; }
	int minY = 240;
	int maxY = 400;
	position = position - minY;
	return 127 - (position * 127 / (maxY-minY));
}

void colorCalibration(string winName) {
	while (true) {
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess || imgOriginal.empty()) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			return;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

		//morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(3, 3)));

		imshow("Thresholded Image", imgThresholded); //show the thresholded image
		imshow("Original Image", imgOriginal);

		if (waitKey(30) == 13) //wait for 'enter' key press for 30ms. If 'esc' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
	destroyWindow(winName);
	destroyWindow("Thresholded Image");
	destroyWindow("Original Image");

}

void analyzeImage() {
	Mat imgOriginal;

	bool bSuccess = cap.read(imgOriginal); // read a new frame from video

	if (!bSuccess || imgOriginal.empty()) //if not success, break loop
	{
		cout << "Cannot read a frame from video stream" << endl;
		return;
	}

	Mat imgHSV;

	cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

	Mat imgThresholded;

	inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

   //morphological opening (remove small objects from the foreground)
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	//morphological closing (fill small holes in the foreground)
	dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	/*
	/  DETECT CENTER OF MULTIPLE BLOB
	*/
	Mat canny_output;
	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	// detect edges using canny
	Canny(imgThresholded, canny_output, 50, 150, 3);

	// find contours
	findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

	// get the moments
	vector<Moments> mu(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mu[i] = moments(contours[i], false);
	}

	// get the centroid of figures.
	vector<Point2f> mc(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
	}


	// draw contours

	Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
	for (int i = 0; i < contours.size(); i++)
	{
		double area = contourArea(contours[i]);  //  Find the area of contour
		Scalar color = Scalar(0, 0, 128); // B G R values
		//drawContours(drawing, contours, i, color, 2, 8, hierarchy, 0, Point());
		//circle(drawing, mc[i], 4, color, -1, 8, 0);
		if (area > 1000) {
			circle(imgOriginal, mc[i], 4, color, -1, 8, 0);
		}
		//cout << mc[i] << endl;

		if (area > largest_area)
		{
			largest_area = area;
			largest_contour_index = i;               //Store the index of largest contour
		}
		//cout << area << endl;
	}



	largest_area = 0;
	largest_contour_index = 0;

	//Image windows
	//imshow("Thresholded Image", imgThresholded); //show the thresholded image
	//imshow("Contours", drawing); //show the contour image
	//rectangle(imgOriginal, Point(400,300), Point(500,380), Scalar(0, 0, 128), 2);
	//imshow("Original", imgOriginal); //show the original image
}

void gesture_loop() {
	QuadTree tree = QuadTree(Dimension(0, 0, 640, 480));
	while (true) {
		//activation of button
		if (state == 1) { //SLIDERS
			tree.searchFader(actual.x, actual.y);
			while (state == 1) {
				tree.searchButton(actual.x, actual.y);
				Sleep(100);
			}
		}
		else if(state == 2){
			tree.searchButton(actual.x, actual.y);
			while (state == 2) {
				Sleep(100);
			}
		}
		else {
			Sleep(100);
		}
	}
}

void image_loop() {
	while (true) {
		Mat imgOriginal;

		bool bSuccess = cap.read(imgOriginal); // read a new frame from video

		if (!bSuccess || imgOriginal.empty()) //if not success, break loop
		{
			cout << "Cannot read a frame from video stream" << endl;
			return;
		}

		Mat imgHSV;

		cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

		Mat imgThresholded;

		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgThresholded); //Threshold the image

	   //morphological opening (remove small objects from the foreground)
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//morphological closing (fill small holes in the foreground)
		dilate(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		/*
		/  DETECT CENTER OF MULTIPLE BLOB
		*/
		Mat canny_output;
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		// detect edges using canny
		Canny(imgThresholded, canny_output, 50, 150, 3);

		// find contours
		findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));

		// get the moments
		vector<Moments> mu(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mu[i] = moments(contours[i], false);
		}

		// get the centroid of figures.
		vector<Point2f> mc(contours.size());
		for (int i = 0; i < contours.size(); i++)
		{
			mc[i] = Point2f(mu[i].m10 / mu[i].m00, mu[i].m01 / mu[i].m00);
		}

		// draw contours
		Mat drawing(canny_output.size(), CV_8UC3, Scalar(255, 255, 255));
		double max_area = 0;
		for (int i = 0; i < contours.size(); i++)
		{
			double area = contourArea(contours[i]);  //  Find the area of contour
			Scalar color = Scalar(255, 255, 255); // B G R values
			if (area > 500) {
				max_area = area;
				circle(imgOriginal, mc[i], 4, color, -1, 8, 0);
			}
		}

		//TODO: Assertion failed -> end program when there is no contour
		if(!mc.empty() && max_area > 500) {
			cout << "X: " << mc[largest_contour_index].x << " , Y: " << mc[largest_contour_index].y << endl;
			float x = mc[largest_contour_index].x;
			float y = mc[largest_contour_index].y;

			actual = Point(x, y);
		} else { actual = Point(-1, -1); }

		//Image windows

		Mat imgG1 = imgOriginal.clone();
		Mat imgG2 = imgOriginal.clone();

		//gesture1 - faders
		rectangle(imgG1, Point(200, 0), Point(440, 60), Scalar(0, 0, 128), 2);
		rectangle(imgG1, Point(200, 60), Point(440, 120), Scalar(0, 0, 128), 2);
		rectangle(imgG1, Point(200, 120), Point(440, 180), Scalar(0, 0, 128), 2);
		rectangle(imgG1, Point(200, 180), Point(440, 240), Scalar(0, 0, 128), 2);

		line(imgG1, Point(320, 0), Point(320, 420), Scalar(0, 0, 128), 2);

		//volume + crossfader
		rectangle(imgG1, Point(250, 240), Point(390, 480), Scalar(0, 0, 128), 2);
		line(imgG1, Point(250, 420), Point(390, 420), Scalar(0, 0, 128), 2);

		//gesture2 - buttons
		circle(imgG2, Point(125, 120), 100, Scalar(0, 128, 0), 2);
		circle(imgG2, Point(525, 120), 100, Scalar(0, 128, 0), 2);

		rectangle(imgG2, Point(0, 280), Point(250, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(62.5, 280), Point(62.5, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(125, 280), Point(125, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(187.5, 280), Point(187.5, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(0, 380), Point(250, 380), Scalar(0, 128, 0), 2);

		rectangle(imgG2, Point(390, 280), Point(640, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(452.5, 280), Point(452.5, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(515, 280), Point(515, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(577.5, 280), Point(577.5, 480), Scalar(0, 128, 0), 2);
		line(imgG2, Point(390, 380), Point(640, 380), Scalar(0, 128, 0), 2);
		

		imshow("Gesture 1", imgG1); //show the original image
		imshow("Gesture 2", imgG2); //show the original image
		//imshow("Threshold", imgThresholded); //show the original image

		if (waitKey(30) == 13) //wait for 'enter' key press for 30ms. If 'enter' key is pressed, break loop
		{
			cout << "esc key is pressed by user" << endl;
			break;
		}
	}
}