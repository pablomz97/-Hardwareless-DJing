#ifndef ARDVFUNCT_H
#define ARDVFUNCT_H

#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include "SerialPort.h"

using namespace std;

extern unique_ptr<SerialPort> arduino;

void connectArduino();
void arduinoLoop();

#endif
