//the goal of this sketch is to simply combine all the midi goblin bites into one sketch and have them all run at the same time.
//this works - need to figure out handlenavigation and handlevalueedits

#include "a_libraries.h"
#include "b_definitions.h"
#include "c_appVariables.h"
#include "d_selectionVariables.h"
#include "e_encoderAndButtonVars.h"
#include "f_midiVars.h"
#include "g_displayedVars.h"
#include "h_deviceVars.h"
#include "i_initializers.h"
#include "j_displayFunctions.h"
#include "k_checkAppStatuses.h"
#include "l_arpNote.h"
#include "m_lfo.h"
#include "n_arpeggiator.h"
#include "o_sequencer.h"
#include "p_handleNav.h"
#include "q_vectors.h"
#include "r_fileFunctions.h"
#include "s_handleValEdits.h"
#include "t_arpNote.h"
#include "u_controller.h"
#include "v_addcontrollerparam.h"
#include "w_morph.h"
#include "x_midiClock.h"




//handle encoder and buttons
void handleInput() {
unsigned long currentTime = millis();  
bool debounceExpired = (currentTime - lastBtnPressTime) > btndebounce;

  handleBtns();
  handleEncoder();
  handleUserButtons();
  if (buttonPressed != 255 && debounceExpired || encState != unchanged) { // only allow one button press at a time
    handleNavigation();
    if (!navBtnPressed){// allows for the selection on the screen to be changed
    handleValueEdits(); // allows for variables on the screen to be changed
    encState = unchanged;
    previousEncValue = encValue;
    }
    navBtnPressed = false;
  }


  memcpy(previousBtnStates, currentBtnStates, 16);
}

void handleEncoder() {
  currentEncStates[0] = digitalRead(ENC_A);
  currentEncStates[1] = digitalRead(ENC_B);
  delayMicroseconds(5);
  if (currentEncStates[0] != encStates[0] || currentEncStates[1] != encStates[1]) {
    updateEncoderStates();
    checkEncPattern();
    if (encValue != previousEncValue) {
      DS = false;
      previousEncValue = encValue;
    }
  }
}


void verifyVariables() {
  Serial.println("Verifying Device Configuration Variables:");
  Serial.println("-------------------------------------------------");
  Serial.println("Name          | CC Num | MSB Num | LSB Num | MSB Value | LSB Value | Panel MSB | Panel LSB");

  for (int i = 0; i < numEntries; i++) {
    Serial.print(String(deviceCcNames[i]));
    for (int j = 15 - String(deviceCcNames[i]).length(); j > 0; j--) // Adjust spacing for names
      Serial.print(" ");

    Serial.print("| ");
    Serial.print(String(deviceCcNums[i]) + "       ");
    Serial.print("| ");
    Serial.print(String(deviceMsbNums[i]) + "      ");
    Serial.print("| ");
    Serial.print(String(deviceLsbNums[i]) + "      ");
    Serial.print("| ");
    Serial.print(String(deviceMsbValues[i]) + "       ");
    Serial.print("| ");
    Serial.print(String(deviceLsbValues[i]) + "       ");
    Serial.print("| ");
    Serial.print(String(panelMsbValues[i]) + "        ");
    Serial.print("| ");
    Serial.println(String(panelLsbValues[i]));
  }

  Serial.println("-------------------------------------------------");
  Serial.println("Verification complete.");
}


void autoChord(int status, byte note, byte velocity){
  //if an on note is received while the app is set to major key
if (status == 0x90 && velocity > 0 && autoChordKey && !autoChordHeld){
        autoChordRootNote = note; 
        for(byte i = 0; i < 7; i++){

  autoChordMajorChordArray[i] = autoChordRootNote + majorChordPattern[i];   
}
        

        memcpy(autoChordPlaying, autoChordMajorChordArray, sizeof(autoChordMajorChordArray));
      //transmit the chord based on the length defined by the user
          for (byte i =0; i < autoChordLength -1 ; i++){
          MIDI.sendNoteOn( autoChordPlaying[i],autoChordVelocity ,autoChordOutChannel);
          }
          autoChordHeld = true;
    }
//if an on note is received while the app is set to minor key
if (status == 0x90 && velocity > 0 && !autoChordKey && !autoChordHeld){
        autoChordRootNote = note; 
        for(byte i = 0; i < 7; i++){

  autoChordMinorChordArray[i] = autoChordRootNote + minorChordPattern[i];   
}

        memcpy(autoChordPlaying, autoChordMinorChordArray, sizeof(autoChordMinorChordArray));
        //transmit the chord based on the length defined by the user
          for (byte i =0; i < autoChordLength -1; i++){
          MIDI.sendNoteOn( autoChordPlaying[i],autoChordVelocity ,autoChordOutChannel);
          }
          autoChordHeld = true;
    }

//If an off messege is received for the note that is being held down by the user
if (status == 0x80 && note == autoChordRootNote  && autoChordKey){
    //send off note messeges for each note that is currently playing
    for (byte i =0; i < autoChordLength - 1; i++){
     MIDI.sendNoteOff( autoChordPlaying[i],0,autoChordOutChannel);
        }
    memset(autoChordMajorChordArray, 0, sizeof(autoChordMajorChordArray));    
    memset(autoChordPlaying, 0, sizeof(autoChordPlaying));    
    autoChordRootNote = 0;
    autoChordHeld = false;
}

if (status == 0x80 && note == autoChordRootNote  && !autoChordKey){
   
    for (byte i =0; i < autoChordLength - 1   ; i++){
     MIDI.sendNoteOff( autoChordPlaying[i],0,autoChordOutChannel);
        }
    memset(autoChordMinorChordArray, 0, sizeof(autoChordMinorChordArray));  
    memset(autoChordPlaying, 0, sizeof(autoChordPlaying)); 
    autoChordRootNote = 0;
    autoChordHeld = false;
    }
}




void handleMidiNote(unsigned long currentTime, int status, byte note, byte velocity,byte incomingMidiChannel){
  if (arpAppStatus){
    arpeggiator(currentTime, status, note, velocity);
  }
   if (autoChordAppStatus){
  autoChord(status, note, velocity);
  }
  if (arpNoteAppStatus){
    arpNote(currentTime, status, note,velocity);
  }
  if (currentScreen == SEQUENCER && !seqSubMenuVisible){
    sequencer(currentTime,status,note,velocity);
    if(sequencerUpdated){
    DS = false;
    sequencerUpdated = false;
    }
  }
  if (currentScreen == SEQCHORDENTRY){
      if(status == midi::NoteOn){
      chordEntry(note,selectedArrayEntry);
      }
  }
  if(midiNoteThru){
      if(status == midi::NoteOn){
      MIDI.sendNoteOn(note,velocity,midiNoteThruOutChannel);
      }
      if(status == midi::NoteOff){
      MIDI.sendNoteOff(note,0,midiNoteThruOutChannel);  
      }
  }
     lastMidiNoteNumberReceived = note;
    lastMidiNoteVelocityReceived = velocity;
    lastMidiNoteChannelReceived = incomingMidiChannel;
    if(currentScreen == MONITOR){
      if (status == midi::NoteOn){
      DS = false;
      }
    }
    if(currentScreen == ARPEGGIATOROPTIONS){
      DS = false;
    }
  
}


