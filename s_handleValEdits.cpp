#include "s_handleValEdits.h"
ValueEditConfig midiThruConfig[4] = {
    {&midiNoteThru, nullptr, nullptr, nullptr},
    {&midiCcThru, nullptr, nullptr, nullptr},
    {nullptr, &midiNoteThruOutChannel, &midiChannelMin, &midiChannelMax},
    {nullptr, &midiCcThruOutChannel, &midiChannelMin, &midiChannelMax}
};



ValueEditConfig autoChordConfig[4] = {
    {&autoChordAppStatus, nullptr, nullptr, nullptr},
    {&autoChordKey, nullptr, nullptr, nullptr},
    {nullptr, &autoChordLength, &autoChordLengthMin, &autoChordLengthMax},
    {nullptr, &autoChordVelocity, &autoChordVelocityMin, &autoChordVelocityMax}
};

ValueEditConfig arpSettingsConfig[5] = {
  {nullptr,nullptr,nullptr,nullptr},
  {nullptr,&arpTranspAmt, &midiMin, &midiMax},  
  {&arpStepSync, nullptr, nullptr, nullptr},
  {nullptr,&arpOutChannel, &midiChannelMin, &midiChannelMax},
  {nullptr,&arpVel, &midiMin, &midiMax},  
};

extern byte midiChannelMin;
extern byte midiChannelMax;

ValueEditConfig arpNoteConfig[4] = {
    {&arpNoteAppStatus, nullptr, nullptr, nullptr},
    {&arpNoteKey, nullptr, nullptr, nullptr},
    {nullptr, &arpNoteLength, &arpNoteLengthMin, &arpNoteLengthMax},
    {&arpNoteDirection, nullptr, nullptr, nullptr}
};



ValueEditConfig arpeggiatorConfig[4] = {
    {&arpAppStatus, nullptr, nullptr, nullptr},
    {&arpLatch, nullptr, nullptr, nullptr},
    {nullptr, &arpModeVal, &arpModeMin, &arpModeMax},
    {nullptr, &arpLengthMax, &arpLengthMaxMin, &arpLengthMaxMax}
};

//lfo isnt used...gotta figure that one out. 
ValueEditConfig2 lfoConfig[5] = {
    {nullptr, &lfoWaveForm, &lfoWaveFormMin, &lfoWaveFormMax},
    {&lfoAppStatus, nullptr, nullptr, nullptr},
    {nullptr, &deviceLfoIndex, &lfoDeviceCcMin, &lfoDeviceCcMax},
    {nullptr, &lfoFreq, &lfoFreqMin, &lfoFreqMax},
    {nullptr, &lfoDepth, &lfoDepthMin, &lfoDepthMax}
};

//lfo isnt used...gotta figure that one out. 
ValueEditConfig2 midiOutChannelsConfig[11] = {
    {nullptr, &patchesOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &arpOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &autoChordOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &arpNoteOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &seqOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &lfoOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &reMapOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &controllerOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &morphOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &midiNoteThruOutChannel, &midiOutChannelMin, &midiOutChannelMax},
    {nullptr, &midiCcThruOutChannel, &midiOutChannelMin, &midiOutChannelMax}

};



void handleValueEditsForDelete(){
  switch(buttonPressed){
    case yesBtnPressed:{
      
      deleteFile(*selectedList, selectedFile, selectedPathSuffix, selectedListFileCount); // Pass pointer to file count
      sortVector(selectedList);
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      currentScreen = previousScreen;
      screenSwitch();
      break;
    }
  }
}

