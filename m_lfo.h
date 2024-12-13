#ifndef M_LFO_H
#define M_LFO_H
#include <MIDI.h>
#include "b_definitions.h"
#include "c_appVariables.h"
#include "d_selectionVariables.h"
#include "h_deviceVars.h"


#include <Arduino.h>
unsigned long calculateIncrementTime();
void playLfo();
void calculateLfoValueDepth(int &minValue, int &maxValue);
extern bool lfoAppStatusCheck;
extern bool lfoStarted;
void startLfo();
void stopLfo();
#endif // M_LFO_H
