#include "n_arpeggiator.h"

void transposeArp(byte arpTransState){
switch(arpTransState){
  //minus
  case 0:
    for(byte i = 0; i < 8; i++){
      if (arpTranspMinus[i] == 254){
        arpArray[i] = 255;
      }else{
        arpArray[i] = arpTranspMinus[i];
      }
    }
  break;

  //plus
  case 2:
      for(byte i = 0; i<8; i++){
        if(arpTranspPlus[i] == 254){
          arpArray[i] = 255;
        }else{
          arpArray[i] = arpTranspPlus[i];
        }
      }
  break;
}
updateArpTranspArrays();
}

void drawArpeggiatorOptions(){
  display.setTextColor(SH110X_WHITE);
  defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
  defineListGridColumn(listGridColumnB, 90, 10); // Example x coordinate
  if(arpLatch && arpStarted){
        drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, "TRANSPOSE <--/-->", false);
        }
  if(!arpLatch || !arpStarted){
          drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, "LATCH ARP 2 TRANSPOSE", false);          
      }

  display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
  display.print("TRANSPOSE AMT: ");
  display.setCursor(listGridColumnB[1].x,listGridColumnB[1].y);
  display.print(arpTranspAmt);

  display.setCursor(listGridColumnA[2].x,listGridColumnA[2].y);
  display.print("SEQUENCER SYNC: ");
  display.setCursor(listGridColumnB[2].x,listGridColumnB[2].y);
  display.print(arpStepSync ? "ON" : "OFF");

  display.setCursor(listGridColumnA[3].x,listGridColumnA[3].y);
  display.print("ARP OUT CH 1: ");
  display.setCursor(listGridColumnB[3].x,listGridColumnB[3].y);
  display.print(arpOutChannel);

  display.setCursor(listGridColumnA[4].x,listGridColumnA[4].y);
  display.print("VELOCITY: ");
  display.setCursor(listGridColumnB[4].x,listGridColumnB[4].y);
  display.print(arpVel);  

}

void startArp(){
  if (arpAppStatus && !arpStarted && arpeggiatiorNotePlaying != 255){
  unsigned long currentTime = millis();
  if (currentTime > arpeggiatorStartTime && !arpStepSync || arpStepSync && seqStarted && currentTime > nextSeqTime){
    arpPlayPosition = 0;
    arpStarted = true;
    nextArpeggiatorTime = currentTime;
    return;
  }
}
}

void playArp(){
  if(arpStarted){

    unsigned long currentTime = millis();
    if (currentTime > nextArpeggiatorTime){
      if(arpTransState != 1){
        transposeArp(arpTransState);
        arpTransState = 1;
          //arpPlayPosition = 0;
        }
        
      MIDI.sendNoteOff(arpeggiatiorNotePlaying,0,arpOutChannel);
      if(arpPlayPosition < ARP_ARRAY_SIZE - 1 && arpArray[arpPlayPosition+1] != 255 && arpPlayPosition+1 < arpLengthMax){
      arpPlayPosition++;
      MIDI.sendNoteOn(arpArray[arpPlayPosition],arpVel,arpOutChannel);
      arpeggiatiorNotePlaying = arpArray[arpPlayPosition];
      
      
      if(arpStepSync && seqStarted){
        nextArpeggiatorTime = nextSeqTime + 1;
      
      }else{
      nextArpeggiatorTime = currentTime + arpeggiatorTempo;
      }
      return;
    }
    if(arpArray[arpPlayPosition+1] == 255 || arpPlayPosition + 1 > ARP_ARRAY_SIZE - 1 || arpPlayPosition + 1 >= arpLengthMax){
      
      arpPlayPosition = 0;
      sortArpArray(arpModeVal);
      MIDI.sendNoteOn(arpArray[arpPlayPosition],arpVel,arpOutChannel);
      arpeggiatiorNotePlaying = arpArray[arpPlayPosition];
      if(arpStepSync && seqStarted){
      nextArpeggiatorTime = nextSeqTime + 1;
      
      }else{
      nextArpeggiatorTime = currentTime + arpeggiatorTempo;  
      }
      return;
    }
      }

  //nothing here yet but first note is playing

  }
}


void stopArp(){
    MIDI.sendNoteOff(arpeggiatiorNotePlaying,127,arpOutChannel);
    arpeggiatiorNotePlaying = 255;
    arpeggiatorStartTime = 0;
    arpPlayPosition = 0;
    arpStarted = false;
    arpNotesHeld = 0;
    if(currentScreen == ARPEGGIATOROPTIONS){
      DS = false;
    }
}
void shuffleArpArray(byte arr[], size_t size) {
    // Separate the non-255 values
    std::vector<byte> non255Values;
    for (size_t i = 0; i < size; ++i) {
        if (arr[i] != 255) {
            non255Values.push_back(arr[i]);
        }
    }

    // Shuffle the non-255 values
    for (size_t i = 0; i < non255Values.size(); ++i) {
        size_t j = i + random(non255Values.size() - i);
        std::swap(non255Values[i], non255Values[j]);
    }

    // Combine shuffled non-255 values with 255s at the end
    size_t index = 0;
    for (size_t i = 0; i < non255Values.size(); ++i) {
        arr[index++] = non255Values[i];
    }
    while (index < size) {
        arr[index++] = 255;
    }
}


