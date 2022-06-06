// midiprobe.cpp

//-------------------------------- INCLUDES ------------------------------------
#include <Windows.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>

//files includes
#include "OpenCVFunction.h"
#include "ArduinoFunctions.h"
#include "MidiFunctions.h"

#include "QuadTree.h"

using namespace std;

//----------------------------- GLOBAL VARIABLES -------------------------------


//moves and resizes console
void fixConsole() {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r); //stores the console's current dimensions

	MoveWindow(console, r.left+800, r.top, 350, 300, TRUE); // 800 width, 100 height
}

//----------------------------------- MAIN -------------------------------------
int main()
{
	fixConsole();

	//Create camera object
	openCamera();
	//adjust HSV values
	createControl();

	//starts MIDI connection
	//connectMIDI();

	//Arduino constructor
	//connectArduino();	

	//Arduino serial loop
	//thread arduinoThread(arduinoLoop);
	//thread gestureThread(gesture_loop);
	thread imageThread(image_loop);

	//arduinoThread.join();
	//gestureThread.join();
	imageThread.join();

	// Clean up
	delete midiout;
	return 0;
}

//QuadTree testing
/*
int main() {
	connectMIDI();

	// Set console code page to UTF-8 so console known how to interpret string data
	SetConsoleOutputCP(CP_UTF8);

	// Enable buffering to prevent VS from chopping up UTF-8 byte sequences
	setvbuf(stdout, nullptr, _IOFBF, 1000);

	//write();
	//read();

	QuadTree tree = QuadTree(Dimension(0, 0, 640, 480));

	/*tree.placeFader(Dimension(200, 0, 320, 60), volume, "gainA");
	tree.placeFader(Dimension(320, 0, 440, 60), volume, "gainB");
	tree.placeFader(Dimension(200, 60, 320, 120), volume, "highsA");
	tree.placeFader(Dimension(320, 60, 440, 120), volume, "highsB");
	tree.placeFader(Dimension(200, 120, 320, 180), volume, "midsA");
	tree.placeFader(Dimension(320, 120, 440, 180), volume, "midsB");
	tree.placeFader(Dimension(200, 180, 320, 240), volume, "lowsA");
	tree.placeFader(Dimension(320, 180, 440, 240), volume, "lowsB");

	tree.placeFader(Dimension(250, 240, 320, 420), volume, "volumeA");
	tree.placeFader(Dimension(320, 240, 390, 420), volume, "volumeB");*//*

	tree.placeButton(Dimension(0, 0, 320, 240), clean, "button");
	tree.placeFader(Dimension(0, 0, 160, 120), volume, "fader");

	tree.searchButton(100,100);

	//printBT(&tree);
	tree.printBT();

	int x;
	cin >> x;

	delete midiout;

	return 0;
}*/