void chordEntry(byte note, byte selectedArrayEntry){
bool noteExistsInChord = false;
byte noteCount = 0;    
    for (byte i = 0; i < 8; i++){
      if (sequencerArray[selectedArrayEntry].seqStepNotes[i] == note){
        noteExistsInChord = true;
        return;
      }
    }
if(!noteExistsInChord){
  for(byte i = 0; i < 8;i++){
    if(sequencerArray[selectedArrayEntry].seqStepNotes[i] == 255){
      sequencerArray[selectedArrayEntry].seqStepNotes[i] = note;
      break;
    }
  }

  
  //count how many notes are in the chord (if more than 1 its a chord, if ==1 || 0 consider it a note)
  for(byte i = 0; i < 8; i++){
    if (sequencerArray[selectedArrayEntry].seqStepNotes[i] != 255){
      noteCount++;
    }
  }

  if (noteCount == 0){
    sequencerArray[selectedArrayEntry].numNotes = 0;
  }
  if (noteCount == 1){
    sequencerArray[selectedArrayEntry].numNotes = 1;
  }
  if (noteCount > 1){
    sequencerArray[selectedArrayEntry].numNotes = 2;
  }


  
  DS = false;
}
}
  

void sendReMappedCc(byte receivedCc, byte receivedCcValue, byte reMapOutChannel){
  if (midiIndex[receivedCc]->reMapped == true){
    MIDI.sendControlChange(midiIndex[receivedCc]->reMapCc,receivedCcValue,reMapOutChannel);
  }
}

void executeTrig(byte trigIndex){
if(triggerList[trigIndex].status == true){
switch(triggerList[trigIndex].functionNum){
  case 0:
  return;
  break;
  case 1:
  changeBoolState(appStatuses[triggerList[trigIndex].appNum]);
  DS = false;
  return;
  case 2:
  
  sendPatch(triggerList[trigIndex].triggerPatch);
  DS = false;
  return;
  
  break;
  case 3:
  if(seqStarted){
      turnOffSeqApp();    
      loadSequence(triggerList[trigIndex].triggerSequence);
      seqPlayPosition = 0;
      seqStarted = true;
      seqAppStatus = true;
      DS = false;
      }else if (!seqStarted){
        loadSequence(triggerList[trigIndex].triggerSequence);
        DS = false;
      }
  return;
  break;
  case 4:
  panic();
  break;
}
}
else {
  return;
}
}

void checkTrigMatch(byte receivedCc, byte receivedCcValue){
for (byte i = 0; i < triggerList.size();i++){
  if ( receivedCc == triggerList[i].ccNum && receivedCcValue == triggerList[i].ccVal){
    executeTrig(i);
    break;
  }
}
}

void handleNrpn(int status, byte receivedCc, byte receivedCcValue,byte incomingMidiChannel){
  //check to see if the message received is in the correct order. if it is, plop it's values into
  //the receivedNrpn array. If the message is complete - flag it.
        
  receivedNrpnMatch = false;
  switch(receivedCc){
    case 99:
      if (nrpnState == NRPN_IDLE){
        nrpnBufferReset = false;
        nrpnState = NRPN_AWAITING_98;
        nrpnBuffer[0] = receivedCcValue;
        //receivedNrpn[0].MSB = receivedCcValue;
        nrpnStartTime = millis();
        nrpnInOrder = true;
        //serial for troubleshooting
        
      }else{
        nrpnInOrder = false;
        msgJustReceived = MidiCC;
      }
    break;
    case 98:
      if (nrpnState == NRPN_AWAITING_98){
        nrpnBuffer[1] = receivedCcValue;
        nrpnState = NRPN_AWAITING_6;
        //receivedNrpn[0].LSB = receivedCcValue;
        nrpnInOrder = true;
      }else{
        nrpnInOrder = false;
        msgJustReceived = MidiCC;
      }
    break;
    case 6:
      if (nrpnState == NRPN_AWAITING_6){
        nrpnBuffer[2] = receivedCcValue;
        nrpnState = NRPN_AWAITING_38;
        
        //receivedNrpn[0].MSBDAT = receivedCcValue;
        nrpnInOrder = true;
      }else{
        nrpnInOrder = false;
        msgJustReceived = MidiCC;
      }
    break;
    case 38:
      if (nrpnState == NRPN_AWAITING_38){
        nrpnState == NRPN_IDLE;
        nrpnBuffer[3] = receivedCcValue;
        Serial.print(nrpnBuffer[0]);
        Serial.print(" ");
        Serial.print(nrpnBuffer[1]);
        Serial.print(" ");
        Serial.print(nrpnBuffer[2]);
        Serial.print(" ");
        Serial.println(nrpnBuffer[3]);
        //receivedNrpn[0].LSBDAT = receivedCcValue;
        nrpnInOrder = true;
        justReceivedNrpn = true; //flag the message as complete. 
        msgJustReceived = Nrpn;
        
        
      }else{
        nrpnInOrder = false;
        msgJustReceived = MidiCC;
      }
    break;
  }

  //if the msg is received and its not in order, reset the nrpn buffer, handle it as CC
  if (!nrpnInOrder){
    if (!nrpnBufferReset){
      nrpnState = NRPN_IDLE;
      memset(nrpnBuffer, 255, sizeof(nrpnBuffer));
      justReceivedNrpn = false;
      nrpnBufferReset = true;
    }
    //if you are gettings multiple messages in a row for 99, 98, 6 or 3 - its a cc message, not nrpn.
    if (previousCcReceived == receivedCc){
          handleMidiCc(status, receivedCc, receivedCcValue,incomingMidiChannel);
        }
    
    previousCcReceived = receivedCc;
    previousCcValueReceived = receivedCcValue;  
    incomingCcNum = receivedCc;
    incomingCcValue = receivedCcValue;
    return;
  }

  if(nrpnInOrder && justReceivedNrpn){
    
    
    for (byte i = 0; i < numEntries; i++){
      
      if (deviceParams[i].msb == nrpnBuffer[0] && deviceParams[i].lsb == nrpnBuffer[1] ){
            receivedNrpnMatch = true;
            deviceParams[i].msbVal = nrpnBuffer[2];
            deviceParams[i].panelMsbVal = nrpnBuffer[2];
            deviceParams[i].lsbVal = nrpnBuffer[3];
            deviceParams[i].panelLsbVal = nrpnBuffer[3];
            deviceParams[i].obtained = true;
            if(midiCcThru){
            sendNrpn(nrpnBuffer[0],nrpnBuffer[1],nrpnBuffer[2],nrpnBuffer[3]);
            }
            memcpy(previousNrpn, nrpnBuffer, sizeof(nrpnBuffer));
            memset(nrpnBuffer, 255, sizeof(nrpnBuffer));
            nrpnBufferReset = true;
            justReceivedNrpn = false;
            lastNrpnMsgTime = millis();
            lastMidiMessageTime = lastNrpnMsgTime;
            lastMsgReceived = Nrpn;
            nrpnState = NRPN_IDLE;
            if (currentScreen == MONITOR){
              
              DS = false;
            }
            break;
            }
        if(receivedNrpnMatch == false){
          lastMsgReceived = randomNrpn;
          DS = false;
        }
    }
    
    
  }
      
} 
    
    
   