void handleValueEditsForSaveNewFile(){
  switch (encState) {
    case increased:
    if (alphabetIndex < sizeof(alphabet)){
      alphabetIndex++;
      newFileNameArrayIndexes[selection] = alphabetIndex;
      newFileNameArray[selection] = alphabet[newFileNameArrayIndexes[selection]];
      DS = false;
      return;
    }
      break;
    case decreased:
    if (alphabetIndex > 0){
      alphabetIndex--;
      newFileNameArrayIndexes[selection] = alphabetIndex;
      newFileNameArray[selection] = alphabet[newFileNameArrayIndexes[selection]];
      DS = false;
      return;
    }
      break;
  }
  switch(buttonPressed){
    case upBtnPressed:{
          if (alphabetIndex < sizeof(alphabet)){
      alphabetIndex++;
      newFileNameArrayIndexes[selection] = alphabetIndex;
      newFileNameArray[selection] = alphabet[newFileNameArrayIndexes[selection]];
      DS = false;
      return;
    }
      break;
    }
    case downBtnPressed:{
    if (alphabetIndex > 0){
      alphabetIndex--;
      newFileNameArrayIndexes[selection] = alphabetIndex;
      newFileNameArray[selection] = alphabet[newFileNameArrayIndexes[selection]];
      DS = false;
      return;
    }      
      break;
    }
  }
}
void handleValueEditsForTriggers(){
byte tempTriggerCount = triggerList.size();
  switch(buttonPressed){
    case deleteBtnPressed: 
    if(triggerList[selectedTriggerIndex].index != 255){
    Serial.println(selectedTriggerIndex);
    triggerList[selectedTriggerIndex].ccNum = 0;
    triggerList[selectedTriggerIndex].ccVal = 0;
    triggerList[selectedTriggerIndex].status = false;
    triggerList[selectedTriggerIndex].functionNum = 0;
    triggerList[selectedTriggerIndex].appNum = 0;
    triggerList[selectedTriggerIndex].patch = 0;
    triggerList[selectedTriggerIndex].sequence = 0;
    triggerList[selectedTriggerIndex].panic = false;
    char tmpWord[25] = {"NOTHING"};

    strncpy(triggerList[selectedTriggerIndex].triggerPatch, tmpWord, 25);
    triggerList[selectedTriggerIndex].triggerPatch[MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null termination
    strncpy(triggerList[selectedTriggerIndex].triggerSequence, tmpWord, 25);
    triggerList[selectedTriggerIndex].triggerSequence[MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null termination
    
    currentScreen = TRIGGERS;
    screenSwitch();
    }
    break;
    case encoderBtnPressed:
    case yesBtnPressed:{
    
    if(*triggerPtrs[navY].index == 255 && tempTriggerCount < maxTriggers){
        insertTriggerEntry();
      currentScreen = TRIGGERS;
      screenSwitch();
      }
    

    
    
    if(navX == 3){
      changeBoolState(triggerPtrs[selectedTriggerIndex].status);
      DS = false;
    }
    }
    break;
    case downBtnPressed:
    if(navX == 1){
    increaseParameterValue(*triggerPtrs[selectedTriggerIndex].ccNum, midiMax);
    DS = false;
    return;
    }
    if(navX == 2){
    increaseParameterValue(*triggerPtrs[selectedTriggerIndex].ccVal, midiMax);
    DS = false;
    return;
    }
    break;
    case upBtnPressed:
    if(navX == 1){
    decreaseParameterValue(*triggerPtrs[selectedTriggerIndex].ccNum, midiMin);
    DS = false;
    return;
    }
    if(navX == 2){
    decreaseParameterValue(*triggerPtrs[selectedTriggerIndex].ccVal, midiMin);
    DS = false;
    return;
    }
    break;
    
  }
  
    
  }
  
void handleValueEditsForSequencer(){
if (!seqSubMenuVisible){
if(noteEdit){
if(seqStarted){
encNoteEdit(selection,midiMax, midiMin,true);  
}
if(!seqStarted){
encNoteEdit(selection,midiMax, midiMin,false);    
}
}
switch(buttonPressed){
  case encoderBtnPressed:{
    if (!noteEdit){
      noteEdit = true;
      navBtnPressed = true;
      tempoBarVisible = false;
      
      sequencerArray[selection].numNotes = 1;
      sequencerArray[selection].seqStepNotes[0] = encNoteTempNote;
      MIDI.sendNoteOn(sequencerArray[selection].seqStepNotes[0],seqVel,seqOutChannel);
      DS = false;
      return;
    }
    if (noteEdit){
      finishNoteEdit();
      return;
    }
  }
  break;
  case yesBtnPressed:{
    if(selection < seqLength){
      if(noteEdit){
        finishNoteEdit();
      }
      changeBoolState(selectedAppStatus);
      DS = false;
      return;
    }
    if (selection >= seqLength){
      seqLength = selection + 1;
      DS = false;
      return;
    }
  }
  break;
  case deleteBtnPressed:{
    if (selection == 0 || selection == 1){
        if (seqStarted && selection == seqPlayPosition ||seqStarted && selection == seqPlayPosition -1 ){  
          if(sequencerArray[selection].numNotes == 1){
          MIDI.sendNoteOff(sequencerArray[selection].seqStepNotes[0],0,seqOutChannel);
          }
          if(sequencerArray[selection].numNotes == 2){
            for(byte i = 0;i<8;i++){
              if (sequencerArray[selection].seqStepNotes[i] != 255){
              MIDI.sendNoteOff(sequencerArray[selection].seqStepNotes[i],0,seqOutChannel);      
              }
            }
          
          }
        }
        memset(sequencerArray[selection].seqStepNotes, 255, sizeof(sequencerArray[selection].seqStepNotes));
        sequencerArray[selection].numNotes = 0;
        seqSwang[selection] = 0;
        if(selection == 1){
          selection--;
        }
        DS = false;
        return;      
    }

    if (selection < seqLength - 1 && selection > 1){
        if (seqStarted && seqNotePlaying == sequencerArray[selection].seqStepNotes[0]){
          MIDI.sendNoteOff(sequencerArray[selection].seqStepNotes[0],0,seqOutChannel);
        }
      memset(sequencerArray[selection].seqStepNotes, 255, sizeof(sequencerArray[selection].seqStepNotes));
      sequencerArray[selection].numNotes = 0;
      seqSwang[selection] = 0;
      selection--;
      DS = false;
      return;
    }

    
    if (selection == seqLength-1){
        if (seqStarted && seqNotePlaying == sequencerArray[selection].seqStepNotes[0]){
          MIDI.sendNoteOff(sequencerArray[selection].seqStepNotes[0],0,seqOutChannel);
          seqNotePlaying = 255;
          sequencerArray[selection].numNotes = 0;
        }
      if (sequencerArray[selection].seqStepNotes[0] != 255){
        memset(sequencerArray[selection].seqStepNotes, 255, sizeof(sequencerArray[selection].seqStepNotes));
        sequencerArray[selection].numNotes = 0;
        seqSwang[selection] = 0;
        DS = false;
        return;
      }
      if (sequencerArray[selection].seqStepNotes[0] == 255 && selection > 0){
        if (seqStarted && seqNotePlaying == sequencerArray[selection].seqStepNotes[0]){
          MIDI.sendNoteOff(sequencerArray[selection].seqStepNotes[0],0,seqOutChannel);
          sequencerArray[selection].numNotes = 0;
          seqNotePlaying = 255;
        }
      seqSwang[selection] = 0;
      seqLength--;
      selection--;
      DS = false;
      return;
    }
    }

  }
  break;
}
}
if (seqSubMenuVisible){
  switch(subMenuSelection){
    case 0:
    case 1:
    case 2:{
    selectedParameterMin = &seqRecordModeMin;
    selectedParameterMax = &seqRecordModeMax;
    selectedParameterVal = &seqRecordMode;
    }
    break;
    case 3:
    selectedParameterVal = nullptr;
    selectedParameterMin = nullptr;
    selectedParameterMax = nullptr;
    selectedBool = &seqRingout;
    break;
    case 4:
    selectedParameterVal = nullptr;
    selectedParameterMin = nullptr;
    selectedParameterMax = nullptr;
    selectedBool = &seqTie;
    break;
    case 5:
    selectedBool = nullptr;
    selectedParameterVal = &seqVel;
    selectedParameterMin = &seqVelMin;
    selectedParameterMax = &seqVelMax;
    break;
  }
        switch (encState) {
    case increased:
      if(subMenuSelection == 5){
        increaseParameterValue(selectedParameterVal, selectedParameterMax);
        DS = false;
        return;
      }
      break;
    case decreased:
      if (subMenuSelection == 5){
        decreaseParameterValue(selectedParameterVal, selectedParameterMin);
        DS = false;
        return;
      }
      break;
  }
  switch (buttonPressed){
    case yesBtnPressed:{
      if (subMenuSelection <=2){
        seqRecordMode = subMenuSelection;
        DS = false;
        return;
      }
      if (subMenuSelection == 3){  
       if (seqStarted){
          for (byte i = 0; i < seqLength; i++){
                if (sequencerArray[i].numNotes == 1){
                  MIDI.sendNoteOff(sequencerArray[i].seqStepNotes[0],0,seqOutChannel);
                  }
                if(sequencerArray[i].numNotes == 2){
                  for(byte j = 0; j < 8; j++){
                  if(sequencerArray[i].seqStepNotes[j]!=255){  
                    MIDI.sendNoteOff(sequencerArray[i].seqStepNotes[j],0,seqOutChannel);
                    }
                  if(sequencerArray[i].seqStepNotes[j]==255){
                    break;
                  }
                }
              }
            
          }
        changeBoolState(selectedBool);
        if (seqTie){
          seqTie = false;
        }
        DS = false;
        return;
      }
       }
      
      if (subMenuSelection == 4){
        if (seqStarted){
          for (byte i = 0; i < seqLength; i++){
                if (sequencerArray[i].numNotes == 1){
                  MIDI.sendNoteOff(sequencerArray[i].seqStepNotes[0],0,seqOutChannel);
                  }
                if(sequencerArray[i].numNotes == 2){
                  for(byte j = 0; j < 8; j++){
                  if(sequencerArray[i].seqStepNotes[j]!=255){  
                    MIDI.sendNoteOff(sequencerArray[i].seqStepNotes[j],0,seqOutChannel);
                    }
                  if(sequencerArray[i].seqStepNotes[j]==255){
                    break;
                  }
                }
              }
            
          }
        }
        changeBoolState(selectedBool);
        if (seqRingout){
          seqRingout = false;
        }
        DS = false;
        return;
      }
    }
    break;
    case leftBtnPressed:{
      if (subMenuSelection == 5){
        decreaseParameterValue(selectedParameterVal, selectedParameterMin);
        DS = false;
        return;
      }
    }
    break;
    case rightBtnPressed:{
      if (subMenuSelection == 5){
        increaseParameterValue(selectedParameterVal, selectedParameterMin);
        DS = false;
        return;
      }
    }
    break;
  }
}
}



void handleValueEditsForLfo() {
  if (selection >= 0 && selection < sizeof(lfoConfig) / sizeof(lfoConfig[0])) {
    selectedBool = lfoConfig[selection].selectedBool;
    selectedParameterVal = lfoConfig[selection].selectedParameterVal;
    selectedParameterMin = lfoConfig[selection].selectedParameterMin;
    selectedParameterMax = lfoConfig[selection].selectedParameterMax;
  }

  switch (encState) {
    case increased:
      if (selectedParameterVal != nullptr) {
         if(selection == 2 && lfoAppStatus){
            MIDI.sendControlChange(tempDeviceCcNum,tempDeviceCcVal,lfoOutChannel);
            panelCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
            deviceCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
          }
        increaseParameterValue(selectedParameterVal, selectedParameterMax);
        DS = false;
        if (selection == 2) {
          selectedLfoCcNum = deviceCcNums[deviceLfoIndex];
          tempDeviceCcNum = selectedLfoCcNum;
          tempDeviceLfoIndex = deviceLfoIndex;
          tempDeviceCcVal = deviceCcValues[deviceLfoIndex];
          lfoOscVal = deviceCcValues[deviceLfoIndex];
          DS = false;
        }
        return;
      }
      break;
    case decreased:
      if (selectedParameterVal != nullptr) {
         if(selection == 2 && lfoAppStatus){
            MIDI.sendControlChange(tempDeviceCcNum,tempDeviceCcVal,lfoOutChannel);
            panelCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
            deviceCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
          }
        decreaseParameterValue(selectedParameterVal, selectedParameterMin);
        DS = false;
        if (selection == 2) {
          selectedLfoCcNum = deviceCcNums[deviceLfoIndex];
          tempDeviceCcNum = selectedLfoCcNum;
          tempDeviceLfoIndex = deviceLfoIndex;
          tempDeviceCcVal = deviceCcValues[deviceLfoIndex];
          lfoOscVal = deviceCcValues[deviceLfoIndex];
          DS = false;
        }
        return;
      }
      break;
  }

  switch (buttonPressed) {
    case encoderBtnPressed:
    case yesBtnPressed:
      changeBoolState(selectedAppStatus);
      DS = false;
      return;
      break;
    case leftBtnPressed:
      if (selection == 0) {
        decreaseParameterValue(selectedParameterVal, selectedParameterMin);
        DS = false;
        return;
      }
      break;
    case rightBtnPressed:
      if (selection == 0) {
        increaseParameterValue(selectedParameterVal, selectedParameterMax);
        DS = false;
        return;
      }
      break;
    case upBtnPressed:
      if (selection == 2 || selection == 3 || selection == 4) {
         if(selection == 2 && lfoAppStatus){
            MIDI.sendControlChange(tempDeviceCcNum,tempDeviceCcVal,lfoOutChannel);
            panelCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
            deviceCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
          }
        increaseParameterValue(selectedParameterVal, selectedParameterMax);
        DS = false;
        if (selection == 2) {
          selectedLfoCcNum = deviceCcNums[deviceLfoIndex];
          tempDeviceCcNum = selectedLfoCcNum;
          tempDeviceLfoIndex = deviceLfoIndex;
          tempDeviceCcVal = deviceCcValues[deviceLfoIndex];
          lfoOscVal = deviceCcValues[deviceLfoIndex];
          DS = false;
        }
        return;
      }
      break;
    case downBtnPressed:
      if (selection == 2 || selection == 3 || selection == 4) {
         if(selection == 2 && lfoAppStatus){
            MIDI.sendControlChange(tempDeviceCcNum,tempDeviceCcVal,lfoOutChannel);
            panelCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
            deviceCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
          }
        decreaseParameterValue(selectedParameterVal, selectedParameterMin);
        DS = false;
        if (selection == 2) {
          selectedLfoCcNum = deviceCcNums[deviceLfoIndex];
          tempDeviceCcNum = selectedLfoCcNum;
          tempDeviceLfoIndex = deviceLfoIndex;
          tempDeviceCcVal = deviceCcValues[deviceLfoIndex];
          lfoOscVal = deviceCcValues[deviceLfoIndex];
          DS = false;
        }
        return;
      }
      break;
  }
}

void handleValueEditsForMidiOutChannels() {
  if (selectedMidiOutChannel >= 0 && selectedMidiOutChannel < sizeof(midiOutChannelsConfig) / sizeof(midiOutChannelsConfig[0])) {
    selectedBool = midiOutChannelsConfig[selectedMidiOutChannel].selectedBool;
    selectedParameterVal = midiOutChannelsConfig[selectedMidiOutChannel].selectedParameterVal;
    selectedParameterMin = midiOutChannelsConfig[selectedMidiOutChannel].selectedParameterMin;
    selectedParameterMax = midiOutChannelsConfig[selectedMidiOutChannel].selectedParameterMax;
  }

switch(buttonPressed){
    case leftBtnPressed:{
        decreaseParameterValue(selectedParameterVal, selectedParameterMin);
        displayedMidiOutChannels[selection] = *midiChannels[selectedMidiOutChannel]; 
        DS = false;
        return;
    }
      break;
    case rightBtnPressed:{
        increaseParameterValue(selectedParameterVal, selectedParameterMax);
        displayedMidiOutChannels[selection] = *midiChannels[selectedMidiOutChannel]; 
        DS = false;
        return;
    }
      break;    
}
}

void handleValueEditsForApp(ValueEditConfig* valueEditConfig, int configSize) {
    switch (encState) {
        case increased:
            if (tempoBarVisible) {
                updateSelectedTempo(1, selectedTempo);
                DS = false;
                return;
            }
            break;
        case decreased:
            if (tempoBarVisible) {
                updateSelectedTempo(-1, selectedTempo);
                DS = false;
                return;
            }
            break;
    }

    if (selection < 0 || selection >= configSize) return;

    ValueEditConfig currentConfig = valueEditConfig[selection];

    selectedBool = currentConfig.configSelectedBool;
    selectedParameterVal = currentConfig.configSelectedParameterVal;
    selectedParameterMin = currentConfig.configSelectedParameterMin;
    selectedParameterMax = currentConfig.configSelectedParameterMax;
    
    switch (buttonPressed) {
        case yesBtnPressed:
        case encoderBtnPressed: {
            if (selectedBool != nullptr) {
                updateApp();
                changeBoolState(selectedBool);
                DS = false;
                return;
            }
            break;
        }
        case rightBtnPressed: {
            if (selectedBool != nullptr) {
                updateApp();
                changeBoolState(selectedBool);
                DS = false;
                return;
            }
            if (selectedParameterVal != nullptr) {
                updateApp();
                increaseParameterValue(selectedParameterVal, selectedParameterMax);
                DS = false;
            }
            break;
        }
        case leftBtnPressed: {
            if (selectedBool != nullptr) {
                updateApp();
                changeBoolState(selectedBool);
                DS = false;
                return;
            }
            if (selectedParameterVal != nullptr) {
                updateApp();
                decreaseParameterValue(selectedParameterVal, selectedParameterMin);
                DS = false;
            }
            break;
        }
    }
    

}

void handleValueEditsForTriggerFunctions(){

switch(buttonPressed){
  case encoderBtnPressed:
  case yesBtnPressed:
  if (navX == 1){
    triggerList[selectedTriggerIndex].functionNum = navY;
    DS = false;
  }
  break;
  }
}
void handleValueEditsForControllerOptions(){
  switch(encState){
    case increased:
    if(navY == 1){
    increaseParameterValue(&controllerOutChannel,&midiOutChannelMax);  
    DS = false;
    return;
    }
    break;
    case decreased:
    if(navY == 1){
    decreaseParameterValue(&controllerOutChannel,&midiOutChannelMin);  
    DS = false;
    return;
    }
    break;
    
  }
  switch(buttonPressed){
  case leftBtnPressed:
  if (navY == 0){
    changeBoolState(&ctrlrUseDeviceParams);
    DS = false;
    return;
  }
  if (navY == 1){
    decreaseParameterValue(&controllerOutChannel,&midiOutChannelMin);
    DS = false;
    return;
  }
  break;
  case rightBtnPressed:
  if (navY == 0){
    changeBoolState(&ctrlrUseDeviceParams);
    DS = false;
    return;
  }
  if (navY == 1){
    increaseParameterValue(&controllerOutChannel,&midiOutChannelMax);
    DS = false;
    return;
  }
  break;  
  }
}
void handleValueEditsForReMap(){
    selectedBool = nullptr;
    selectedParameterVal = nullptr;
    selectedParameterMin = nullptr;
    selectedParameterMax = nullptr;     
        
    switch(selection){
      case 0:{
      selectedBool = &reMapAppStatus;
      }     
      break;
      case 1:{
        if (receivedDeviceCc != 255){
          midiIndex[receivedDeviceCc]->ccNum;
          selectedBool = &midiIndex[receivedDeviceCc]->reMapped;
        }
      
      }
      break;
      case 2:{
        if (receivedDeviceCc != 255){
          midiIndex[receivedDeviceCc]->reMapCc;
          selectedBool = &midiIndex[receivedDeviceCc]->reMapped;
        }
      
      
        selectedParameterVal = &reMapNewCcNum;
        selectedParameterMin = &midiMin;
        selectedParameterMax = &midiMax;      
      }
      break;

    }
    
encNav(encState);
  
  switch(buttonPressed){
    case yesBtnPressed:{
      if(selection == 0){
        changeBoolState(selectedBool);
        DS = false;
        return;
      }
      if (selection == 1 || selection == 2){
        if (midiIndex[receivedDeviceCc]->ccNum != reMapNewCcNum){
          changeBoolState(selectedBool);
          if(*selectedBool == true){
          midiIndex[receivedDeviceCc]->reMapCc = reMapNewCcNum;
          }
          if(*selectedBool == false){
            midiIndex[receivedDeviceCc]->reMapCc = 255;
          }
          DS = false;
          return;   
          }
      }

    }
    break;
    case leftBtnPressed:{
      if(selection == 0){
      changeBoolState(selectedBool);
      DS = false;
      return;        
      }
     if(selection == 2){
      decreaseParameterValue(selectedParameterVal, selectedParameterMin);
      DS = false;
      return;
     }
    }
    break;
    case rightBtnPressed:{
      if (selection == 0){
      changeBoolState(selectedBool);
      DS = false;
      return;      
      }
      if (selection == 2){
        increaseParameterValue(selectedParameterVal, selectedParameterMax);
      DS = false;
      return;
      }

    }
    break;
  }
}
void handleValueEditsForSeqSwang(){
  
  switch(encState){
    case increased:
    increaseParameterValue(*selectedSeqSwang,seqSwangMax);
    DS = false;
    return;
    break;
    case decreased:
    decreaseParameterValue(*selectedSeqSwang,seqSwangMin);
    DS = false;
    return;
    break;
  }

}


void handleValueEditsForClockSource(){

switch(buttonPressed){
  case rightBtnPressed:
  increaseParameterValue(clockSource, clockSourceMax);
  DS = false;
  return;
  break;


  case leftBtnPressed:
  decreaseParameterValue(clockSource, clockSourceMin);
  DS = false;
  return;
  break;
}
}

void handleValueEdits() {
      ValueEditConfig* currentConfig = nullptr;
    int configSize = 0;
  //need an additional switch for selection. 
      switch (encState) {
    case increased:
      if(tempoBarVisible){
      updateSelectedTempo(1, selectedTempo);
      DS = false;
      }
      if(encoderBarVisible){
      updateEncoderBar(-5, selectedValue, 0 , 100);
      DS = false;  
      }
      break;
    case decreased:
      if (tempoBarVisible){
      updateSelectedTempo(-1, selectedTempo);
      DS = false;
      }
      if(encoderBarVisible){
      updateEncoderBar(5, selectedValue, 0 , 100);
      DS = false;  
      }
      break;
  }
  
switch(currentScreen){

  case DELETE:{
    handleValueEditsForDelete();
    break;
  }
  case SAVENEWFILE:{
    handleValueEditsForSaveNewFile();
    break;
  }
  case LFO:
  handleValueEditsForLfo();
    break;
  case SEQUENCER:
  handleValueEditsForSequencer();
  break;
  case REMAP:
  handleValueEditsForReMap();
  break;
  case MIDIOUTCHANNELS:
  handleValueEditsForMidiOutChannels();
  break;
  case TRIGGERS:
  handleValueEditsForTriggers();
  break;
  case SEQCHORDENTRY:
  handleValueEditsForSeqChordEntry();
  break;
  case CONTROLLER:
  handleValueEditsForController();
  break;
  case ADDCONTROLLERPARAM:
  handleValueEditsForAddControllerParam();  
  break;
  case TRIGGERFUNCTIONS:
  handleValueEditsForTriggerFunctions();
  break;
  case CONTROLLEROPTIONS:
  handleValueEditsForControllerOptions();
  break;
  case SEQSWANG:
  handleValueEditsForSeqSwang();
  break;
  case CLOCKSOURCE:
  handleValueEditsForClockSource();
  break;

        case ARPEGGIATOR:
            currentConfig = arpeggiatorConfig;
            configSize = sizeof(arpeggiatorConfig) / sizeof(arpeggiatorConfig[0]);
            break;
        case ARPEGGIATOROPTIONS:
            currentConfig = arpSettingsConfig;
            configSize = sizeof(arpSettingsConfig) / sizeof(arpSettingsConfig[0]);
            break;
        case ARPNOTE:
            currentConfig = arpNoteConfig;
            configSize = sizeof(arpNoteConfig) / sizeof(arpNoteConfig[0]);
            break;
        case AUTOCHORD:
            currentConfig = autoChordConfig;
            configSize = sizeof(autoChordConfig) / sizeof(autoChordConfig[0]);
            break;
        case MIDITHRU:
            currentConfig = midiThruConfig;
            configSize = sizeof(midiThruConfig) / sizeof(midiThruConfig[0]);
            break;

}
    if (currentConfig != nullptr && currentScreen != LFO) {
        handleValueEditsForApp(currentConfig, configSize);
    }
    
}

void handleValueEditsForAddControllerParam(){
  byte alphabetMax = sizeof(alphabet)-1;
  byte alphabetMin = 0;
  switch(displayIndex){
    default:
    switch(encState){
      case increased:
      
      increaseParameterValue(&tempCcNameAlphaIndex[displayIndex -1], &alphabetMax);
      DS = false;
      break;
      case decreased:
      decreaseParameterValue(&tempCcNameAlphaIndex[displayIndex -1], &alphabetMin);
      DS = false;
      break;
    
    }
  
  break;
  case 0: 
    switch (encState) {
    case increased:
      increaseParameterValue(&tempCcNum, &midiMax);
      DS = false;
      
      break;
    case decreased:
      decreaseParameterValue(&tempCcNum, &midiMin);
      DS = false;
      break;
    }
  break;
  case 15:
  break;
  case 16:
  break;

  }
   

  
  switch(buttonPressed){
    case yesBtnPressed:{
      if (displayIndex == 16){

        char tempArray[15] = {}; 
        for(byte i = 0; i < 15; i++){
          if (tempCcName[i] != 255){
          tempArray[i] = tempCcName[i];
          }
        }
        tempArray[14] = '\0';        
        
        
        CtrlrStruct newEntry;
        byte tmpIndex = ctrlrParamList.size();
        if(tmpIndex > 0){
          tmpIndex--;
        }
        newEntry.index = tmpIndex;
        strcpy(newEntry.ccName, tempArray);  // Set the paramName
        newEntry.ccNum = tempCcNum;  // Set the paramCcNum
        newEntry.ccVal = 0;  // Set the paramCcVal  
        insertEntry(newEntry, ctrlrParamList);
        controllerParamsCount = ctrlrParamList.size() - 1;
        currentScreen = CONTROLLER;
        screenSwitch();
      }      
    }
    break;
  }
}

void handleValueEditsForController(){
  bool controlSelected = false;
  if (controllerParamsCount > 0 && ctrlrX == 1){
  if (*ctrlrPtrs[ctrlrY].index != 255){
    selectedControllerParamNum = &ctrlrPtrs[ctrlrY].ccNum;
    selectedControllerParamVal = &ctrlrPtrs[ctrlrY].ccVal;    
    controlSelected = true;
  }else if(*ctrlrPtrs[ctrlrY].index == 255 || ctrlrX == 0){
    controlSelected = false;
  }

  
  if(controlSelected && controllerAppStatus){
      byte tmpCcNum = (**selectedControllerParamNum);
      byte tmpCcVal = (**selectedControllerParamVal);
    switch(encState){
      case increased:
      if (tmpCcVal < midiMax){
      increaseParameterValue(&tmpCcVal, &midiMax);
      MIDI.sendControlChange(tmpCcNum,tmpCcVal,controllerOutChannel);
      (**selectedControllerParamVal)++;
      
      if (ctrlrUseDeviceParams){
        deviceCcValues[ctrlrIndex] = **selectedControllerParamVal;
        midiIndex[tmpCcNum]->ccVal = tmpCcVal;
        midiIndex[tmpCcNum]->morphStart = tmpCcVal;
        midiIndex[tmpCcNum]->morphCalc = tmpCcVal;
        if(midiIndex[tmpCcNum]->obtained == false){
          midiIndex[tmpCcNum]->obtained = true;
        }
        
      }
      DS = false;
      }
      break;
      case decreased:
      if (tmpCcVal >  midiMin){
      decreaseParameterValue(&tmpCcVal, &midiMin);
      MIDI.sendControlChange(tmpCcNum,tmpCcVal,controllerOutChannel);
      (**selectedControllerParamVal)--;
      
      if (ctrlrUseDeviceParams){
        deviceCcValues[ctrlrIndex] = tmpCcVal;
        midiIndex[tmpCcNum]->ccVal = tmpCcVal;
        midiIndex[tmpCcNum]->morphStart = tmpCcVal;
        midiIndex[tmpCcNum]->morphCalc = tmpCcVal;
        if(midiIndex[tmpCcNum]->obtained == false){
          midiIndex[tmpCcNum]->obtained = true;
        }
        
      }
      DS = false;
      }
      break;
    }
  }
  }
  
}

void handleValueEditsForSeqChordEntry(){

 switch(buttonPressed){
    case yesBtnPressed:{
    if (navX == 0 && navY == 0){
    for (byte i = 0; i < 8; i++) {  // Loop through notes 0-127
      MIDI.sendNoteOff(sequencerArray[selectedArrayEntry].seqStepNotes[i], 0, seqOutChannel);  // Send Note Off message
    }
    memset(tempChord,255,8);
    memset(sequencerArray[selectedArrayEntry].seqStepNotes, 255, sizeof(sequencerArray[selectedArrayEntry].seqStepNotes));   
    if(seqChordPlaying){
      seqChordPlaying = false;
    }
    DS = false;
    return;
    }
    if (navX == 1 && navY == 0){
      if(seqChordPlaying && playingSeqChordPosition == selectedArrayEntry){
            for (byte i = 0; i<8;i++) {  // Loop through notes 0-127
                
              if (sequencerArray[selectedArrayEntry].seqStepNotes[i] != 255){
                MIDI.sendNoteOff(sequencerArray[selectedArrayEntry].seqStepNotes[i], 0, seqOutChannel);  // Send Note Off message
                if(i>0){
                  sequencerArray[selectedArrayEntry].seqStepNotes[i] = 255;
                }
              }
            }

         currentScreen = SEQUENCER;
         screenSwitch();
         return;
      }
      
    if (!seqChordPlaying){
      memset(sequencerArray[selectedArrayEntry].seqStepNotes, 255, sizeof(sequencerArray[selectedArrayEntry].seqStepNotes));   
      
         currentScreen = SEQUENCER;
         screenSwitch();
         return;
    }      
    }
    if (navY == 1){
      currentScreen = SEQUENCER;
      screenSwitch();
      return;
    }

    }
    break; 
 }
}
