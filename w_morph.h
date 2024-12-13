#ifndef W_MORPH_H
#define W_MORPH_H
#include <Arduino.h>
#include "c_appVariables.h"
#include <MIDI.h>
#include "b_definitions.h"
#include "h_deviceVars.h"
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h> //when creating default instance we need to use "Adafruit_SH1106G"



void handleMorph(int morphPosition);
void calcMorph(byte numEntries, int morphPosition);
void resetMorph();

#endif // W_MORPH_H