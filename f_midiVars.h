#ifndef F_MIDIVARS_H
#define F_MIDIVARS_H
#include <Arduino.h>
#include "g_displayedVars.h"
//midi variables
extern byte midiOutChannel; // define midi out channel
extern byte noteReceived; // the midi note received
extern byte octaveShift; // determines output note played when one is received
extern const char* harmonyNoteName; // used to display harmony note letter
extern const char* harmonyOctave; // used to display harmony note octave
extern const char* noteName; // used to display received note letter
extern const char* octave; // used to display received note octave
//monitor
extern bool justReceivedNrpn;
extern bool justReceivedCc;
extern bool justReceivedNote;
extern byte displayedIncomingMidiChannel;
const char* byteToNote(byte midiNote);

#endif // F_MIDIVARS_H
