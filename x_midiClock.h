#ifndef X_MIDICLOCK_H
#define X_MIDICLOCK_H
#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h> //when creating default instance we need to use "Adafruit_SH1106G"
#include "h_deviceVars.h"
#include "b_definitions.h"
#include "c_appVariables.h"


void drawClockSourceScreen();
unsigned long bpmToMilliseconds(int bpm);
extern unsigned long midiClock0;
extern unsigned long midiClock24;
extern unsigned long bpm;            // Stores the averaged BPM
extern unsigned int clockCount;      // Counter to track which clock message we're on (0-23)
extern unsigned long previousBpm;
extern unsigned long midiClockBeatLength;
extern unsigned long bpmValues[4]; // Array to hold previous BPM values
extern int bpmIndex;    
extern bool midiClockStarted;
extern byte clockSource;
extern byte clockSourceMin;
extern byte clockSourceMax;
extern unsigned long arpBpm;
extern unsigned long arpNoteBpm;
extern unsigned long seqBpm;
void calculateMidiClockBpm(unsigned long midiClock0, unsigned long midiClock23);
void handleMidiClock(unsigned long timeReceived);
extern char clockSourceNames[2][7];
#endif // X_MIDICLOCK_H