void handleMidiCc(int status, byte receivedCc, byte receivedCcValue,byte incomingMidiChannel){
unsigned long currentTime = millis();

updateCcValues(receivedCc,receivedCcValue,incomingMidiChannel); // this is dumb, if you use pointers instead you dont need it
if (currentScreen == CONTROLLER || currentScreen == MONITOR){
  DS = false;
}



if(currentScreen == REMAP){
    if(receivedCc != receivedDeviceCc){
    receivedDeviceCc = receivedCc;
    DS = false;  
    }
}

if (reMapAppStatus){
  sendReMappedCc(receivedCc,receivedCcValue, reMapOutChannel);
}

checkTrigMatch(receivedCc,receivedCcValue);
if(currentScreen == ADDCONTROLLERPARAM && displayIndex == 0){
  tempCcNum = receivedCc;
  DS = false;
}
if (currentScreen == TRIGGERS && (navX == 1 || navX ==2)){
  triggerList[selectedTriggerIndex].ccNum = receivedCc;
  triggerList[selectedTriggerIndex].ccVal = receivedCcValue;
  DS = false;
}
if(midiCcThru){
  MIDI.sendControlChange(receivedCc, receivedCcValue, midiCcThruOutChannel);
  
}

 if(currentScreen == CONTROLLER && midiIndex[receivedCc] == nullptr && ctrlrX != 0){
  byte tmpCcNum = (**selectedControllerParamNum);
  byte tmpCcVal = (**selectedControllerParamVal);
  MIDI.sendControlChange(tmpCcNum, tmpCcVal, controllerOutChannel);
  midiIndex[tmpCcNum]->morphStart = tmpCcVal;
  if(midiIndex[tmpCcNum]->obtained == false){
    midiIndex[tmpCcNum]->obtained = true;
  }
  midiIndex[tmpCcNum]->morphCalc = tmpCcVal;
  midiIndex[tmpCcNum]->ccVal = receivedCcValue;
  DS = false;
}

lastMsgReceived = msgJustReceived;
previousCcReceived = receivedCc;
previousCcValueReceived = receivedCcValue;
lastMidiMessageTime = millis();

}

void updateCcValues(byte receivedCc, byte receivedCcValue, byte incomingMidiChannel){
  if(midiIndex[receivedCc] != nullptr){
    midiIndex[receivedCc]->ccVal = receivedCcValue;
    midiIndex[receivedCc]->panelCcVal = receivedCcValue;
    midiIndex[receivedCc]->obtained = true;
    midiIndex[receivedCc]->morphStart = receivedCcValue;
    midiIndex[receivedCc]->morphCalc = receivedCcValue;
    //updating deviceCcValues because im not ready to delete it yet...damn arrays
    patchLoaded = false;
    morphed = false;
        
  }
    if(currentScreen == MONITOR){
      if(midiIndex[receivedCc] == nullptr){
        msgJustReceived = randomCc;
      }
      for(byte i = 0; i > 5; i++){
      }
      DS = false;
      displayedIncomingMidiChannel = incomingMidiChannel;
    }
    

}
  

void handleMidi() {
  if (MIDI.read()) {
    //set the current time, create temp variables
    unsigned long currentTime = millis();
    int status = MIDI.getType();
    byte note;
    byte velocity;
    byte receivedCc;
    byte receivedCcValue;
    byte incomingMidiChannel = (status & 0x0F) +1;
    switch(status){
      case midi::Clock:
        if(clockSource == 1){
          handleMidiClock(currentTime);
        return;
        }
      break;

      case midi::NoteOn:
      case midi::NoteOff:
        note = MIDI.getData1();
        velocity = MIDI.getData2();
        msgJustReceived = MidiNote;
          // if a midi note on has been received
        if (status == midi::NoteOn && velocity > 0) {
        handleMidiNote(currentTime,status, note, velocity,incomingMidiChannel);
        return;
        }
        // if a midi note off has been received - some synths use note on + zero velocity
        if (status == midi::NoteOff || (status == midi::NoteOn && velocity == 0)) {
        handleMidiNote(currentTime,status,note, velocity,incomingMidiChannel);
        return;
        }
      break;

      case midi::ControlChange:
      //Serial.println("tst");
        receivedCc = MIDI.getData1();
        receivedCcValue = MIDI.getData2();
        
        //filter out junk messages
        if(receivedCc > 128 || receivedCcValue > 128){
          return;
        }

        //handle midi cc messages that can't possibly be nrpn
        
        if(!deviceHasNrpn || (deviceHasNrpn && (receivedCc != 99 && receivedCc != 98 && receivedCc != 6 && receivedCc != 38 ))){
          //Serial.println("tst");
            msgJustReceived = MidiCC;
            handleMidiCc(status, receivedCc, receivedCcValue,incomingMidiChannel);
          return;
        }

        //handle potential nrpn messages
        if(deviceHasNrpn && (receivedCc == 99 || receivedCc == 98 || receivedCc == 6 || receivedCc == 38)){
        
            if (currentTime - lastMidiMessageTime >= midiDebounceInterval){
              handleMidiCc(status,receivedCc, receivedCcValue,incomingMidiChannel); 
              msgJustReceived = MidiCC;
            }
            if (currentTime - lastMidiMessageTime < midiDebounceInterval){
             
              handleNrpn(status, receivedCc, receivedCcValue,incomingMidiChannel);
            }
        }
          
        
      break;
    
    }
  }
}






void displayMessageScreen(){
  if (currentScreen == MESSAGE){
  unsigned long currentTime = millis();
  
  if (currentTime - messageStartTime > messageTimeout){
   currentScreen = previousScreen;
   screenSwitch();

}
}
}



void drawScreen() {
  display.clearDisplay();
  drawScreenContent();
  drawSelection(selection);
  display.display();
  return;
}

