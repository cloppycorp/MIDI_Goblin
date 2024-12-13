#ifndef O_SEQUENCER_H
#define O_SEQUENCER_H
#include <MIDI.h>
#include "b_definitions.h"
#include "c_appVariables.h"
#include "d_selectionVariables.h"
#include "h_deviceVars.h"
#include "k_checkAppStatuses.h"
#include "x_midiClock.h"
#include "q_vectors.h"
#include <Arduino.h>
unsigned long calculateNextSeqTime(unsigned long sequencerTempo, int8_t seqSwangValue);

void encNoteEdit(byte selectedArrayPosition,byte parameterMax, byte parameterMin,bool seqPlaying);
extern byte encNoteTempNote;
extern bool chordResetPressed;
extern bool encNotePlaying;
extern byte tempChord[8];
void drawSeqPlayBox(byte seqPlayPosition);
void sequencer(unsigned long currentTime,byte status,byte note,byte velocity);
extern bool seqChordPlaying;
extern byte playingSeqChordPosition;
void startSeq();
void playSeq();



#endif // O_SEQUENCER_H
