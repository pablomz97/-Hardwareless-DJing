#include "ArduinoFunctions.h"
#include "OpenCVFunction.h"

//pointer usage since it allows to set as global variable without constructor
unique_ptr<SerialPort> arduino;

int lastInput = 0;

void connectArduino() {
	const char *port_name = "\\\\.\\COM3";

	arduino.reset(new SerialPort(port_name));
	if (arduino->isConnected()) cout << "Connection Established" << endl;
	else cout << "ERROR, check port name" << endl;
}

void arduinoLoop() {
	//String for incoming data
	char incomingData[MAX_DATA_LENGTH];

	while (arduino->isConnected()) {
		//Check if data has been read or not
		int read_result = arduino->readSerialPort(incomingData, MAX_DATA_LENGTH);
		//cout << (incomingData[0] == '1') << endl;

		//button is not pressed
		if (incomingData[0] == '0' && lastInput != 0) {
			state = 0;
			lastInput = 0;
		}

		//button is pressed
		else if (incomingData[0] == '1' && lastInput == 0) {
			state = 1;
			lastInput = 1;
		}

		//wait a bit
		Sleep(10);
	}
}