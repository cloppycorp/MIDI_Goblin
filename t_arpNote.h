#ifndef T_ARPNOTE_H
#define T_ARPNOTE_H
#include <MIDI.h>
#include "c_appVariables.h"
#include "b_definitions.h"

void calculateArpNoteChords(byte note, byte velocity);
void cancelArpNote();
void arpNote(unsigned long currentTime, int status, byte note, byte velocity);
#endif // T_ARPNOTE_H