void drawSelectedFile(byte selection){
      if (*selectedListFileCount !=0){
      int lineHeight = 10; // Adjust this based on your text size
      int y = HEADER_AREA_HEIGHT + selection * lineHeight;
      int16_t x1, y1;
      uint16_t w, h;
      display.getTextBounds(displayedListEntries[selection], 1, y + 1, &x1, &y1, &w, &h);
      display.fillRect(x1 - 1, y1 - 1, w + 1, h + 1, SH110X_WHITE);
      display.setCursor(1, y + 1);
      display.setTextColor(SH110X_BLACK);
      display.println(displayedListEntries[selection]);
      }
}
void drawSelectionForApp(){
    if (selection == 0){
      drawWhiteBox(tableColumnB[selection].x,tableColumnB[selection].y, tableColumnB_0);
    }
    if (selection == 1){
      drawWhiteBox(tableColumnB[selection].x,tableColumnB[selection].y, tableColumnB_1);
    }
    if (selection == 2){
      drawWhiteBox(tableColumnB[selection].x,tableColumnB[selection].y, tableColumnB_2);
    }
    if (selection == 3 && currentScreen == REMAP){
      drawBigCell(0,53,"SAVE/LOAD",true);
    }else if (selection == 3){
      drawWhiteBox(tableColumnB[selection].x,tableColumnB[selection].y, tableColumnB_3);
    }
      
}

void drawSelectionForLfo(){
  switch(selection){
        //draw box around wavebox
        case 0:{
          display.drawRect(WAVEBOX_X, WAVEBOX_Y, WAVEBOXWIDTH, WAVEBOXHEIGHT, SH110X_WHITE);
          break;
        }
        case 1:{
          drawWhiteBox(lfoGridPoint[selection].x,lfoGridPoint[selection].y, (lfoAppStatus ? "ON" : "OFF") );
          break;
        }
        case 2:{
          

        drawWhiteBox(lfoGridPoint[selection].x,lfoGridPoint[selection].y, deviceCcNames[deviceLfoIndex] );
        break;
        }
        case 3:{
        char freqChar[3];
        itoa(lfoFreq, freqChar, 10);
        drawWhiteBox(lfoGridPoint[selection].x,lfoGridPoint[selection].y, freqChar );
        break;  
        }
        case 4:{
          char depthChar[3];
          itoa(lfoDepth, depthChar, 10);
          drawWhiteBox(lfoGridPoint[selection].x,lfoGridPoint[selection].y, depthChar );
          break;
        }
  }
}
void drawSelectionForChordEntry(){
  if (navX == 0 && navY == 0){
      drawSmallCell(chordEntryGrid[10].x, chordEntryGrid[10].y, chordEntryGrid[11].x, chordEntryGrid[11].y, 63, 11, "RESET", true);
  }
  if (navX == 1 && navY == 0){
      drawSmallCell(chordEntryGrid[12].x, chordEntryGrid[12].y, chordEntryGrid[13].x, chordEntryGrid[13].y, 62, 11, "CANCEL", true);
  }
  if (navY == 1){
    drawBigCell(chordEntryGrid[14].x, chordEntryGrid[14].y, "CONFIRM", true);
  }
}
void drawSelectionForMorph(){
  switch(navY){
    case 0:
    if (selectedMorphPatch == 999){
    drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, "CHOOSE PATCH", true);
    }
    if (selectedMorphPatch != 999){
    char morphPatchName[21];
    strncpy(morphPatchName, patchList[selectedMorphPatch].data(), 20);
    morphPatchName[20] = '\0';  // Ensure it's null-terminated
    drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, morphPatchName, true);
    }
    break;
    case 1:
    drawBigCell(listGridColumnA[1].x, listGridColumnA[1].y+2, "RESET", true);

    break;
  }
}
void drawSelectionForMidiThru(){
  switch(selection){
    case 0:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, (midiNoteThru ? "ON" : "OFF"));
    break;
    
    case 1:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, (midiCcThru ? "ON" : "OFF"));
    break;

    case 2:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, midiNoteThruOutChannel);
    break;
    case 3:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, midiCcThruOutChannel);
    break;
  }
}
void drawSelectionForArpOptions(){
  switch(selection){
    case 0:
      if(arpLatch && arpStarted){
        drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, "TRANSPOSE <--/-->", true);
        }
  if(!arpLatch || !arpStarted){
          drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, "LATCH ARP 2 TRANSPOSE", true);          
      }
    break;

    case 1:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, arpTranspAmt);
    break;
    case 2:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, (arpStepSync ? "ON" : "OFF"));
    break;
    
    case 3:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, arpOutChannel);
    break;

    case 4:
    drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, arpVel);
    break;
  }
}
void drawSelectionForClockSource(){
  switch(selection){
  case 0:
  drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, clockSourceNames[clockSource]);
  break;

  case 1:
  drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, arpBpm);
  break;

  case 2:
  drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, arpNoteBpm);
  break;

  case 3:
  drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, seqBpm);
  break;
}
}
void drawSelection(byte selection) {
  switch (currentScreen) {
    case DEVICE_SELECT: {
    drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, displayedListEntries[displayIndex]);
        break;
      }
    case MENU: {
    drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, displayedListEntries[displayIndex]);
        break;
      }
    case MONITOR:{
    if(msgJustReceived == MidiCC || msgJustReceived == Nrpn|| msgJustReceived == Nothing){
    drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, deviceMonitorNames[selection] );
    }else{
      return;
    }
    break;
    }
    case PATCHES: {
      if(patchFileCount == 0){
        return;
      }
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
        break;
      }
    case TRIGGERPATCHES:{
      if(patchFileCount == 0){
        return;
      }
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
    }
    break;
    case SEQUENCES: {
      if(sequenceFileCount == 0){
        return;
      }
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
      
        break;
      }
    case REMAPLIST: {
      if(reMapFileCount == 0){
        return;
      }
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
        break;
      }
    case TRACKERLIST: {
      
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
        break;
      }
    case SETTINGS: {
      
      drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, displayedListEntries[displayIndex] );
        break;
      }
    case SAVENEWFILE:{
      drawSmallWhiteBox(newFileNameGrid[selection].x,newFileNameGrid[selection].y, newFileNameArray[selection] );
      break;
    }
    case MIDIOUTCHANNELS:{
      drawWhiteBox(listGridColumnB[selection].x,listGridColumnB[selection].y, displayedMidiOutChannels[selection] );
    }
    break;
    case ARPEGGIATOR:{
    drawSelectionForApp();
    }
    break;
    case ARPEGGIATOROPTIONS:{
    drawSelectionForArpOptions();  
    }
    break;
    case AUTOCHORD:{
    drawSelectionForApp();
    break;
    }
    case ARPNOTE:{
    drawSelectionForApp();
    break;
    }
    case REMAP:{
    drawSelectionForApp();
    break;
    }
    case LFO:{
    drawSelectionForLfo();
    break;
    }
    case SEQUENCER:{
     drawSelectionForSequencer();
    }
    break;
    case USERBUTTONSELECT:{
      drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, userButtonNames[displayIndex] );
    }
    break;
    case USERBUTTONFUNCTIONS:{
      if (!functionSelectionLeftRight){
      if (*selectedListIndex == 2){
        
        if(strcmp(userButtons[*selectedUserBtn].buttonPatch, "NOTHING") == 0){
          drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, functionNames[displayIndex] );
          return;
        }else{
          display.fillRect(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y,100,9,SH110X_BLACK);
          drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y,userButtons[*selectedUserBtn].buttonPatch);
        }
       
      }
      if (*selectedListIndex == 3){
        
        if(strcmp(userButtons[*selectedUserBtn].buttonSequence, "NOTHING") == 0){
          drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, functionNames[displayIndex] );
          return;
        }else{
          display.fillRect(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y,100,9,SH110X_BLACK);
          drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y,userButtons[*selectedUserBtn].buttonSequence);
        }
       
      }
      if(*selectedListIndex != 2 && *selectedListIndex != 3){
      drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, functionNames[displayIndex] );
      return;
      }
      }
      if (functionSelectionLeftRight){
      drawWhiteBox(listGridColumnB[displayIndex].x,listGridColumnB[displayIndex].y, "X" );
      
      return;
      }

    }
    break;
    case USERBUTTONCHOOSEAPP :{
    drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, displayedListEntries[displayIndex] );
    }
    break;
    case USERBUTTONCHOOSEPATCH: {
      if(patchFileCount == 0){
        return;
      }
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
        break;
      }
    case USERBUTTONCHOOSESESEQUENCE:
    if(sequenceFileCount == 0){
      return;
    }
    drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
    break;
    case TRIGGERS:{
      drawSelectionForTriggers();
    }
    break;
    case TRIGGERFUNCTIONS:{
      drawSelectionForTriggerFunctions();
    }
    break;
    case TRIGGERCHOOSEAPP:{
        drawWhiteBox(listGridColumnA[displayIndex].x,listGridColumnA[displayIndex].y, displayedListEntries[displayIndex] );
    }
    break;
    case TRIGGERCHOOSESEQUENCE:
    drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
    break;
    case SEQCHORDENTRY:{
      drawSelectionForChordEntry();
    }
    break;
    case CONTROLLER:{
      drawSelectionForController(ctrlrX,ctrlrY);
    }
    break;
    case ADDCONTROLLERPARAM:{
     drawSelectionForAddControllerParam(); 
    }
    
    break;
    case CONTROLLEROPTIONS:{
      switch(navY){
        case 0:
        drawWhiteBox(listGridColumnB[navY].x,listGridColumnB[navY].y,ctrlrUseDeviceParams ? "YES" : "NO");
        break;
        case 1:
        drawWhiteBox(listGridColumnB[navY].x,listGridColumnB[navY].y,controllerOutChannel);
        break;
      }
    }
    break;
    case MORPH:
    drawSelectionForMorph();
    break;
    case MORPHPATCHES: {
      drawWhiteBox(listGridColumnA[selection].x,listGridColumnA[selection].y, displayedListEntries[selection] );
        break;
      }
    case MIDITHRU:{
      drawSelectionForMidiThru();
    }
    break;
    case CLOCKSOURCE:{
     drawSelectionForClockSource();
    }
    break;

  }
}
void drawSelectionForAddControllerParam(){
  switch(displayIndex){
    default:
    drawSmallWhiteBox(addCcParamGrid[displayIndex].x,addCcParamGrid[displayIndex].y, tempCcName[displayIndex-1] );
    break;
    case 0:
    drawWhiteBox(tableColumnB[displayIndex].x,tableColumnB[displayIndex].y, tempCcNum );
    break;
    case 15:
    drawBigCell(addCcParamGrid[15].x, addCcParamGrid[15].y, "CANCEL", true); 
    break;
    case 16:
    drawBigCell(addCcParamGrid[16].x, addCcParamGrid[16].y, "CONFIRM", true);
    break;
  }
}
void drawSelectionForController(byte ctrlrX, byte ctrlrY){
  if(controllerParamsCount > 0){
  if(*ctrlrPtrs[ctrlrY].index == 255){
    drawBigCell(listGridColumnA[ctrlrY].x, listGridColumnA[ctrlrY].y, "+", true);
    return;
  }
  if(ctrlrX == 1){
    drawSmallCell(listGridColumnC[ctrlrY].x-1, listGridColumnC[ctrlrY].y-1,listGridColumnC[ctrlrY].x, listGridColumnC[ctrlrY].y, 19, 9,*ctrlrPtrs[ctrlrY].ccVal, true);
    return;
  } 
  if (ctrlrX == 0){
    char tempCcName[15];
    strcpy(tempCcName, ctrlrPtrs[ctrlrY].ccName);
    drawWhiteBox(listGridColumnA[ctrlrY].x,listGridColumnA[ctrlrY].y,tempCcName );
    
    return;
  }
  }
  
  if(controllerParamsCount == 0){
    drawBigCell(listGridColumnA[ctrlrY].x, listGridColumnA[ctrlrY].y, "+", true);
    return;
  }
}


