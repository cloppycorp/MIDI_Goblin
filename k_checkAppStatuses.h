#ifndef K_CHECKAPPSTATUSES_H
#define K_CHECKAPPSTATUSES_H
#include <Arduino.h>
#include "a_libraries.h"
#include "b_definitions.h"
#include "f_midiVars.h"
#include "c_appVariables.h"
#include "l_arpNote.h"
#include "m_lfo.h"
#include "n_arpeggiator.h"
#include "o_sequencer.h"
extern bool arpAppStatusCheck;
extern bool arpNoteAppStatusCheck;
extern bool autoChordAppStatusCheck;
extern bool seqAppStatusCheck;



void checkArpAppStatus();
void checkLfoAppStatus();
void checkArpNoteAppStatus();
void checkAutoChordAppStatus();
void checkSeqAppStatus();

void checkAppStatuses();


void turnOffAutoChordApp();
void turnOffArpNoteApp();
void turnOffArpApp();
void turnOffSeqApp();
void playArpNoteSequence();


void updateAutoChordApp();
void updateApp();
#endif // K_CHECKAPPSTATUSES_H
