#include "MIDIFunctions.h"

RtMidiOut *midiout = 0;
bool playA = false;
bool playB = false;

/*cleans all the messages from the MIDI channels*/
void clean() {
	vector<unsigned char> message;
	message.push_back(128);
	message.push_back(64);
	message.push_back(0);
	for (int i = 0; i < 16; i++) {
		message[1] = i;
		for (int j = 0; j < 128; j++) {
			message[2] = j;
			midiout->sendMessage(&message);
		}
	}
}

/*starts connection with the MIDI Loopback and cleans the channels*/
void connectMIDI() {
	// RtMidiOut constructor
	try {
		midiout = new RtMidiOut();
	}
	catch (RtMidiError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	// Check outputs.
	//unsigned int nPorts = midiout->getPortCount();
	//cout << "\nThere are " << nPorts << " MIDI output ports available.\n";
	//string portName;
	//cout << "Opening port " << midiout->getPortName(1) << "\n";
	midiout->openPort(1);
	cout << "Opened port " << midiout->getPortName(1) << "\n";
	clean();
}

void crossfader(string deck, int value) {
	vector<unsigned char> message;
	message.push_back(178);
	message.push_back(0);
	message.push_back(value);

	midiout->sendMessage(&message);
}

/*sets volume = value for deck*/
void volume(string deck, int value) {
	vector<unsigned char> message;
	if (deck.compare("A") == 0) {	
		message.push_back(176);
	}
	else if (deck.compare("B") == 0) {
		message.push_back(177);
	}

	message.push_back(0);
	message.push_back(value);
	midiout->sendMessage(&message);

	/*stop pushing play message
		vector<unsigned char> messageoff;
		messageoff.push_back(128);
		messageoff.push_back(0);
		messageoff.push_back(MIN_VALUE);

		//Sleep(0.5 * 1000);
		//midiout->sendMessage(&messageoff);
		//playB = !playB;*/
}

void play(string deck) {
	vector<unsigned char> message;
	if (deck.compare("A") == 0) {
		message.push_back(144);
	}
	else {
		message.push_back(145);
	}
	message.push_back(0);
	message.push_back(MAX_VALUE);

	midiout->sendMessage(&message);
}

/*obsolete play*/
/*void play(string deck) {
	if (deck.compare("A") == 0) {
		if (playA == true) {
			cout << "pause" << endl;
			//Push play message
			vector<unsigned char> message;
			message.push_back(144);
			message.push_back(0);
			message.push_back(MAX_VALUE);

			midiout->sendMessage(&message);
			playA = false;
		}
		else {
			cout << "play" << endl;
			//Push play message
			vector<unsigned char> message;
			message.push_back(144);
			message.push_back(0);
			message.push_back(MAX_VALUE);

			midiout->sendMessage(&message);
			playA = true;
		}
	}else {
		if (playB == true) {
			cout << "pause" << endl;
			//Push play message
			vector<unsigned char> message;
			message.push_back(145);
			message.push_back(0);
			message.push_back(MAX_VALUE);

			midiout->sendMessage(&message);
			playB = false;
		}
		else {
			cout << "play" << endl;
			//Push play message
			vector<unsigned char> message;
			message.push_back(145);
			message.push_back(0);
			message.push_back(MAX_VALUE);

			midiout->sendMessage(&message);
			playB = true;
		}
	}
}*/

//vector<unsigned char> message;
//vector<unsigned char> messageOff;
//// Control Change: 176, 7, 0 (volume)
//int x = 0;
//int y = 0;
//message.push_back(144);
//message.push_back(0);
//message.push_back(MAX_VALUE);
//messageOff.push_back(128);
//messageOff.push_back(0);
//messageOff.push_back(MIN_VALUE);
/*while (x != -1) {
	//cin >> y;
	cin >> x;
	//message[1] = y;
	message[2] = x;
	midiout->sendMessage(&message);
	Sleep(0.5*1000);
	midiout->sendMessage(&messageOff);
}*/