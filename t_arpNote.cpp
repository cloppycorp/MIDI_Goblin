#include "t_arpNote.h"

void calculateArpNoteChords(byte note, byte velocity){
        arpNoteRootNote = note; 
        //if the direction is ->
 if (arpNoteDirection){
  //if an on note is received while the app is set to major key
for(byte i = 0; i < 8; i++){
  arpNoteMajorChordArray[i] = arpNoteRootNote + majorChordPattern[i];   
  arpNoteMinorChordArray[i] = arpNoteRootNote + minorChordPattern[i];   
}

 
} 
      //if direction <-
 if (!arpNoteDirection){
  //if an on note is received while the app is set to major key

for(byte i = 0; i < 8; i++){
  arpNoteMajorChordArray[i] = arpNoteRootNote - majorChordPattern[i];   
  arpNoteMinorChordArray[i] = arpNoteRootNote - minorChordPattern[i];   
  
}

 }
if(arpNoteKey){
memcpy(arpNotePlayingChordArray, arpNoteMajorChordArray, sizeof(arpNoteMajorChordArray));
}
if(!arpNoteKey){
memcpy(arpNotePlayingChordArray, arpNoteMinorChordArray, sizeof(arpNoteMinorChordArray));  
}

}

void cancelArpNote(){ 
  arpNoteStartTime =0;
  memset(arpNoteMinorChordArray, 0, sizeof(arpNoteMinorChordArray));
  memset(arpNoteMajorChordArray, 0, sizeof(arpNoteMajorChordArray));

  firstArpNoteNotePlaying = false;
  arpNoteRootNote = 0;
}

void arpNote(unsigned long currentTime, int status, byte note, byte velocity){
  if (status == 0x90 && !firstArpNoteNotePlaying && arpNoteRootNote == 0){
  firstArpNoteNotePlaying = true;
  arpNoteNotePlaying = note;
  calculateArpNoteChords(note, velocity);
  arpNoteStartTime = currentTime + arpNoteTempo;
  
  }
  
  if (status == 0x80 && note == arpNoteRootNote && !arpNoteSequencePlaying){
  cancelArpNote();
  }

  if (status == 0x90 && arpNoteSequencePlaying && note == arpNoteRootNote){
    MIDI.sendNoteOff(arpNoteNotePlaying, 0, arpNoteOutChannel);
    firstArpNoteNotePlaying = true;
    arpNoteSequencePlaying = false;  
    arpNoteNotePlaying = note;
  calculateArpNoteChords(note, velocity);
  arpNoteStartTime = currentTime + arpNoteTempo;
  }
  
  if(status == 0x90 && arpNoteSequencePlaying && note != arpNoteRootNote ){
    MIDI.sendNoteOff(arpNoteNotePlaying, 0, arpNoteOutChannel);
    firstArpNoteNotePlaying = true;
    arpNoteSequencePlaying = false;  
    arpNoteNotePlaying = note;
  calculateArpNoteChords(note, velocity);
  arpNoteStartTime = currentTime + arpNoteTempo;
  }

  
}