void sortArpArray(byte arpModeVal){
      // Sort the array after updating
    switch(arpModeVal){
      case 0:
      std::sort(arpArray, arpArray + ARP_ARRAY_SIZE, compareArpValuesAscending);
      return;
      break;
      case 1:
      std::sort(arpArray, arpArray + ARP_ARRAY_SIZE, compareArpValuesDescending);
      return;
      break;
      case 2:
      if (arpDirectionPlaying){
      std::sort(arpArray, arpArray + ARP_ARRAY_SIZE, compareArpValuesDescending);
      arpDirectionPlaying = false;
      return;
      }
      if(!arpDirectionPlaying){
      std::sort(arpArray, arpArray + ARP_ARRAY_SIZE, compareArpValuesAscending);
      arpDirectionPlaying = true;  
      return;
      }
      break;
      case 3:
      shuffleArpArray(arpArray, ARP_ARRAY_SIZE);
      return;
      break;
    }  
    updateArpTranspArrays();
}




bool compareArpValuesDescending(byte a, byte b) {
    if (a == 255) return false; // 255 should be at the end
    if (b == 255) return true;  // 255 should be at the end
    return a > b; // Sort in descending order
}

bool compareArpValuesAscending(byte a, byte b) {
    if (a == 255) return false; // 255 should be at the end
    if (b == 255) return true;  // 255 should be at the end
    return a < b; // Sort in ascending order
}

void printArpArray(){
//for troubleshooting
for (byte i = 0; i< sizeof(arpArray); i++){
  if (i < ARP_ARRAY_SIZE - 1){
    Serial.print(arpArray[i]);
    Serial.print(",");
  }else if (i == ARP_ARRAY_SIZE-1){
    Serial.println(arpArray[i]);
  }
}
}

void addToArpArray(unsigned long currentTime,byte status, byte note,byte velocity){
  if(arpArray[0] == 255){
    arpArray[0] = note;
    arpeggiatorStartTime = currentTime + arpeggiatorTempo;
    arpeggiatiorNotePlaying = note;
    arpNotesHeld++;
    arpStarted = false;
    updateArpTranspArrays();
    return;
  }else if(arpArray[0] != 255){
    MIDI.sendNoteOff(note,0,arpOutChannel);
  for(byte i = 0; i < ARP_ARRAY_SIZE;i++){
    if(arpArray[i] == 255){
      arpArray[i] = note;
      arpNotesHeld++;
      arpArrayUpdated = true;
      updateArpTranspArrays();
     return;
    }
  }
  }
}

void updateArpTranspArrays(){

  memcpy(arpTranspMinus, arpArray, sizeof(arpArray));
  memcpy(arpTranspPlus, arpArray, sizeof(arpArray));

  for(byte i = 0; i<8; i++){
    byte tmpPlusByte = arpTranspPlus[i];
    byte tmpMinusByte = arpTranspMinus[i];

    if(tmpPlusByte == 255){
    arpTranspPlus[i] = 255;  
    }else if ((tmpPlusByte + arpTranspAmt) <= 127){
      arpTranspPlus[i] = tmpPlusByte + arpTranspAmt;
    } else {
      arpTranspPlus[i] = 254;
    }
    
    if(tmpMinusByte == 255){
      arpTranspMinus[i] = 255;
    }else if(tmpMinusByte - arpTranspAmt >= 0){
      arpTranspMinus[i] = tmpMinusByte - arpTranspAmt;
    }else{
      arpTranspMinus[i] = 254;
    }
    

  }

  
minArpTransp = doesArrayContainNumber(arpTranspMinus, 254);
maxArpTransp = doesArrayContainNumber(arpTranspPlus, 254); 
}


void removeFromArpArray( byte note,byte velocity){
  for(byte i = 0; i < ARP_ARRAY_SIZE;i++){
    if ( arpArray[i] == note){
      arpArray[i] = 255;
      arpNotesHeld--;
      arpArrayUpdated = true;
      updateArpTranspArrays();
      break;
    }
  }
}
void arpeggiator(unsigned long currentTime,byte status, byte note,byte velocity){
if (status == midi::NoteOn && arpNotesHeld <= arpLengthMax){
  bool noteInArray = false;
  for(byte i = 0; i < ARP_ARRAY_SIZE; i++){
    if (arpArray[i] == note){
      noteInArray = true;
      break;
    }
  }
 if(!noteInArray && arpNotesHeld <= arpLengthMax - 1){
 addToArpArray(currentTime, status, note, velocity);
 }

 if(noteInArray){
      removeFromArpArray(note,velocity);
    }
}

if (!arpLatch){
if (status == midi::NoteOff && arpNotesHeld > 0 ){
   removeFromArpArray(note, velocity);
}
}
if (arpArrayUpdated){
  sortArpArray(arpModeVal);
  updateArpTranspArrays();
  if(arpArray[arpPlayPosition] == 255 && arpPlayPosition > 0){
    arpPlayPosition--;
  }
  arpArrayUpdated = false;
}

if (arpArray[0] == 255){
  stopArp();
}

//if the arpeggio if holding the maximum number of notes, and midiNote thru is enabled, 
//and midinotethru channel != arpNote secondary channel
//transmit the note on the secondary arpeggiator channel
if ((status == midi::NoteOn || status == midi::NoteOff) 
   && arpNotesHeld == arpLengthMax 
   && arpOutChannel != midiNoteThruOutChannel){

  if (status == midi::NoteOn){
    MIDI.sendNoteOn(note,velocity,arpOutChannel);
    arpOverFlow = true;
  }
  if (status == midi::NoteOff){
    MIDI.sendNoteOff(note,0,arpOutChannel);
    arpOverFlow = false;
  }
}

}