void drawSelectionForTriggerFunctions(){

  if (navX == 0 && navY != 2 && navY != 3){
      drawWhiteBox(listGridColumnA[navY].x,listGridColumnA[navY].y, functionNames[navY] );     
      return;    
  }
  if(navX == 0 && navY == 2){
    
    if (strcmp(triggerList[selectedTriggerIndex].triggerPatch, "NOTHING") == 0){
    drawWhiteBox(listGridColumnA[navY].x,listGridColumnA[navY].y, functionNames[navY] );     
      return;
    }else{
      display.fillRect(listGridColumnA[navY].x,listGridColumnA[navY].y,100,9,SH110X_BLACK);
      drawWhiteBox(listGridColumnA[navY].x,listGridColumnA[navY].y, triggerList[selectedTriggerIndex].triggerPatch);
    return;
    }
    
  }
  if(navX == 0 && navY == 3){
    
    if (strcmp(triggerList[selectedTriggerIndex].triggerSequence, "NOTHING") == 0){
    drawWhiteBox(listGridColumnA[navY].x,listGridColumnA[navY].y, functionNames[navY] );     
      return;
    }else{
      display.fillRect(listGridColumnA[navY].x,listGridColumnA[navY].y,100,9,SH110X_BLACK);
      drawWhiteBox(listGridColumnA[navY].x,listGridColumnA[navY].y, triggerList[selectedTriggerIndex].triggerSequence);
    return;
    }
    
  }
  if (navX == 1){
      drawWhiteBox(listGridColumnB[navY].x,listGridColumnB[navY].y, "X" );     
      return;        
  }
}

