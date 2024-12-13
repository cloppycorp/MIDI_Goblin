#include "l_arpNote.h"

void startArpNoteSequence() {
    unsigned long currentTime = millis();
  if (arpNoteAppStatus && firstArpNoteNotePlaying) {
 
    if (currentTime >= arpNoteStartTime) {  
      //MIDI.sendNoteOff(arpNoteRootNote, 0, arpNoteOutChannel);
      arpNoteSequencePlaying = true;
      firstArpNoteNotePlaying = false;
      arpNoteSequenceNotePosition = 0;
      //MIDI.sendNoteOn(arpNotePlayingChordArray[arpNoteSequenceNotePosition], 127, arpNoteOutChannel);
      //arpNoteNotePlaying = arpNotePlayingChordArray[arpNoteSequenceNotePosition];
      arpNoteNextSequenceTime = currentTime;
    }
  }
}

void stopArpNote(){
  MIDI.sendNoteOff(arpNoteNotePlaying, 0, arpNoteOutChannel);
  arpNoteNextSequenceTime = 0;
  memset(arpNoteMajorChordArray, 0, sizeof(arpNoteMajorChordArray));
  memset(arpNoteMajorChordArray, 0, sizeof(arpNoteMajorChordArray));
  memset(arpNoteMinorChordArray, 0, sizeof(arpNoteMinorChordArray));
  firstArpNoteNotePlaying = false;
  arpNoteRootNote = 0; 
  arpNoteNotePlaying = 0;
  arpNoteSequencePlaying = false;
  arpNoteSequenceNotePosition = 0;

 
}

void playArpNoteSequence() {
     unsigned long currentTime = millis();
  if (arpNoteSequencePlaying) {
   


    if (currentTime >= arpNoteNextSequenceTime) {
      
  if (arpNoteSequenceNotePosition == arpNoteLength){
    stopArpNote();
    return;
  }
        MIDI.sendNoteOff(arpNoteNotePlaying, 0, arpNoteOutChannel);
        arpNoteSequenceNotePosition++;
        MIDI.sendNoteOn(arpNotePlayingChordArray[arpNoteSequenceNotePosition], 127, arpNoteOutChannel);
        arpNoteNotePlaying = arpNotePlayingChordArray[arpNoteSequenceNotePosition];
        arpNoteNextSequenceTime = currentTime + arpNoteTempo;

        return;

   
  }
}
}
