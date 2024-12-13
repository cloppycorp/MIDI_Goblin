#ifndef N_ARPEGGIATOR_H
#define N_ARPEGGIATOR_H
#include <MIDI.h>
#include "b_definitions.h"
#include "c_appVariables.h"
#include "d_selectionVariables.h"
#include "h_deviceVars.h"
#include <Arduino.h>
#include <algorithm> // for std::sort
#include "j_displayFunctions.h"
void transposeArp(byte arpTransState);
void updateArpTranspArrays();
void drawArpeggiatorOptions();
void stopArp();
void playArp();
void startArp();
void shuffleArpArray(byte arr[], size_t size);
void sortArpArray(byte arpModeVal);
bool compareArpValuesDescending(byte a, byte b);
bool compareArpValuesAscending(byte a, byte b);
void arpeggiator(unsigned long currentTime,byte status,byte note, byte velocity);
void updateArpArray(byte status,byte note,byte velocity);
void printArpArray();

#endif // N_ARPEGGIATOR_H
