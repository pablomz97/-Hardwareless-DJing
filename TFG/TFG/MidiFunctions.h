#ifndef MIDIFUNCT_H
#define MIDIFUNCT_H

#include "RtMidi.h"

using namespace std;

//Midi-Values
#define MAX_VALUE 127
#define MIN_VALUE 0
#define MID_VALUE 64

extern RtMidiOut *midiout ;

void clean();
void connectMIDI();
void crossfader(string deck, int value);
void volume(string deck, int value);
void play(string deck);

#endif