void drawSelectionForTriggers(){
if(*triggerPtrs[navY].index == 255){
  drawBigCell(listGridColumnA[navY].x-1, listGridColumnA[navY].y, "+", true);
}  
if(*triggerPtrs[navY].index != 255){
  switch(navX){
    case 0:
    char trigName[9];
    strncpy(trigName,triggerList[selectedTriggerIndex].name,sizeof(trigName));
    drawWhiteBox(listGridColumnA[navY].x,listGridColumnA[navY].y, trigName ); 
    break;
    case 1:
    drawWhiteBox(listGridColumnB[navY].x,listGridColumnB[navY].y, *triggerPtrs[navY].ccNum );
    break;
    case 2:
    drawWhiteBox(listGridColumnC[navY].x,listGridColumnC[navY].y, *triggerPtrs[navY].ccVal );
    break;
    case 3:
    drawWhiteBox(listGridColumnD[navY].x,listGridColumnD[navY].y, (*triggerPtrs[navY].status) ?  "ON" : "OFF" );
    break;
  }
}   

}

void drawSelectionForSequencer(){
if (!seqSubMenuVisible){
    if (selection < seqLength){
      
      if (sequencerArray[selection].seqStepNotes[0] != 255 && sequencerArray[selection].numNotes == 1){
    drawWhiteBox(seqGrid[selection].x+2, seqGrid[selection].y+4, byteToNote(sequencerArray[selection].seqStepNotes[0]));
    } else if (sequencerArray[selection].seqStepNotes[0] == 255){
      drawWhiteBox(seqGrid[selection].x+2, seqGrid[selection].y+4, "--");
    }else if (sequencerArray[selection].numNotes == 2){
      drawWhiteBox(seqGrid[selection].x + 4, seqGrid[selection].y + 4, (char)0x13);
    }
    
    }

    if (selection >= seqLength && selection < seqLengthMax){
       display.fillRect(seqGrid[selection].x, seqGrid[selection].y, SEQ_BOX_WIDTH,SEQ_BOX_HEIGHT,SH110X_WHITE);
       display.setTextColor(SH110X_BLACK);
       display.setCursor(seqGrid[selection].x+5, seqGrid[selection].y+4);
       display.print("+");
    } 

}
if(seqSubMenuVisible){
  if (subMenuSelection <=2){
   drawWhiteBox(seqSubMenuGrid[subMenuSelection + 2].x, seqSubMenuGrid[subMenuSelection + 2].y, seqSubMenuLables[subMenuSelection + 2]);
   display.setCursor(seqSubMenuGrid[subMenuSelection + 13].x, seqSubMenuGrid[subMenuSelection + 13].y);
   display.setTextColor(SH110X_WHITE);
   display.write(0x09);
   
  }if (subMenuSelection == 6){ 
  drawBigCell(seqSubMenuGrid[16].x, seqSubMenuGrid[16].y, "SAVE/LOAD", true);
  }
  
  
  else{
    drawWhiteBox(seqSubMenuGrid[subMenuSelection + 2].x, seqSubMenuGrid[subMenuSelection + 2].y, seqSubMenuLables[subMenuSelection + 2]);
    if (subMenuSelection == 3){
     drawWhiteBox(seqSubMenuGrid[8].x,seqSubMenuGrid[8].y, (seqRingout? "Y" : "N"));
  }else if(subMenuSelection == 4){
    drawWhiteBox(seqSubMenuGrid[9].x,seqSubMenuGrid[9].y, (seqTie? "Y" : "N"));
}else if(subMenuSelection == 5){
    drawWhiteBox(seqSubMenuGrid[10].x,seqSubMenuGrid[10].y, seqVel);

}
  }
}
}

void drawAppList(){
  display.setTextColor(SH110X_WHITE);
  
 for (byte i = 0; i<5;i++){
  display.setCursor(listGridColumnA[i].x,listGridColumnA[i].y);
  display.print(visibleAppNames[i]);
 }
}




