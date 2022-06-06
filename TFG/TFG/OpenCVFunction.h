#ifndef OPENCVFUNCT_H
#define OPENCVFUNCT_H

#include <string>
#include <Windows.h>

//opencv includes
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include "MidiFunctions.h"
#include "QuadTree.h"

using namespace cv;
using namespace std;

//Screen Values
extern int maxX, maxY, midX, midY;

//HSV values
extern int iLowH, iHighH, iLowS, iHighS, iLowV, iHighV;

extern int state;

void openCamera();
void createControl();
int getY(int position);
void colorCalibration(string winName);
void analyzeImage();
void gesture_loop();
void image_loop();

#endif
