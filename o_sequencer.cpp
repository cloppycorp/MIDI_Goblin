#include "o_sequencer.h"
bool chordResetPressed = false;
byte tempChord[8] = {255,255,255,255,255,255,255,255};
bool seqChordPlaying = false;
byte playingSeqChordPosition = 255;
bool encNotePlaying = false;
byte encNoteTempNote = 50;
unsigned long calculateNextSeqTime(unsigned long sequencerTempo, int8_t seqSwangValue) {
    // Calculate the adjustment factor (swing factor between 0.5 and 1.5)
    float adjustmentFactor = 1.0 + (seqSwangValue / 100.0);

    // Return the adjusted sequencerTempo
    return sequencerTempo * adjustmentFactor;
}

void encNoteEdit(byte selectedArrayPosition,byte parameterMax, byte parameterMin,bool seqPlaying){
    switch (encState) {
    case increased:
    if(!seqPlaying){
      MIDI.sendNoteOff(sequencerArray[selectedArrayPosition].seqStepNotes[0],0,seqOutChannel);
    }
    increaseParameterValue(&sequencerArray[selection].seqStepNotes[0],&parameterMax);
    
      DS = false;
    if(!seqPlaying){

      MIDI.sendNoteOn(sequencerArray[selectedArrayPosition].seqStepNotes[0],seqVel,seqOutChannel);
      encNotePlaying = true;
    }
      return;
      break;
    case decreased:
    if(!seqPlaying){
      MIDI.sendNoteOff(sequencerArray[selectedArrayPosition].seqStepNotes[0],0,seqOutChannel);
    }
    decreaseParameterValue(&sequencerArray[selection].seqStepNotes[0],&parameterMin);
    
      DS = false;
    if(!seqPlaying){
      MIDI.sendNoteOn(sequencerArray[selectedArrayPosition].seqStepNotes[0],seqVel,seqOutChannel);
      encNotePlaying = true;
    }
      return;
      break;


  }  
}


void drawSeqPlayBox(byte seqPlayPosition){

  if (seqPlayPosition == selection){
   display.drawRect(seqGrid[seqPlayPosition].x,seqGrid[seqPlayPosition].y, SEQ_BOX_WIDTH, SEQ_BOX_HEIGHT , SH110X_BLACK); 
  }else{
  display.drawRect(seqGrid[seqPlayPosition].x,seqGrid[seqPlayPosition].y, SEQ_BOX_WIDTH, SEQ_BOX_HEIGHT - 1, SH110X_WHITE);
  }

}






void sequencer(unsigned long currentTime,byte status,byte note,byte velocity){
  sequencerUpdated = false;
  if ((selection < seqLength) && (seqRecordMode == 0 || seqRecordMode == 2) && (status == midi::NoteOn)){
    memset(sequencerArray[selection].seqStepNotes, 255, sizeof(sequencerArray[selection].seqStepNotes));
    sequencerArray[selection].numNotes = 1;
    sequencerArray[selection].seqStepNotes[0] = note;
    
    if(selection < seqLength-1){
    selection++;
    }
    sequencerUpdated = true;
    return;
    }
  }


void startSeq(){
  if (seqAppStatus && !seqStarted){
    unsigned long currentTime = millis();
    seqPlayPosition = 0;
    //if the first step isnt empty
    if(sequencerArray[seqPlayPosition].numNotes != 0){
      
      //if the first step is a singluar note play the note
      if(sequencerArray[seqPlayPosition].numNotes == 1){
        MIDI.sendNoteOn(sequencerArray[seqPlayPosition].seqStepNotes[0],seqVel,seqOutChannel);
        seqNotePlaying = sequencerArray[seqPlayPosition].seqStepNotes[0];
        seqChordPlaying = false;
        playingSeqChordPosition = 255;
      }

      //if the first step is a chord
      if(sequencerArray[seqPlayPosition].numNotes == 2){
      for(byte i = 0; i < 8; i++){
        if (sequencerArray[seqPlayPosition].seqStepNotes[i] == 255){
        break;
        }else if(sequencerArray[seqPlayPosition].seqStepNotes[i] != 255){
          MIDI.sendNoteOn(sequencerArray[seqPlayPosition].seqStepNotes[i],seqVel,seqOutChannel);
        }
      }
      playingSeqChordPosition = seqPlayPosition;
      seqChordPlaying = true;
      }


    if(clockSource == 0 || midiClockBeatLength == 0){
    nextSeqTime = currentTime + sequencerTempo;
    }
    if(clockSource == 1 && midiClockBeatLength != 0){
    nextSeqTime = currentTime + midiClockBeatLength;
    }

    
  }
    seqStarted = true;
    seqAppStatusCheck = false;
    DS = false;
}
}


void playSeq(){

if (seqAppStatus && seqStarted){
unsigned long currentTime = millis();
    if(currentTime > nextSeqTime){
     if(!seqTie && !seqRingout){
      if(!seqChordPlaying && seqNotePlaying != 255){
      MIDI.sendNoteOff(seqNotePlaying,0,seqOutChannel);
      seqNotePlaying = 255;
      }
      if(seqChordPlaying){
        for (byte i = 0; i< 8;i++){
       if(sequencerArray[playingSeqChordPosition].seqStepNotes[i] != 255){
       MIDI.sendNoteOff(sequencerArray[playingSeqChordPosition].seqStepNotes[i],0,seqOutChannel);   
       }
       }
       seqChordPlaying = false;
       seqNotePlaying = 255;
       playingSeqChordPosition = 255;  
       }
        }
     if (seqPlayPosition == seqLength - 1){
      seqPlayPosition = 0;
    }else if(seqPlayPosition < seqLength - 1){
      seqPlayPosition++;
    }
    //mungus
    delay(1);
    //if the step isnt empty
    if (sequencerArray[seqPlayPosition].numNotes != 0){
      //if the step is a single note
      if(sequencerArray[seqPlayPosition].numNotes == 1){
        MIDI.sendNoteOn(sequencerArray[seqPlayPosition].seqStepNotes[0],seqVel,seqOutChannel);
        seqNotePlaying = sequencerArray[seqPlayPosition].seqStepNotes[0];
      }
      if(sequencerArray[seqPlayPosition].numNotes == 2){
        for(byte i = 0; i < 8; i++){
          if (sequencerArray[seqPlayPosition].seqStepNotes[i] != 255){
            MIDI.sendNoteOn(sequencerArray[seqPlayPosition].seqStepNotes[i],seqVel,seqOutChannel);
          }else{
            break;
          }
        }
        seqNotePlaying = 255;
        seqChordPlaying = true;
        playingSeqChordPosition = seqPlayPosition;        
      }
    }

    switch(clockSource){
    case 0://goblin
    if (seqSwang[seqPlayPosition] == 0 ){
         nextSeqTime = currentTime + sequencerTempo;
      }
    if (seqSwang[seqPlayPosition] != 0){
         nextSeqTime = currentTime + calculateNextSeqTime(sequencerTempo, seqSwang[seqPlayPosition]);
      }

    break;
    case 1://midi clock
      nextSeqTime = currentTime + midiClockBeatLength;

    break;
    }


    if(currentScreen == SEQUENCER){
    DS = false;
    }
    }
  }
}