void drawScreenContent() {
  switch (currentScreen) {
    case DEVICE_SELECT:{
      drawHeader();
      drawColumn(listGridColumnA,1,displayedListEntries,5);
      break;
    }
    case MENU:{
      drawHeader();
      drawColumn(listGridColumnA,1,displayedListEntries,5);
      //displayEncPattern();//for debutting
      break;
    }
    case PATCHES:{
      drawHeader();
      displayFileList();
      break;
    }
    case REMAPLIST:{
     drawHeader();
     displayFileList(); 
    }
    break;
    case DELETE:{
     drawHeader();
     deleteConfirm();
    break;
    }
    case SAVENEWFILE:{
    drawHeader(); 
    fileNameEntry(); 
    //displayEncPattern();
    break;  
    }
    case ARPEGGIATOR:{
    drawHeader(); 
    strcpy(tableColumnA_0, "STATUS");
    strcpy(tableColumnB_0, (*selectedAppStatus ?  "ON" : "OFF"));
    strcpy(tableColumnA_1, "LATCH");
    strcpy(tableColumnB_1, (arpLatch ? "ON" : "OFF"));
    strcpy(tableColumnA_2, "MODE");
    strcpy(tableColumnB_2, arpModes[arpModeVal]);
    strcpy(tableColumnA_3, "MAX SIZE");
    sprintf(tableColumnB_3, "%d",arpLengthMax);
    drawTable(0,HEADER_AREA_HEIGHT + 1,4);
    drawTableContents(
    tableColumnA_0,tableColumnB_0,
    tableColumnA_1,tableColumnB_1,
    tableColumnA_2,tableColumnB_2,
    tableColumnA_3,tableColumnB_3,
    tableColumnA_4,tableColumnB_4,
    4);
    drawTempoBar(0,56,selectedTempo);
    break;
    }
    case REMAP:{
    drawHeader(); 

    drawTable(0,HEADER_AREA_HEIGHT + 1,3);
    strcpy(currentScreenName, "REMAP");
    strcpy(tableColumnA_0, "STATUS");
    strcpy(tableColumnB_0, (*selectedAppStatus ?  "ON" : "OFF"));
    strcpy(tableColumnA_1, "DEVICE CC");
    if (receivedDeviceCc == 255){
      sprintf(tableColumnB_1, "%d",0);    
    }else if(receivedDeviceCc != 255){
    sprintf(tableColumnB_1, "%d",receivedDeviceCc);
    }
    strcpy(tableColumnA_2, "NEW CC");
    sprintf(tableColumnB_2, "%d",reMapNewCcNum);    
    drawTableContents(
    tableColumnA_0,tableColumnB_0,
    tableColumnA_1,tableColumnB_1,
    tableColumnA_2,tableColumnB_2,
    tableColumnA_3,tableColumnB_3,
    tableColumnA_4,tableColumnB_4,
    3);
    if (receivedDeviceCc != 255){
    if (midiIndex[receivedDeviceCc]->reMapped == false){
    drawBigCell(0,41,"YES = set new cc",false);
    }else if(midiIndex[receivedDeviceCc]->reMapped == true){
    drawBigCell(0, 41, midiIndex[receivedDeviceCc]->ccNum," MAPPED TO: ", midiIndex[receivedDeviceCc]->reMapCc, true);
    }

    }
        drawBigCell(0,53,"SAVE/LOAD",false);
      break;
    }
    
    case ARPNOTE:{
    drawHeader(); 
    drawTable(0,HEADER_AREA_HEIGHT + 1,4);
    strcpy(tableColumnA_0, "STATUS");
    strcpy(tableColumnB_0, (*selectedAppStatus ?  "ON" : "OFF"));
    strcpy(tableColumnA_1, "KEY");
    strcpy(tableColumnB_1, (arpNoteKey ? "MAJOR" : "MINOR"));    
    strcpy(tableColumnA_2, "LENGTH");
    sprintf(tableColumnB_2, "%d", arpNoteLength + 1);
    strcpy(tableColumnA_3, "DIRECTION");    
    strcpy(tableColumnB_3, (arpNoteDirection ? "--->" : "<---"));
    drawTableContents(
    tableColumnA_0,tableColumnB_0,
    tableColumnA_1,tableColumnB_1,
    tableColumnA_2,tableColumnB_2,
    tableColumnA_3,tableColumnB_3,
    tableColumnA_4,tableColumnB_4,
    4);
    drawTempoBar(0,56,selectedTempo);
    break;
    }
    case AUTOCHORD:{
    strcpy(tableColumnA_0, "STATUS");
    strcpy(tableColumnB_0, (*selectedAppStatus ?  "ON" : "OFF"));
    strcpy(tableColumnA_1, "KEY");
    strcpy(tableColumnB_1, (autoChordKey ? "MAJOR" : "MINOR"));    
    strcpy(tableColumnA_2, "LENGTH");
    sprintf(tableColumnB_2, "%d", autoChordLength);
    strcpy(tableColumnA_3, "VELOCITY");    
    sprintf(tableColumnB_3, "%d", autoChordVelocity);


    drawHeader();
    drawTable(0,HEADER_AREA_HEIGHT + 1,4);
    drawTableContents(
    tableColumnA_0,tableColumnB_0,
    tableColumnA_1,tableColumnB_1,
    tableColumnA_2,tableColumnB_2,
    tableColumnA_3,tableColumnB_3,
    tableColumnA_4,tableColumnB_4,
    4);    
    break;
    }
    case MONITOR:{
    
    switch(msgJustReceived){
      case Nothing:
      case MidiCC:
      case Nrpn:
      drawDeviceMonitor();
      break;
      case MidiNote:
      drawHeader();
      drawMidiNoteMonitor();
      break;
      case randomNrpn:
      drawNrpnMonitor(nrpnBuffer[0], nrpnBuffer[1], nrpnBuffer[2], nrpnBuffer[3]);
      break;
      case randomCc:
      drawHeader();
      drawRandomCcMonitor(previousCcReceived,previousCcValueReceived,displayedIncomingMidiChannel);
      break;
      //case Nrpn:
      //drawHeader();
      //drawNrpnMonitor();
      //break;

    }
    
    }
    break;
    case LFO:{
      drawHeader();
      drawWaveform();
      drawLfoTaskBar();
    break;
    }
    case SEQUENCER:{
       drawSeqNotes();
       drawSeqLength();
       if(seqAppStatus){ 
       drawSeqPlayBox(seqPlayPosition);
       }
       if (seqSubMenuVisible){
        drawSeqSubMenu();
       }     
       
    }
    break;
    case SEQUENCES:{
      drawHeader();
      displayFileList();
    }
    break;
    case OVERWRITE:{
      drawHeader();
      overwriteConfirm();
      break;
    }
    case MESSAGE:{
      
    }
    break;
    case TRIGGERS:{
      drawHeader();

      drawTriggersScreen();
      //drawColumn(listGridColumnA,0,displayedListEntries,5);
      //drawColumn(listGridColumnB,43,triggerCcNums);
      //drawColumn(listGridColumnC,67,triggerCcVals);
      //drawColumn(listGridColumnD, 102,triggerOnOffStatuses);
    }
    break;
    case SETTINGS:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,5);
      
    }
    break;
    case MIDIOUTCHANNELS:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,5);
      drawColumn(listGridColumnB,80,displayedMidiOutChannels);
    }
    break;
    case USERBUTTONSELECT:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,3);
    }
    break;
    case USERBUTTONFUNCTIONS:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,5);
      drawFunctionSmily(listGridColumnB, 105, *selectedUserBtnFnctn, false);
      
    }
    break;
    case USERBUTTONCHOOSEAPP:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,5);
      drawColumn(listGridColumnB,105,displayedListEntries2,5);
    }
    break;
    case USERBUTTONCHOOSEPATCH:{
      drawHeader();
      displayFileList();
    }
    break;
    case TRIGGERFUNCTIONS:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,5);
      drawSelectedTriggerApp(listGridColumnB, 105, triggerList[selectedTriggerIndex].functionNum, false);
    
    }
    break;
    case TRIGGERPATCHES:{
      drawHeader();
      displayFileList();      
    }
    break;
    case TRIGGERCHOOSEAPP:{
      drawHeader();
      drawColumn(listGridColumnA,0,displayedListEntries,5);
      drawColumn(listGridColumnB,105,displayedListEntries2,5);
    }
    break;
    case SEQCHORDENTRY:{
      drawHeader();
      drawChordEntryScreen(selectedArrayEntry);
      
    }
    break;
    case CONTROLLER:{
      drawHeader();
      drawControllerList();
      //drawControllerTemp();
    }
    break;
    case ADDCONTROLLERPARAM:{
      drawHeader();
      drawAddControllerParam();
    }
    break;
    case CONTROLLEROPTIONS:{
      drawHeader();
      drawControllerOptions();
    }
    break;
    case MORPH:{
      drawHeader();
      drawMorphScreen();
      
    }
    break;
    case MORPHPATCHES:{
    drawHeader();
    displayFileList();
    }
    break;
    case SEQSWANG:{
    drawHeader();
    display.setTextColor(SH110X_WHITE);
    display.setCursor(30,12);
    display.print("SWANGINESS");
    drawSwangBar(2, 24, selectedSeqSwang, seqSwangMin, seqSwangMax);
    }
    break;
    case ARPEGGIATOROPTIONS:{
    drawHeader();
    drawArpeggiatorOptions();
    }
    break;
    case MIDITHRU:{
    drawHeader();
    drawMidiThruScreen();
    }
    break;
    case CLOCKSOURCE:{
      drawHeader();
      drawClockSourceScreen();
    }
    break;
    case SYSTEMINFO:{
      drawHeader();
      drawSystemInfoScreen();
    }
    break;
    case USERBUTTONCHOOSESESEQUENCE:{
      drawHeader();
      displayFileList();
    }
    break;
    case TRIGGERCHOOSESEQUENCE:{
      drawHeader();
      displayFileList();
    }
    break;

  }
}













