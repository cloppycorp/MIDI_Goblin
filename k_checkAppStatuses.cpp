#include "k_checkAppStatuses.h"

bool autoChordAppStatusCheck = false;
bool arpNoteAppStatusCheck = false;
bool arpAppStatusCheck = false;
bool seqAppStatusCheck = false;


void checkAppStatuses(){
  checkArpAppStatus();
  checkLfoAppStatus();
  checkAutoChordAppStatus();
  checkArpNoteAppStatus();
  checkSeqAppStatus();
}



void turnOffSeqApp(){
 
    for (byte note = 0; note < 128; note++) {  // Loop through notes 0-127
      MIDI.sendNoteOff(note, 0, seqOutChannel);  // Send Note Off message
    }
  

  

  seqChordPlaying = false;
  playingSeqChordPosition = 255;
  seqNotePlaying = 255;
  seqStarted = false;
  seqPlayPosition = 0;
  seqAppStatusCheck = true;
}

void checkSeqAppStatus(){
  if(!seqAppStatusCheck){
    if(!seqAppStatus && seqStarted){
      turnOffSeqApp();
      return;
    }
    
  }
}

void turnOffArpApp(){
  if(arpeggiatiorNotePlaying != 255){
    MIDI.sendNoteOff(arpeggiatiorNotePlaying,127,arpOutChannel);
    arpeggiatiorNotePlaying = 255;
    memset(arpArray, 255, sizeof(arpArray));  
    memset(arpTranspPlus, 255, sizeof(arpTranspPlus));
    memset(arpTranspMinus, 255, sizeof(arpTranspMinus));
     arpAppStatusCheck = false;
     arpPlayPosition = 0;
     arpNotesHeld = 0;
     arpStarted = false;
     arpeggiatorStartTime = 0;
     arpOverFlow = false;
  }
}

void checkArpAppStatus(){
  if (!arpAppStatusCheck){
    if (!arpAppStatus && arpStarted){
      turnOffArpApp();
      return;
    }
  }
}

void checkLfoAppStatus(){
   if (!lfoAppStatusCheck){
    //additional check for notestarted is required because lfo app will be off most of the time and you 
    //dont want to keep calling stopLfo if its already off. 
    if(!lfoAppStatus && lfoStarted){
      stopLfo();
      return;
    }
   }
}

void turnOffArpNoteApp(){
    if (arpNoteNotePlaying != 0){
    MIDI.sendNoteOff(arpNoteNotePlaying, 0,arpNoteOutChannel);
  }
 stopArpNote();
 arpNoteAppStatus = false; 
}
void checkArpNoteAppStatus(){
    if (!arpNoteAppStatusCheck){
    
    if (!arpNoteAppStatus){
     turnOffArpNoteApp();
     arpNoteAppStatusCheck = true;
     return; 
    }
  if (arpNoteAppStatus){
    arpNoteAppStatusCheck = false;
    return;
  }
  }
}

void checkAutoChordAppStatus(){
 
  if (!autoChordAppStatusCheck){
    
    if (!autoChordAppStatus){
     turnOffAutoChordApp();
     autoChordAppStatusCheck = true;
     return; 
    }
  if (autoChordAppStatus){
    autoChordAppStatusCheck = false;
    return;
  }
  }
  
}

void turnOffAutoChordApp(){
  //send off notes for the major chord array
       for (byte i =0; i < autoChordLength; i++){
        MIDI.sendNoteOff(autoChordPlaying[i],0,autoChordOutChannel);
          }
        memset(autoChordPlaying, 0, sizeof(autoChordPlaying));  
        autoChordRootNote = 0;
        autoChordHeld = false;
        autoChordAppStatus = false;
        
       // Serial.flush();  // Flush outgoing (transmit) buffer
       
    }


void updateAutoChordApp(){
  //if a change is made to autochord while the app is on and a chord is being held, we need to turn off the 
  //current chord being played, then turn on the chord with the new values
    if(autoChordHeld){
              for (byte i =0; i < autoChordLength -1 ; i++){
          MIDI.sendNoteOff( autoChordPlaying[i],0 ,autoChordOutChannel);
              }

}
}
void updateApp(){
  if (autoChordAppStatus){
    updateAutoChordApp();
  }
}