void displayEncPattern() {
  //used for troubleshooting encoder
  display.setCursor(85, 30);
  display.print(encValue);
  if(currentScreen == SAVENEWFILE){
    display.fillRect(84 , 19 , 40, 40, SH110X_BLACK); 
    display.setTextColor(SH110X_WHITE);
      display.setCursor(85, 20);
  display.print(encValue);
    display.setCursor(85, 30);
    display.print(alphabetIndex);
    display.setCursor(85, 40);  
    display.print(alphabet[alphabetIndex]);
    display.setCursor(85, 50);  
    display.print(newFileNameArray[selection]);
        
  }
}






// Helper function to remove file extension
void removeFileExtension(char* fileName) {
    char* dotPosition = strchr(fileName, '.');
    if (dotPosition != nullptr) {
        *dotPosition = '\0';  // Truncate at the dot
    }
}







void panic() {
  //turns off every possible app
  arpAppStatus = false;
  arpNoteAppStatus = false;
  lfoAppStatus = false;
  reMapAppStatus = false;
  seqAppStatus = false;
  autoChordAppStatus = false;
  //sends every possible note off for every possible channel
  for (byte channel = 1; channel <= 16; channel++) {  // Loop through channels 1-16
    for (byte note = 0; note < 128; note++) {  // Loop through notes 0-127
      MIDI.sendNoteOff(note, 0, channel);  // Send Note Off message
    }
  }
}

void handleUserButtons(){
  switch (buttonPressed){
    case user1BtnPressed:
    executeChosenFunction(0);
    return;
    //executeChosenFunction(button1ChosenFunction,button1AppToToggle,button1ChosenPatch);
    break;
    case user2BtnPressed:
    executeChosenFunction(1);
    return;
    //executeChosenFunction(button2ChosenFunction,button2AppToToggle,button2ChosenPatch);
    break;
    case user3BtnPressed:
    executeChosenFunction(2);
    return;
    //executeChosenFunction(button3ChosenFunction,button3AppToToggle,button3ChosenPatch);
    break;
    }
}

void executeChosenFunction(byte buttonNumber){
  
    switch(userButtons[buttonNumber].buttonFunction){
      case 0:
        return;
      break;

      case 1:
        changeBoolState(appStatuses[userButtons[buttonNumber].buttonApp]);
        DS = false;
        return;
      break;

      case 2:
        if (strcmp(userButtons[buttonNumber].buttonPatch, "NOTHING") == 0){
          return;
        }else{
          String filePath = "/" + String(deviceFolder) + "/PATCHES/" + userButtons[buttonNumber].buttonPatch;
          if (SD.exists(filePath)) {
          sendPatch(userButtons[buttonNumber].buttonPatch);
          DS = false;
          return;
          }else{
            displayMsg("file not found");
            delay(100);
            screenSwitch();
            return;
          }
          
        }
      break;

      case 3:
      if (seqStarted){
      turnOffSeqApp();
      loadSequence(userButtons[buttonNumber].buttonSequence);
      seqPlayPosition = 0;
      seqStarted = true;
      seqAppStatus = true;
      DS = false;
      }else{
        loadSequence(userButtons[buttonNumber].buttonSequence);
        DS = false;
      }
      return;
      break;

      case 4:
       panic();
       DS = false;
       return;
      break;
    }



}


void checkForDisplayUpdate(){
  //checks to see if the screen should be drawn. if DS is false - draw the screen
  if (!DS){
    drawScreen();
    DS = true;
  }
}
void setup() {
  //set baud rate for serial monitor
  Serial.begin(115200);
  //set baud rate for the midi input/output
  MIDI_SERIAL.begin(31250, SERIAL_8N1, 35, 16); // 35 = rx, 16 = tx
  MIDI.begin(MIDI_CHANNEL_OMNI);

  // Start the SPI communication for OLED (VSPI)
  SPI_VSPI.begin(OLED_CLK, -1, OLED_SDA, OLED_CS);
  // Initialize the OLED display
  if (!display.begin(0x3C)) { // Address 0x3C for 128x64
    Serial.println(F("SH1106 allocation failed"));
    while (true); // Stop execution
  } else {
    //displayMsg("Display works!");//display the message on the oled screen
    Serial.println("OLED initialized successfully.");

  }

  // Start the SPI communication for SD card reader (HSPI)
  SPI_HSPI.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS_PIN);

  if (!SD.begin(SD_CS_PIN, SPI_HSPI)) {
    displayMsg("SD card initialization failed!");
    Serial.println("SD card initialization failed!");
  } else {
    Serial.println("SD card initialized successfully.");
  }
  loadAndDisplaySplash();
  // Initialize the multiplexer control pins
  pinMode(MUX_S0, OUTPUT);
  pinMode(MUX_S1, OUTPUT);
  pinMode(MUX_S2, OUTPUT);
  pinMode(MUX_S3, OUTPUT);
  pinMode(MUX_SIG, INPUT_PULLUP); // Assuming buttons are active-low
  pinMode(ENC_A, INPUT);
  pinMode(ENC_B, INPUT);
  initCtrlrDispPtrs();
  //set the states of the encoder pattern array
  currentEncStates[0] = digitalRead(ENC_A);
  currentEncStates[1] = digitalRead(ENC_B);
  defineNewFileNameGrid();
  defineTableColumnA();
  defineTableColumnB();
  defineLfoGrid();
  // Initialize the states array
  encStates[0] = currentEncStates[0];
  encStates[1] = currentEncStates[1];
  valueMinMax(arpLengthMax, 8, arpLengthMaxMin, 1, arpLengthMaxMax, 8);
  valueMinMax(arpNoteLength,arpNoteLength,arpNoteLengthMin,0,arpNoteLengthMax,7);
  populateDevices();
  memset(seqSwang, 0, sizeof(seqSwang));  
  initTriggerList();
  initUserBtns();
  MIDI.turnThruOff();
  initiateSequencerArray();
  currentScreen = DEVICE_SELECT;
  Serial.println(currentScreen);
  screenSwitch();
}



void loop() {

  checkAppStatuses();
  checkForDisplayUpdate();
  handleInput();
  checkForDisplayUpdate();
  handleMidi();
  checkForDisplayUpdate();
  startArpNoteSequence();
  checkForDisplayUpdate();
  playArpNoteSequence();
  checkForDisplayUpdate();
  startLfo();
  checkForDisplayUpdate();
  playLfo();
  checkForDisplayUpdate();
  startArp();
  checkForDisplayUpdate();
  playArp();
  checkForDisplayUpdate();
  startSeq();
  checkForDisplayUpdate();
  playSeq();
  checkForDisplayUpdate();
  displayMessageScreen();
  checkForDisplayUpdate();
  handleMorph(morphPosition);
  checkForDisplayUpdate();
}
