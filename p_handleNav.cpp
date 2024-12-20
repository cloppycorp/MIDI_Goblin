#include "p_handleNav.h"
bool noteEdit = false;
void finishNoteEdit(){
      if(noteEdit){
      MIDI.sendNoteOff(sequencerArray[selection].seqStepNotes[0],seqVel,seqOutChannel);
      encNoteTempNote = sequencerArray[selection].seqStepNotes[0];
      sequencerArray[selection].numNotes = 1;
      noteEdit = false;
      tempoBarVisible = true;
      navBtnPressed = true;
      DS = false;
      }
}

void loadAppValues() {
  switch (currentScreen) {
    case DEVICE_SELECT:{
      
      strcpy(currentScreenName, "DEVICE SELECT");
      initiateDisplayedListEntries(displayedListEntries,deviceNames);
      valueMinMax(displayIndex, 0, displayIndexMin, 0, displayIndexMax, 4);      
      selectedDevice = 0;
      selectedListIndex = &selectedDevice;
      selectedListIndexMax = foldersCount - 1;
      break;
    }
    case MENU:{
      strcpy(currentScreenName, "MENU");
      appMenuIndex = 0;
      valueMinMax(displayIndex, 0, displayIndexMin, 0, displayIndexMax, 4);      
      selectedListIndex = &appMenuIndex;
      selectedListIndexMax = (sizeof(appNames) / sizeof(appNames[0])) - 1;
      initiateDisplayedListEntries(displayedListEntries,appNames);
      encoderBarVisible = false;
      tempoBarVisible = false;      
      break;
    }
    case PATCHES:{
      printVectorContents(patchList);
      previousScreen = currentScreen;
      strcpy(currentScreenName, "PATCHES");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, (patchFileCount > 5) ? 4 : (patchFileCount > 0 ? patchFileCount - 1 : 0));
      selectedFile = 0;
      selectedList = &patchList;
      selectedDisplayedNames = displayedPatchNames;
      selectedListFileCount = &patchFileCount;
      selectedListFileCapacity = patchFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/PATCHES";
      
      break;
    }
    case USERBUTTONCHOOSEPATCH:{
      strcpy(currentScreenName, userButtonNames[selectedUserButtonIndex]);
      strcat(currentScreenName, " ");
      strcat(currentScreenName, "PATCH");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, (patchFileCount > 5) ? 4 : (patchFileCount > 0 ? patchFileCount - 1 : 0));
      selectedFile = 0;
      selectedList = &patchList;
      selectedDisplayedNames = displayedPatchNames;
      selectedListFileCount = &patchFileCount;
      selectedListFileCapacity = patchFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/PATCHES";
    }
    break;
    case TRIGGERPATCHES:{
      strcpy(currentScreenName, triggerNames[selectedTrigger]);
      strcat(currentScreenName, " ");
      strcat(currentScreenName, "PATCH");    
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, (patchFileCount > 5) ? 4 : (patchFileCount > 0 ? patchFileCount - 1 : 0));
      selectedFile = 0;
      selectedList = &patchList;
      selectedDisplayedNames = displayedPatchNames;
      selectedListFileCount = &patchFileCount;
      selectedListFileCapacity = patchFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/PATCHES";
    }
    
    break;
    case SEQUENCES:{
      previousScreen = currentScreen;
      strcpy(currentScreenName, "SEQUENCES");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum,(sequenceFileCount > 5) ? 4: (sequenceFileCount > 0 ? sequenceFileCount - 1:0));
      selectedFile = 0;
      selectedList = &sequenceList;
      selectedDisplayedNames = displayedSequenceNames;
      selectedListFileCount = &sequenceFileCount;
      selectedListFileCapacity = sequenceFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/SEQUENCES";
      
    }
    
    break;
    case REMAPLIST:{
      previousScreen = currentScreen;
      strcpy(currentScreenName, "REMAPS");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum,(reMapFileCount > 5) ? 4: (reMapFileCount > 0 ? reMapFileCount - 1:0));
      selectedFile = 0;
      selectedList = &reMapList;
      selectedDisplayedNames = displayedReMapFileNames;
      selectedListFileCount = &reMapFileCount;
      selectedListFileCapacity = reMapFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/REMAPS";      
    }
    break;
    case DELETE:{
      strcpy(currentScreenName, "DELETE");
      
    break;  
    }
    case SAVENEWFILE:{
      strcpy(currentScreenName, "NEW FILE");
      initializeNewFileNameArray();
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, MAX_FILE_NAME_LENGTH -1);
      alphabetIndex = selection;
      newFileNameArrayIndexes[selection] = alphabetIndex;
      newFileNameArray[selection] = alphabet[newFileNameArrayIndexes[selection]];
      
      break;
    }
    case ARPEGGIATOR:{
      selectedAppStatus = &arpAppStatus;
      strcpy(currentScreenName, "ARPEGGIATOR");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 3);
      selectedTempo = &arpeggiatorTempo;
      tempoBarVisible = true;
      break;
    }
    case REMAP:{
    previousScreen = currentScreen;
    strcpy(currentScreenName, "REMAP");
    selectedAppStatus = &reMapAppStatus;  
    valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 3);
    break;
    }
    case ARPNOTE:{
    
    selectedAppStatus = &arpNoteAppStatus;
    strcpy(currentScreenName, "ARP-NOTE");
    valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 3);
    //6 because the array is zero indexed
    selectedTempo = &arpNoteTempo;
    tempoBarVisible = true;
      break;
    }
    case AUTOCHORD:{
    selectedAppStatus = &autoChordAppStatus;
    strcpy(currentScreenName, "AUTO-CHORD");
    valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 3);
    selectedTempo = nullptr;
    tempoBarVisible = false;
    strcpy(tableColumnA_0, "STATUS");
    strcpy(tableColumnB_0, (*selectedAppStatus ?  "ON" : "OFF"));
    strcpy(tableColumnA_1, "KEY");
    strcpy(tableColumnB_1, "XXXX");
    strcpy(tableColumnA_2, "LENGTH");
    strcpy(tableColumnB_2, "XXXX");
    strcpy(tableColumnA_3, "VELOCITY");
    strcpy(tableColumnB_3, "XXXX");      
    break;
    }
    case MONITOR:{
      selectedAppStatus = nullptr;
      lastMsgReceived = Nothing;
      msgJustReceived = Nothing;
      initDeviceMonitor();
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 4);
      selectedDeviceParam = 0;
      selectedCcParameter = 0;
    break;
    }
    case LFO:{
      selectedAppStatus = &lfoAppStatus;
      strcpy(currentScreenName, "LFO");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 4);
      valueMinMax(lfoWaveForm, 0, lfoWaveFormMin, 0, lfoWaveFormMax, 2);
      valueMinMax(lfoFreq, 25, lfoFreqMin, 0, lfoFreqMax, 100);
      valueMinMax(lfoDepth, 50, lfoDepthMin, 0, lfoDepthMax, 100);
      tempoBarVisible = false;
      lfoDeviceCcMin = 0;
      lfoDeviceCcMax = numEntries-1;
      break;
    }
    case SEQUENCER:{
      selectedTempo = &sequencerTempo;
      selectedAppStatus = &seqAppStatus;
      previousSeqSelection = 0;
      selection = 0;
      seqSubMenuVisible = false;
      subMenuSelection = 0;
      defineSeqSubMenuGrid();
      defineSeqGrid();
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 31);
      valueMinMax(subMenuSelection, 0, subMenuSelectionMin, 0, subMenuSelectionMax, 6);
      if(clockSource == 1 && midiClockStarted){
      tempoBarVisible = false;
      }else{
        tempoBarVisible = true;
      }
    }
    break;
    case OVERWRITE:{
      strcpy(currentScreenName, "OVERWRITE");
      
    }
    break;
    case MESSAGE:{
     messageStartTime = millis();
     messageTimeout = 1500;
      
    }
    break;
    case TRIGGERS:{
      strcpy(currentScreenName, "NAME    NUM VAL ON?  ");
      navX = 0;
      navY = 0;
      navXMin = 0;
      navYMin = 0;
      if(triggerList.size() > 0 && triggerList.size() <= 5 ){
      navXMax = 3;
      navYMax = triggerList.size()-1;
      }

      if(triggerList.size() > 5){
      navXMax = 3;  
      navYMax = 4;
      }
      
      
      
      
      initTriggerPtrs();
      Serial.println("trigger ptrs created");
      //initTriggerList();
      populateTriggerPtrs();//populates the triggerptr list with the triggers list
      Serial.println("populated?");
      selectedTriggerIndex = 0;
      //initiateDisplayedListEntries(displayedListEntries,triggerNames);
      tempoBarVisible = false;
      
    }
    break;
    case TRIGGERFUNCTIONS:{
      strcpy(currentScreenName, triggerList[selectedTriggerIndex].name);
      initiateDisplayedListEntries(displayedListEntries,functionNames);
      defineListGridColumn(listGridColumnB, 105, 10); // Example x coordinate
      navX = 0;
      navY = 0;
      navXMax = 1;
      navYMax = 4;
      displayIndex = 0;
    }
    break;
    case SETTINGS:{
      strcpy(currentScreenName, "SETTINGS");
      initiateDisplayedListEntries(displayedListEntries,settingsList);
      displayIndex = 0;
      selectedSetting = 0;
      selectedListIndex = &selectedSetting;
      selectedListIndexMax = (sizeof(settingsList) / sizeof(settingsList[0])) - 1;
      
    }
    break;
    case MIDIOUTCHANNELS:{
      strcpy(currentScreenName, "MIDI CHANNELS");
      initiateDisplayedListEntries(displayedListEntries,midiOutChannelNames);
      initiateDisplayedMidiOutChannels();
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, 4);
      selectedMidiOutChannel = 0;
      
    }
    break;
    case USERBUTTONSELECT:{
       strcpy(currentScreenName, "USER BUTTONS");
       displayIndex = 0;
       selectedUserButtonIndex = 0;
       selectedListIndex = &selectedUserButtonIndex;
       initiateDisplayedListEntries(displayedListEntries,userButtonNames);
       selectedListIndexMax = (sizeof(userButtonNames) / sizeof(userButtonNames[0])) - 1;
    }
    break;
  
  case USERBUTTONFUNCTIONS:{
    strcpy(currentScreenName, userButtonNames[selectedUserButtonIndex]);
    initiateDisplayedListEntries(displayedListEntries,functionNames);
    functionIndex = 0;   
    displayIndex = 0;
    selectedListIndex = &functionIndex;
    selectedListIndexMax = (sizeof(functionNames) / sizeof(functionNames[0])) - 1;
    functionSelectionLeftRight = false;
    selectedBool = &functionSelectionLeftRight;

    
  }
  break;
  case USERBUTTONCHOOSEAPP:{
      strcpy(currentScreenName, "CHOOSE AN APP");
      updateChosenAppArray(selectedButtonAppToToggle);
      initiateDisplayedListEntries(displayedListEntries2,chosenAppArray);
      initiateDisplayedListEntries(displayedListEntries,toggleableAppNames);
      toggleableAppIndex = 0;
      displayIndex = 0;         
      selectedListIndex = &toggleableAppIndex;
      selectedListIndexMax = (sizeof(toggleableAppNames) / sizeof(toggleableAppNames[0])) - 1;
  }
  break;
  case TRIGGERCHOOSEAPP:{
      strcpy(currentScreenName, "CHOOSE AN APP");
      updateChosenAppArray(triggerList[selectedTriggerIndex].appNum);
      initiateDisplayedListEntries(displayedListEntries,toggleableAppNames);
      initiateDisplayedListEntries(displayedListEntries2,chosenAppArray);
      toggleableAppIndex = 0;
      displayIndex = 0;
      selectedListIndex = &toggleableAppIndex;
      selectedListIndexMax = (sizeof(toggleableAppNames) / sizeof(toggleableAppNames[0])) - 1;
  }
  break;
  case SEQCHORDENTRY:{
    strcpy(currentScreenName, "CHORD ENTRY");
    memset(tempChord, 255, sizeof(tempChord));
    defineChordEntryGrid();
    valueMinMax(navX, 0, navXMin, 0, navXMax, 1);
    valueMinMax(navY, 0, navYMin, 0, navYMax, 1);
    
  }
  break;
  case CONTROLLER:{
    strcpy(currentScreenName, "NAME          CC VAL");
    resetCtrlrDispPtrs();
    ctrlrX = 0;
    ctrlrY = 0;
    displayIndex = 0;
    ctrlrIndex = 0;
    byte tmpListSize;
    if(!ctrlrUseDeviceParams){
    tmpListSize = ctrlrParamList.size();
    }
    if(ctrlrUseDeviceParams){
    tmpListSize = deviceParams.size();
    }
    if (tmpListSize > 0){
    if (!ctrlrUseDeviceParams){
    populateDispPtrs(ctrlrPtrs,ctrlrParamList);
    }
    if (ctrlrUseDeviceParams){
    populateDispPtrs(ctrlrPtrs,deviceParams);
    }
    controllerParamsCount = tmpListSize;
    ctrlrIndexMax = controllerParamsCount - 1;
    ctrlrYMax = controllerParamsCount;
    ctrlrXMax = 1;
    }
    if (tmpListSize == 0){
    controllerParamsCount = 0;
    displayIndexMax = 0;
    ctrlrIndexMax = 0;
    ctrlrYMax = 0;
    ctrlrXMax = 0;
    }
    
    
    defineListGridColumn(listGridColumnA, 0, 10); // Example x coordinate
    defineListGridColumn(listGridColumnB, 89, 10); // Example x coordinate
    defineListGridColumn(listGridColumnC, 109, 10); // Example x coordinate  
    defineListGridColumn(listGridColumnD, 113, 10); // Example x coordinate 
    Serial.println("grids defined");
    
    if(controllerParamsCount>0){
      
      if (controllerParamsCount < 5){
      ctrlrYMax = controllerParamsCount;
      ctrlrXMax = 1;
      displayIndexMax = controllerParamsCount;
      Serial.print("displayindexmax: ");
      Serial.println(displayIndexMax);
    }
    if (controllerParamsCount >= 5){
      displayIndexMax = 4;
      ctrlrYMax = 4;
      ctrlrXMax = 1;
      Serial.print("displayindexmax: ");
      Serial.println(displayIndexMax);      
    }
    }


  }
  break;
  case ADDCONTROLLERPARAM:{
    tempCcNum = 0;
    strcpy(currentScreenName, "ADD CC INFO");
    defineCcParamGrid();
    valueMinMax(displayIndex, 0, displayIndexMin, 0, displayIndexMax, 16);
    memset(tempCcName, ' ', sizeof(tempCcName));
    memset(tempCcNameAlphaIndex, 255, sizeof(tempCcNameAlphaIndex));
    
  }
  break;
  case CONTROLLEROPTIONS:{
    strcpy(currentScreenName, "OPTIONS");
    navY = 0;
    navYMax = 2;
    defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
    defineListGridColumn(listGridColumnB, 100, 10); // Example x coordinate
  }
  break;
  case MORPH:{
    strcpy(currentScreenName, "MORPH");
    if(!morphed && selectedMorphPatch != 999){
      populateMorphEnd(patchList, selectedMorphPatch);
      morphPosition = 0;
    }
    navY = 0;
    navYMax = 1;
    selectedValue = &morphPosition;
  }
  break;
  case MORPHPATCHES:
      previousScreen = currentScreen;
      strcpy(currentScreenName, "PATCH TO MORPH TO");
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum, (patchFileCount > 5) ? 4 : (patchFileCount > 0 ? patchFileCount - 1 : 0));
      selectedFile = 0;
      selectedList = &patchList;
      selectedDisplayedNames = displayedPatchNames;
      selectedListFileCount = &patchFileCount;
      selectedListFileCapacity = patchFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/PATCHES";
  break;
  case SEQSWANG:{
    strcpy(currentScreenName, "SWANG");
    tempoBarVisible = false;
  }
  break;
  case ARPEGGIATOROPTIONS:{
    strcpy(currentScreenName, "OPTIONS");
    tempoBarVisible = false;
    selection = 0;
    selectionMinimum = 0;
    selectionMaximum = 5;
  }
  break;
  case MIDITHRU:{
    strcpy(currentScreenName, "THRU OPTIONS");
    navBtnPressed = true;
    defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
    defineListGridColumn(listGridColumnB, 108, 10); // Example x coordinate
    selection = 0;
    selectionMinimum = 0;
    selectionMaximum = 3;
    tempoBarVisible = false;   
  }
  break;
  case CLOCKSOURCE:{
    strcpy(currentScreenName, "CLOCK SOURCE");
    navBtnPressed = true;
    selection = 0;
    selectionMinimum = 0;
    selectionMaximum = 0;
    tempoBarVisible = false;
  }
  break;
  case SYSTEMINFO:{
    strcpy(currentScreenName, "SYSTEM INFO");
    navBtnPressed = true;
    selection = 0;
    selectionMinimum = 0;
    selectionMaximum = 0;
    tempoBarVisible = false;
  }
  break;
  case USERBUTTONCHOOSESESEQUENCE:{
      strcpy(currentScreenName, userButtonNames[selectedUserButtonIndex]);
      strcat(currentScreenName, " ");
      strcat(currentScreenName, "SEQUENCES");
      previousScreen = currentScreen;
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum,(sequenceFileCount > 5) ? 4: (sequenceFileCount > 0 ? sequenceFileCount - 1:0));
      selectedFile = 0;
      selectedList = &sequenceList;
      selectedDisplayedNames = displayedSequenceNames;
      selectedListFileCount = &sequenceFileCount;
      selectedListFileCapacity = sequenceFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/SEQUENCES";
  }
  break;
  case TRIGGERCHOOSESEQUENCE:{
      previousScreen = currentScreen;
      strcpy(currentScreenName, triggerNames[selectedTrigger]);
      strcat(currentScreenName, " ");
      strcat(currentScreenName, "SEQUENCES");   
      valueMinMax(selection, 0, selectionMinimum, 0, selectionMaximum,(sequenceFileCount > 5) ? 4: (sequenceFileCount > 0 ? sequenceFileCount - 1:0));
      selectedFile = 0;
      selectedList = &sequenceList;
      selectedDisplayedNames = displayedSequenceNames;
      selectedListFileCount = &sequenceFileCount;
      selectedListFileCapacity = sequenceFileCapacity;
      initializeDisplayedFileNames(*selectedList, selectedDisplayedNames);
      selectedPathSuffix = "/SEQUENCES";
  } 
  break;
  }

}



void chooseDevice() {
  displayMsg("loading...");
  strcpy(deviceFolder, deviceNames[*selectedListIndex]);
  getSynthMidiInfo();
  //verifyVariables(); //used for troubleshooting
  getDeviceSettings();
  createPatchList();
  createRemapList();
  createSequenceList();
  createTrackerList();
  
  



  deviceLfoIndex = 0;
  selectedLfoCcNum = deviceCcNums[deviceLfoIndex];
  delete[] deviceNames;
  deviceNames = nullptr;  // Optional: Set the pointer to nullptr to avoid dangling pointers

  initializeDeviceSettings();
  loadSettings();
  deviceSettings.clear();
  loadTriggerFile();
  loadUserButtonsFile();
  
  delay(1);
}

void screenSwitch() {
      // If it's one of the above, perform these actions
      loadAppValues();
      DS = false;
}
void encNav(byte encState){
    switch (encState) {
    case increased:
      buttonPressed = downBtnPressed;
      
      break;
    case decreased:
      buttonPressed = upBtnPressed;
      
      break;


  } 
}
void gotoMainMenu(){
  currentScreen = MENU;
  
  previousScreen = currentScreen;
  screenSwitch();
}

void userBtnSelectNavCommands(int* selectedListIndex){

  switch(buttonPressed){


   case yesBtnPressed:{
      //new
      selectedUserBtn = &userButtons[*selectedListIndex].buttonNumber;
      selectedUserBtnFnctn = &userButtons[*selectedListIndex].buttonFunction;
      selectedUserBtnApp = &userButtons[*selectedListIndex].buttonApp;
      selectedUserBtnPanic = &userButtons[*selectedListIndex].buttonPanic;
      selectedUserBtnPatch = userButtons[*selectedListIndex].buttonPatch;
      selectedUserBtnSequence = userButtons[*selectedListIndex].buttonSequence;
    switch(*selectedListIndex){
      case 0: 
      //old
      selectedButtonSelectedFunction = &button1ChosenFunction;
      selectedButtonAppToToggle = &button1AppToToggle;
      selectedButtonChosenAppArray = displayedButton1ChosenApp;
      selectedButtonChosenPatch = button1ChosenPatch;
      
      break;
      case 1: selectedButtonSelectedFunction = &button2ChosenFunction;
      selectedButtonAppToToggle = &button2AppToToggle;
      selectedButtonChosenAppArray = displayedButton2ChosenApp;
      selectedButtonChosenPatch = button2ChosenPatch;
      break;
      case 2: selectedButtonSelectedFunction = &button3ChosenFunction;
       selectedButtonAppToToggle = &button3AppToToggle;
       selectedButtonChosenAppArray = displayedButton3ChosenApp;
       selectedButtonChosenPatch = button3ChosenPatch;
      break;
    }
    previousScreen = currentScreen;
    currentScreen = USERBUTTONFUNCTIONS;

    screenSwitch();
    return;
   }
   break;
   
  
    case backBtnPressed:{
    currentScreen = SETTINGS;
    screenSwitch();
    return;
    }
    break;  
  
}
}

void userButtonFunctionsNavCommands(){

  switch(buttonPressed){

   case yesBtnPressed:{
    if (!functionSelectionLeftRight && *selectedListIndex == 1){

      currentScreen = USERBUTTONCHOOSEAPP;
      screenSwitch();
      return;
    }
    if(!functionSelectionLeftRight && *selectedListIndex == 2){
      currentScreen = USERBUTTONCHOOSEPATCH;
      screenSwitch();
      return;
    }
    if(!functionSelectionLeftRight && *selectedListIndex == 3){
      
      currentScreen = USERBUTTONCHOOSESESEQUENCE;
      screenSwitch();
      return;
    }
    if(functionSelectionLeftRight){
    *selectedButtonSelectedFunction = *selectedListIndex;  
    *selectedUserBtnFnctn = *selectedListIndex;
    DS = false;
    return;
   }
   }
   break;
   case leftBtnPressed:{
    changeBoolState(selectedBool);
    DS = false;
    return;
   }
   break;
   case rightBtnPressed:{
    changeBoolState(selectedBool);
    DS = false;
    return;
   }
   break;
    case backBtnPressed:{
      currentScreen = USERBUTTONSELECT;
      screenSwitch();
      return;
    }  

 
  
}
  }

void userBtnChooseAppNavCommands(){
switch(buttonPressed){
  case yesBtnPressed:{
    
    *selectedButtonAppToToggle = toggleableAppIndex;
    *selectedUserBtnApp = toggleableAppIndex;
    updateChosenAppArray(selectedButtonAppToToggle);
    for (byte i = 0; i < 5;i++){
      if (i != displayIndex){
strcpy(displayedListEntries2[i], "  ");
      }else{
        strcpy(displayedListEntries2[i], "X ");
      }
    }
    
    DS = false;
    return;
  }
  break;
  case backBtnPressed:{
    currentScreen = USERBUTTONSELECT;
    screenSwitch();
    return;
  }
  break;

}
}

void settingsNavCommands(int* selectedListIndex){
switch(buttonPressed){
    case backBtnPressed:{
    gotoMainMenu();
    return;
    break;
    }
    case encoderBtnPressed:
    case yesBtnPressed:{
      byte tmpSelection = *selectedListIndex;
      switch (tmpSelection){
        case 0:
        displayMsg("saving settings");
        saveSettings();
        break;
        case 1:
        currentScreen = USERBUTTONSELECT;
        break;
        case 2:
        currentScreen = MIDIOUTCHANNELS;
        break;
        case 3:
        currentScreen = MIDITHRU;
        break;
        case 4:
        currentScreen = CLOCKSOURCE;
        break;
        case 5:
        currentScreen = SYSTEMINFO;
        break;
      }
      screenSwitch();
      return;
    }
      break;
}
}
void handleNavForFileListLimited(){
//this version doesnt allow you to save or overwrite or delete. its just used for scrolling through file lists

  switch (buttonPressed) {


    case yesBtnPressed:
    case encoderBtnPressed: {
      if (*selectedListFileCount == 0){
        
        displayMsg("no file to send!");
        delay(500);
        DS = false;
        return;     
      }
      if(*selectedListFileCount > 0){
        switch(currentScreen){
          case USERBUTTONCHOOSEPATCH:{
          char patchfileName[MAX_FILE_NAME_LENGTH + 1];
          strncpy(patchfileName,(*selectedList)[selectedFile].data(), MAX_FILE_NAME_LENGTH);
          strncpy(userButtons[*selectedUserBtn].buttonPatch,patchList[selectedFile].data(),MAX_FILE_NAME_LENGTH);
          userButtons[*selectedUserBtn].buttonPatch[MAX_FILE_NAME_LENGTH-1] = '\0'; 
          
          patchfileName[MAX_FILE_NAME_LENGTH] = '\0';
          strncpy(selectedButtonChosenPatch,patchfileName,MAX_FILE_NAME_LENGTH);
          selectedButtonChosenPatch[MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null-termination
          Serial.println(selectedButtonChosenPatch);  //for trblshooting
          currentScreen = USERBUTTONFUNCTIONS;
          screenSwitch();
          return;
          }
          break;
          case USERBUTTONCHOOSESESEQUENCE:{
            strncpy(userButtons[*selectedUserBtn].buttonSequence,sequenceList[selectedFile].data(),MAX_FILE_NAME_LENGTH);
            userButtons[*selectedUserBtn].buttonSequence[MAX_FILE_NAME_LENGTH-1] = '\0'; 
            currentScreen = USERBUTTONFUNCTIONS;
            screenSwitch();
          }
          break;
          case TRIGGERCHOOSESEQUENCE:{
            strncpy(triggerList[selectedTriggerIndex].triggerSequence, sequenceList[selectedTriggerIndex +1].data(), sizeof(triggerList[selectedTriggerIndex].triggerSequence));
            triggerList[selectedTriggerIndex].triggerSequence[sizeof(triggerList[selectedTriggerIndex].triggerSequence) - 1] = '\0';
            triggerList[selectedTriggerIndex].patch = selectedFile;  
            currentScreen = TRIGGERFUNCTIONS;
            screenSwitch();
          }   
          break;
          

      }
        return;  
      }
      break;
    }
    case backBtnPressed:{
    switch(currentScreen){
      case USERBUTTONCHOOSEPATCH:{
        currentScreen = USERBUTTONFUNCTIONS;
        screenSwitch();
        return;
      }
      break;
      case USERBUTTONCHOOSESESEQUENCE:{
          currentScreen = USERBUTTONFUNCTIONS;
          screenSwitch();
          return;
          }
          break;
          case TRIGGERCHOOSESEQUENCE:{
          currentScreen = TRIGGERFUNCTIONS;
          screenSwitch();
          return;  
          }
          break;

    }
  
 
  
  break;
}

    case downBtnPressed:{
      if (selection < selectionMaximum) {
        selectedFile++;
      switch(currentScreen){
        case USERBUTTONCHOOSEPATCH:
        selectedPatchFile = selectedFile;
        break; 
      }
      selection++;
      
      DS = false;
      return;
      }
      
    if (selection == selectionMaximum){
      if (selectedFile < *selectedListFileCount - 1){ //-1 because its zero indexed
      selectedFile++;
      
      scrollDisplayedFileListDown(*selectedList, selectedDisplayedNames,selectedFile);
            switch(currentScreen){
        case USERBUTTONCHOOSEPATCH:
        selectedPatchFile = selectedFile;
        break; 
 
      }
      
      DS = false;
    }
    return;
    }
     break;
  }
  case upBtnPressed:{
  if (selection > selectionMinimum){
    selectedFile--;
      switch(currentScreen){
        case USERBUTTONCHOOSEPATCH:
        selectedPatchFile = selectedFile;
        break; 

      }
      selection--;
      
      DS = false;
      return;
  }
  
  if (selection == 0 && selectedFile > selectionMinimum){
    selectedFile--;
          switch(currentScreen){
        case USERBUTTONCHOOSEPATCH:
        selectedPatchFile = selectedFile;
        break; 
 
      }
      
      scrollDisplayedFileListUp(*selectedList, selectedDisplayedNames,selectedFile);
       
       DS = false; 
       return;
  }
   break;
}

  }
}
void handleXYNavigation(byte &x, byte &y, byte xMax, byte yMax){
 //this makes a lot of sense for non scrolling screens with x and y coordinates. 
  switch(buttonPressed){
    case downBtnPressed:{
      if (y < yMax){
        y++;
        DS = false;
        return;
      }
    }
    break;
    case upBtnPressed:{
      if (y > 0){
        y--;
        DS = false;
        return;
      }
    }
    break;
    case leftBtnPressed:{
      if (x > 0){
        x--;
        DS = false;
        return;
      }
    }
    break;
    case rightBtnPressed:{
      if (x < xMax){
        x++;
        DS = false;
        return;
      }
    }
    break;
  }
}
void handleNavigationForOverWrite(){
  switch(buttonPressed){
  case yesBtnPressed:{
    overwriteFile(selectedList,selectedDisplayedNames[selectedFile]);
    currentScreen = previousScreen;
    screenSwitch();
    return;
    break;
  }
  case noBtnPressed:
  case backBtnPressed:
  case menuBtnPressed:{
    currentScreen = previousScreen;
    screenSwitch();
    return;
    break;
  }
  }
}

void handleNavigationForLfo() {
  switch(buttonPressed){
    case downBtnPressed:
      if (selection == 0){
      selection++;
      DS = false;
      return;
    }
    break;
      case backBtnPressed:{
      gotoMainMenu();
      return;
        }
      break;
    
    case upBtnPressed:
      if (selection == 1){
        selection = 0;
        DS = false;
        return;
      }
    
    break;
    case leftBtnPressed:
      if (selection == 1){
        selection =0;
        DS = false;
        return;
      }
      if (selection > 1){
        selection--;
        DS = false;
        return;
      }
    
    break;
    case rightBtnPressed:
      if (selection == 1 || selection == 2 || selection == 3){
        selection++;
        DS = false;
        return;
      }
      if (selection == 4){
        selection = 0;
        DS = false;
        navBtnPressed = true;
        return;
      }
    
    break;
  }
}

void handleNavigationForMonitor(){

encNav(encState);
  switch(buttonPressed){
    case backBtnPressed:{
      
      gotoMainMenu();
      return;
      break;
    }
    case downBtnPressed:{
    
    if (selection < selectionMaximum){
      selection++;
      selectedDeviceParam++;
      DS = false;
      return;
    }
    if (selection == selectionMaximum && selectedDeviceParam < deviceParams.size() -1){ //-1 because its zero indexed
      selectedDeviceParam++;
      scrollMonitorDown(selectedDeviceParam);
      DS = false;
      return;
    }
    break;
    }
    case upBtnPressed:{
    
    if (selection > selectionMinimum){
      selection--;
      selectedDeviceParam--;
      DS = false;
      return;
    }
    if (selection == selectionMinimum && selectedDeviceParam > 0){
      selectedDeviceParam--;
      scrollMonitorUp(selectedDeviceParam);
      DS = false;
    }
    break;
    }
  }
}

void handleNavigationForMidiOutChannel(){
encNav(encState);
  switch(buttonPressed){
    case downBtnPressed:{
      if (selection < selectionMaximum){
        selectedMidiOutChannel++;
        selection++;
        DS = false;
        return;
      }
      
      
      if (selection == selectionMaximum && selectedMidiOutChannel < 10){
        selectedMidiOutChannel++;
        scrollListDown(selectedMidiOutChannel,displayedListEntries, midiOutChannelNames);  
        scrollListDown(selectedMidiOutChannel,displayedMidiOutChannels, midiChannels,true);
        DS = false;
        return;
      }
    }
    break;
    case upBtnPressed:{
      if (selection > selectionMinimum){
        selectedMidiOutChannel--;
        selection--;
        DS = false;
         return;
      }
      
      if (selection == selectionMinimum && selectedMidiOutChannel > 0){
        selectedMidiOutChannel--;
        scrollListUp(selectedMidiOutChannel,displayedListEntries, midiOutChannelNames);  
        scrollListUp(selectedMidiOutChannel,displayedMidiOutChannels, midiChannels,true);
        DS = false;
        return;
      }
      
    }
    break;
    case backBtnPressed:{
      currentScreen = SETTINGS;
      screenSwitch();
      return;
      break;
    }

  }
}

void handleNavigationForApp(){
    switch (encState) {
    case increased:
      if(!tempoBarVisible){
      buttonPressed = downBtnPressed;
      }
      break;
    case decreased:
      if (!tempoBarVisible){
      buttonPressed = upBtnPressed;
      }
      break;
  }
  switch(buttonPressed){
    case downBtnPressed:{
      if (selection < selectionMaximum){
        
        selection++;
        DS = false;
      }
      return;
      break;
    }
    case upBtnPressed:{
      if (selection > selectionMinimum){
        
        selection--;
        DS = false;
      }
      return;
      break;
    }
    case backBtnPressed:{
      if (currentScreen == CLOCKSOURCE){
        currentScreen = SETTINGS;
        screenSwitch();
        return;
      }
      if(currentScreen == ARPEGGIATOROPTIONS){
        currentScreen = ARPEGGIATOR;
        screenSwitch();
        return;
      }else if(currentScreen == MIDITHRU){
        currentScreen = SETTINGS;
        screenSwitch();
        return;
      }else{
      gotoMainMenu();
      return;
    }
    }
      break;
    }

}


void handleNavigationForSaveNewFile(){

  switch(buttonPressed){
    case backBtnPressed:{
      
      switch(previousScreen){
      case PATCHES:{
        currentScreen = PATCHES;
        break;
      }

    
      
    }
    screenSwitch();
    
    break;
  }
      
    case rightBtnPressed:{
    if (selection < selectionMaximum){
    if (newFileNameArrayIndexes[selection + 1] != 0){
      alphabetIndex = newFileNameArrayIndexes[selection + 1];
    }else if (newFileNameArrayIndexes[selection + 1] == 0){
      alphabetIndex = 0;
      newFileNameArray[selection + 1] = alphabet[alphabetIndex];
    }
      
      selection++;
      DS = false;
      return;
    }
    break;
    }
    case leftBtnPressed:{

    if (selection > selectionMinimum){
    if (newFileNameArrayIndexes[selection - 1] != 0){
      alphabetIndex = newFileNameArrayIndexes[selection - 1];
    }
      
      selection--;
      DS = false;
      return;
    }
    break;
    }
  case yesBtnPressed:{
    
    saveNewFile();
    sortVector(selectedList);
    initializeDisplayedFileNames(*selectedList,selectedDisplayedNames);
    currentScreen = previousScreen;
    screenSwitch();
    return;
    break;
  }
  }
}

void handleNavigationForDelete(){
  switch(buttonPressed){

    case noBtnPressed:{
      
      currentScreen = previousScreen;
      DS = false;
      break;
    }
    case backBtnPressed:{
      
      currentScreen = previousScreen;
      DS = false;
      break;
    }
  }
}


void menuNavCommands(int* selectedListIndex){
  switch(buttonPressed){
    case yesBtnPressed:{
            
            currentScreen = (ScreenState)(*selectedListIndex + 2);  // Cast to ScreenState type     
            screenSwitch();
            navBtnPressed = true;
             break; 
    }
    case encoderBtnPressed:{
            
            currentScreen = (ScreenState)(*selectedListIndex + 2);  // Cast to ScreenState type
            screenSwitch();
            navBtnPressed = true;
            break;
    }
  }
}
void handleNavigationForFileList(){

encNav(encState);

  switch (buttonPressed) {
    case overwriteBtnPressed:{
      if (*selectedListFileCount > 0){
      previousScreen = currentScreen;
      currentScreen = OVERWRITE;
      screenSwitch();
      return;
      }
    }
    break;
    case savenewBtnPressed:{
      
      previousScreen = currentScreen;
      currentScreen = SAVENEWFILE;
      screenSwitch();
      return;
      break;
    }
    case deleteBtnPressed:{
       if (*selectedListFileCount > 0){
       previousScreen = currentScreen;
       currentScreen = DELETE;
       screenSwitch();
       return;
       }
       break;
    }
    case yesBtnPressed:
    case sendBtnPressed:
    case encoderBtnPressed: {
      if (*selectedListFileCount == 0){
        
        displayMsg("no file to send!");
        delay(500);
        DS = false;
        return;     
      }
      if(*selectedListFileCount > 0){
        switch(currentScreen){
        case PATCHES:
        sendPatch(patchList[selectedFile].data());
        //sendPatch(*selectedList,selectedFile);
        DS = false;
        return;
        break; 
        case SEQUENCES:
        if(seqStarted){
          turnOffSeqApp();    
          loadSequence(sequenceList[selectedFile].data());
          seqPlayPosition = 0;
          seqStarted = true;
          seqAppStatus = true;
          currentScreen = SEQUENCER;
          screenSwitch();
          }else if (!seqStarted){
            loadSequence(sequenceList[selectedFile].data());
            currentScreen = SEQUENCER;
            screenSwitch();
        }
        return;
        break; 
        case REMAPLIST:
        loadRemap(*selectedList,selectedFile);
        return;
        break; 
        case TRACKERLIST:
        
        break; 
      }
        
        return;  
      }
      break;
    }
    case backBtnPressed:{
    switch(currentScreen){
      case PATCHES:{
         gotoMainMenu(); 
         return; 
      }
      break;
      case SEQUENCES:{
        currentScreen = SEQUENCER;
        screenSwitch();
        return;
      }
      break;
      case REMAPLIST:{
        currentScreen = REMAP;
        screenSwitch();
        return;
        
      }
      break;
    }
  
 
  
  break;
}

    
    break;
    
    case downBtnPressed:{
      if (selection < selectionMaximum) {
        selectedFile++;
      switch(currentScreen){
        case PATCHES:
        selectedPatchFile = selectedFile;
        break; 
        case SEQUENCES:
        selectedSequenceFile = selectedFile;
        break; 
        case REMAPLIST:
        selectedRemapFile = selectedFile;
        break; 
        case TRACKERLIST:
        selectedTrackerFile = selectedFile;
        break; 
      }
      selection++;
      
      DS = false;
      return;
      }
      
    if (selection == selectionMaximum){
      if (selectedFile < *selectedListFileCount - 1){ //-1 because its zero indexed
      selectedFile++;
      
      scrollDisplayedFileListDown(*selectedList, selectedDisplayedNames,selectedFile);
            switch(currentScreen){
        case PATCHES:
        selectedPatchFile = selectedFile;
        break; 
        case SEQUENCES:
        selectedSequenceFile = selectedFile;
        break; 
        case REMAPLIST:
        selectedRemapFile = selectedFile;
        break; 
        case TRACKERLIST:
        selectedTrackerFile = selectedFile;
        break; 
      }
      
      DS = false;
    }
    return;
    }
     break;
  }
  case upBtnPressed:{
  if (selection > selectionMinimum){
    selectedFile--;
      switch(currentScreen){
        case PATCHES:
        selectedPatchFile = selectedFile;
        break; 
        case SEQUENCES:
        selectedSequenceFile = selectedFile;
        break; 
        case REMAPLIST:
        selectedRemapFile = selectedFile;
        break; 
        case TRACKERLIST:
        selectedTrackerFile = selectedFile;
        break; 
      }
      selection--;
      
      DS = false;
      return;
  }
  
  if (selection == 0 && selectedFile > selectionMinimum){
    selectedFile--;
          switch(currentScreen){
        case PATCHES:
        selectedPatchFile = selectedFile;
        break; 
        case SEQUENCES:
        selectedSequenceFile = selectedFile;
        break; 
        case REMAPLIST:
        selectedRemapFile = selectedFile;
        break; 
        case TRACKERLIST:
        selectedTrackerFile = selectedFile;
        break; 
      }
      
      scrollDisplayedFileListUp(*selectedList, selectedDisplayedNames,selectedFile);
       
       DS = false; 
       return;
  }
   break;
}

  }
}

void handleNavForPatchSelection(){
  encNav(encState);
  switch(buttonPressed){
    case yesBtnPressed:
    case encoderBtnPressed:{
      if (*selectedListFileCount == 0){
        displayMsg("no file to send!");
        delay(500);
        DS = false;
        return;     
      }
      if(*selectedListFileCount > 0){
      switch(currentScreen){
      case MORPHPATCHES:
      selectedMorphPatch = selectedFile;
      populateMorphEnd(patchList, selectedMorphPatch);
      morphFileChosen = true;
      morphPosition = 0;
      calcMorph(numEntries, morphPosition);
      currentScreen = MORPH;
      screenSwitch();
      break;
      case TRIGGERPATCHES:
      strncpy(triggerList[selectedTriggerIndex].triggerPatch, patchList[selectedTriggerIndex +1].data(), sizeof(triggerList[selectedTriggerIndex].triggerPatch));
      triggerList[selectedTriggerIndex].triggerPatch[sizeof(triggerList[selectedTriggerIndex].triggerPatch) - 1] = '\0';
      triggerList[selectedTriggerIndex].patch = selectedFile;  
	    currentScreen = TRIGGERFUNCTIONS;
	    screenSwitch();
      break;     
      }
      }
    }
    break;

  case backBtnPressed:
  currentScreen = TRIGGERFUNCTIONS;
  screenSwitch();
  break;
  case downBtnPressed:
  if (selection < selectionMaximum){
    increaseParameterValue(&selection, &selectionMaximum);
    selectedFile++;
    DS = false;
    return;
  }
  if(selection == selectionMaximum && selectedFile < *selectedListFileCount - 1 ){
    selectedFile++;
    scrollDisplayedFileListDown(*selectedList, selectedDisplayedNames,selectedFile);
    DS = false;
    return;
  }
  break;
  case upBtnPressed:
  if (selection > selectionMinimum){
    decreaseParameterValue(&selection, &selectionMinimum);
    selectedFile--;
    DS = false;
    return;
  }
  if(selection == selectionMinimum && selectedFile > 0){
    selectedFile--;
    scrollDisplayedFileListUp(*selectedList, selectedDisplayedNames,selectedFile);
    DS = false;
    return;
  }
  break;
  }
}

void deviceSelectNavCommands() {

  switch (buttonPressed) {
    
    case yesBtnPressed: {
        chooseDevice();
        currentScreen = MENU;
        screenSwitch();
         break;
      }
    case encoderBtnPressed: {
        
        chooseDevice();
        currentScreen = MENU;
        screenSwitch();
         break;
      }

  }
}

void handleNavigationForRemap(){
 
  switch(buttonPressed){
    case backBtnPressed:{
    gotoMainMenu();
    
    return;
    }
    break;
    case yesBtnPressed:{
      if (selection == 3){
        currentScreen = REMAPLIST;
        navBtnPressed = true;
        screenSwitch();
        return;
      }
    }
    break;
    case upBtnPressed:{
      if (selection > 0){
        selection--;
        DS = false;
        return;
      }
    }
    break;
    case downBtnPressed:{
      if (selection < selectionMaximum){
        selection++;
        DS = false;
        return;
      }
    }
    break;
  }
}

void handleNavigationForSequencer(){
  if (!seqSubMenuVisible){
  switch(buttonPressed){
    case noBtnPressed:{
      if (selection < seqLength){
        previousSeqSelection = selection;
        selectedSeqSwang = &seqSwang[selection];
        currentScreen = SEQSWANG;
        screenSwitch();
      }else {
    Serial.println("Error: Selection out of bounds");
    }

    }
    break;
    case overwriteBtnPressed:{
      finishNoteEdit();
      selectedArrayEntry = selection;
      currentScreen = SEQCHORDENTRY;
      screenSwitch();
    }
    break;
    case menuBtnPressed:{
      finishNoteEdit();
      if (seqSubMenuVisible == false){
        seqSubMenuVisible = true;
        subMenuSelection = 0;
        DS = false;
        return;
      }
    }
    break;
    case downBtnPressed:{
      finishNoteEdit();
      if (selection <= 23){
        selection = selection + 8;
        DS = false;
      }
    }
    break;
    case upBtnPressed:{
      finishNoteEdit();
      if(selection > 7){
        selection = selection -8;
        DS = false;
      }
    }
    break;
    case leftBtnPressed:{
      finishNoteEdit();
      if (selection > selectionMinimum){
        selection--;
        DS = false;
      }
    }
    break;
    case rightBtnPressed:{
      finishNoteEdit();
      if (selection < selectionMaximum){
        selection++;
        DS = false;
      }
    }
    break;
    case backBtnPressed:{
      finishNoteEdit();
    gotoMainMenu();
    return;
    break;
    }

  }
}
if(seqSubMenuVisible){
  switch(buttonPressed){
    case yesBtnPressed:{
      if (subMenuSelection == 6){
        currentScreen = SEQUENCES;
        screenSwitch();
        return;
      }
    }
    break;
    case downBtnPressed:{
    if ( subMenuSelection < subMenuSelectionMax){
      subMenuSelection++;
      DS = false;
      return;
    }
    }
    break;
    case upBtnPressed:{
      if (subMenuSelection > subMenuSelectionMin){
        subMenuSelection--;
      DS = false;
      return;
      }
    }
    break;
    case menuBtnPressed:
    case backBtnPressed:{
      seqSubMenuVisible = false;
      selectedBool = nullptr;
      selectedParameterVal = nullptr;
      selectedParameterMin = nullptr;
      selectedParameterMax = nullptr;
      DS = false;
      return;
    }
    break;
  }
}

}

void handleNavForTriggers(){

  encNav(encState);

  switch (buttonPressed){
    case encoderBtnPressed:
    case yesBtnPressed:
    if (navX == 0 && triggerList[selectedTriggerIndex].index != 255){
      selectedTriggerSelectedApp = &triggerList[selectedTriggerIndex].appNum;
      currentScreen = TRIGGERFUNCTIONS;
      screenSwitch();
    }
    break;
    case backBtnPressed:
    gotoMainMenu();
    break;
    case rightBtnPressed:
    increaseParameterValue(&navX, &navXMax);
    DS = false;
    break;
    case leftBtnPressed:
    decreaseParameterValue(&navX, &navXMin);
    DS = false;
    break;
  }
  if(navX == 0 || navX == 3){
    if(triggerList.size() > 1){
    switch(buttonPressed){
      case upBtnPressed:
    if(navY > navYMin){
    decreaseParameterValue(&navY, &navYMin);
    decreaseParameterValue(&selectedTriggerIndex, &navYMin);
    DS = false;
    return;
    }

    if(navY == navYMin & selectedTriggerIndex > 0){
    decreaseParameterValue(&selectedTriggerIndex, &navYMin);
    scrollTriggerScreenUp(selectedTriggerIndex, triggerPtrs, triggerList);
    DS = false;
    return;
    }
      break;
      case downBtnPressed:
      byte selectedTriggerIndexMax = triggerList.size()-1;
      if(navY == navYMax && selectedTriggerIndex < triggerList.size()-1){  
      increaseParameterValue(&selectedTriggerIndex, &selectedTriggerIndexMax);
      scrollTriggerScreenDown(selectedTriggerIndex, triggerPtrs, triggerList);
      DS = false;
      return;
      }
      if (navY < navYMax){
      increaseParameterValue(&selectedTriggerIndex, &navYMax);
      increaseParameterValue(&navY, &navYMax);
      DS = false;
      return;
      }
      
      break;
    }
  }
  }
}

void triggerFunctionsNavCommands(){
  switch(buttonPressed){
    case backBtnPressed:
    currentScreen = TRIGGERS;
    screenSwitch();
    break;
    case yesBtnPressed:{
      if(triggerNavXY[0] == 1){
      *selectedTriggerSelectedFunction = triggerNavXY[1];
      DS = false;
      return;
      }
      if (triggerNavXY[0] == 0 && triggerNavXY[1] == 1){
          currentScreen = TRIGGERCHOOSEAPP;
          screenSwitch();
          return;
        }
    }
    break;
  }
}

void handleNavForTriggerFunctions(){
  encNav(encState);
  switch(buttonPressed){
    case encoderBtnPressed:
    case yesBtnPressed:{
    if(navX == 0){
    if(navY == 1){
      currentScreen = TRIGGERCHOOSEAPP;
      screenSwitch();
      return;
    }
    if(navY == 2){
      currentScreen = TRIGGERPATCHES;
      screenSwitch();
      return;
    }
    if(navY == 3){
      currentScreen = TRIGGERCHOOSESEQUENCE;
      //currentScreen = SEQUENCES;//this works for some reason
      screenSwitch();
      return;
      
    }
    if(navY == 4){
      triggerList[selectedTriggerIndex].panic = true;
      DS = false;
      return;
    }
    }

    }
    break;
    case backBtnPressed:
    currentScreen = TRIGGERS;
    screenSwitch();
    break;
    case downBtnPressed:
    increaseParameterValue(&navY, &navYMax);
    DS = false;
    break;
    case upBtnPressed:
      decreaseParameterValue(&navY, &navYMin);
      DS = false;
    break;
    case rightBtnPressed:
        increaseParameterValue(&navX, &navXMax);
      DS = false;
    break;
    case leftBtnPressed:
          decreaseParameterValue(&navX, &navXMin);
      DS = false;
    break;
  }
}

void triggerFunctionChooseAppNavCommands(){
switch(buttonPressed){
  case yesBtnPressed:{
    
    triggerList[selectedTriggerIndex].appNum = *selectedListIndex;
    updateChosenAppArray(triggerList[selectedTriggerIndex].appNum);
    for (byte i = 0; i < 5;i++){
      if (i != displayIndex){
strcpy(displayedListEntries2[i], "  ");
      }else{
        strcpy(displayedListEntries2[i], "X ");
      }
    }
    
    DS = false;
    return;
  }
  break;
  case backBtnPressed:{
    currentScreen = TRIGGERFUNCTIONS;
    screenSwitch();
    return;
  }
  break;

}
}
void handleNavForSeqChordEntry(){
  switch(buttonPressed){
    case leftBtnPressed:{
      decreaseParameterValue(&navX, &navXMin);
      DS = false;
      return;
    }
    break;
    case rightBtnPressed:{
      increaseParameterValue(&navX, &navXMax);
      DS = false;
      return;
    }
    break;
    case upBtnPressed:{
      decreaseParameterValue(&navY, &navYMin);
      DS = false;
      return;
    }
    break;
    case downBtnPressed:{
      increaseParameterValue(&navY, &navYMax);
      DS = false;
      return;
    }
    break;
    case backBtnPressed:{
      currentScreen = SEQUENCER;
      screenSwitch();
      return;
    }
    break;
  }
}
void handleNavForController(){
 
  switch(buttonPressed){
    case menuBtnPressed:{
      currentScreen = CONTROLLEROPTIONS;
      screenSwitch();
    }
    break;
    case encoderBtnPressed:
    case yesBtnPressed:{
      if(controllerParamsCount == 0 || *ctrlrPtrs[ctrlrY].index == 255)
      currentScreen = ADDCONTROLLERPARAM;
      screenSwitch();
      return;
    }
    break;
    case backBtnPressed:{
      gotoMainMenu();
      return;
    }
    break;
  }
  if(controllerParamsCount > 0){
  if(ctrlrX == 0){
    encNav(encState);
  }
    switch(buttonPressed){
      case leftBtnPressed:
      decreaseParameterValue(ctrlrX,0);
      DS = false;
      return;
      break;
      case rightBtnPressed:
      increaseParameterValue(ctrlrX,ctrlrXMax);
      DS = false;
      return;
      break;
      case downBtnPressed:
      if (ctrlrY < ctrlrYMax){
      increaseParameterValue(ctrlrIndex, ctrlrIndexMax);
      increaseParameterValue(ctrlrY, ctrlrIndexMax);
      DS = false;
      return;
      }
      if (ctrlrY == ctrlrYMax && ctrlrIndex < ctrlrIndexMax){
        increaseParameterValue(ctrlrIndex, ctrlrIndexMax);
        if(!ctrlrUseDeviceParams){
        scrollDispCtrlrDown(ctrlrIndex, ctrlrPtrs,ctrlrParamList);
        }
        if(ctrlrUseDeviceParams){
        scrollDispCtrlrDown(ctrlrIndex, ctrlrPtrs,deviceParams);
        }
        DS = false;
        return;
      }
      
      break;
      case upBtnPressed:
      if (ctrlrY > 0){
      decreaseParameterValue(ctrlrIndex, 0);
      decreaseParameterValue(ctrlrY, 0);        
      DS = false;
      return;
      }
      if (ctrlrY == 0 && ctrlrIndex > 0){
        decreaseParameterValue(ctrlrIndex, 0);
        if(!ctrlrUseDeviceParams){
        scrollDispCtrlrUp(ctrlrIndex, ctrlrPtrs,ctrlrParamList);
        }
        if(ctrlrUseDeviceParams){
        scrollDispCtrlrUp(ctrlrIndex, ctrlrPtrs,deviceParams);
        }
        DS = false;
        return;
      }  
      break;
    }
  }

}

void handleNavForAddControllerParam(){

  switch(buttonPressed){
    case leftBtnPressed:
    if(displayIndex >=1 && displayIndex <=14){
      decreaseParameterValue(&displayIndex, &displayIndexMin);
      DS = false;
    }
    break;
    case rightBtnPressed:
    if(displayIndex >=1 && displayIndex <=14){
      increaseParameterValue(&displayIndex, &displayIndexMax);
      DS = false;
      if(tempCcNameAlphaIndex[displayIndex-1] == 255){
        tempCcNameAlphaIndex[displayIndex-1] = 0;
      }      
    }
    break;
    case downBtnPressed:{
      if(displayIndex >= 1 && displayIndex <=14){
        displayIndex = 15;
        DS = false; 
        return;
      }else{

      increaseParameterValue(&displayIndex, &displayIndexMax);
      DS = false;
      if(tempCcNameAlphaIndex[displayIndex-1] == 255){
        tempCcNameAlphaIndex[displayIndex-1] = 0;
      }
      return;
      }
         
    }
    break;
    
    case upBtnPressed:{
      if(displayIndex == 15){
        displayIndex = 1;
        DS = false;
        return;
      }
      if(displayIndex >= 1 && displayIndex <=14){
        displayIndex = 0;
        DS = false; 
        return;
      }else{
      decreaseParameterValue(&displayIndex, &displayIndexMin);
      DS = false;
      return;
      }
      
    }
    break;
    case yesBtnPressed:{
      if (displayIndex == 15){
        currentScreen = CONTROLLER;
        screenSwitch();
      }

    }
    break;
    case backBtnPressed:{
    currentScreen = CONTROLLER;
    screenSwitch();
    }
   break;
  }
  
}
void handleNavForControllerOptions(){
switch(buttonPressed){
  case downBtnPressed:
  increaseParameterValue(navY,navYMax);
  break;
  case upBtnPressed:
  decreaseParameterValue(navY,navYMin);
  break;
  case backBtnPressed:
  currentScreen = CONTROLLER;
  screenSwitch();
  break;
}
}
void handleNavigationForMorph(){
  switch(buttonPressed){

    case yesBtnPressed:
      if (navY == 0){
        currentScreen = MORPHPATCHES;
        for(byte i = 0; i < numEntries; i++){
        if(deviceParams[i].nrpn == false && deviceParams[i].morphCalc != 255 && deviceParams[i].ccVal != deviceParams[i].morphCalc){
          deviceParams[i].ccVal = deviceParams[i].morphCalc;
        }
        }
        screenSwitch();
        return;
      }
      if (navY == 1){
        morphPosition = 0;
        if(morphFileChosen){
          resetMorph();
          previousMorphPosition = morphPosition;
          morphTimerStarted = false;
          DS = false;
          return;
        }
        DS = false;
        return;
      }
    break;

    case downBtnPressed:
      increaseParameterValue(navY,navYMax);
      DS = false;
    break;

    case upBtnPressed:
      decreaseParameterValue(navY,navYMin);
      DS = false;
    break;

    case backBtnPressed:
      for(byte i = 0; i < numEntries; i++){
        if(deviceParams[i].nrpn == false && deviceParams[i].morphCalc != 255 && deviceParams[i].ccVal != deviceParams[i].morphCalc){
          deviceParams[i].ccVal = deviceParams[i].morphCalc;
        }
      }
      gotoMainMenu();
    break;
  
  }
}

void handleNavForSeqSwang(){
  switch(buttonPressed){
    case yesBtnPressed:
    case backBtnPressed:
    currentScreen = SEQUENCER;
    screenSwitch();
    navBtnPressed = true;
    break;
  }
}
void extraArpeggiatorNavOptions(){
  switch(buttonPressed){
    case menuBtnPressed:
    currentScreen = ARPEGGIATOROPTIONS;
    screenSwitch();
    break;
  }
}

void handleNavForClockSource(){
switch(buttonPressed){
  case upBtnPressed:
  decreaseParameterValue(&selection, &selectionMinimum);
  DS = false;
  break;
  case downBtnPressed:
  increaseParameterValue(&selection, &selectionMaximum);
  DS = false;
  break;
  case backBtnPressed:
  currentScreen = SETTINGS;
  screenSwitch();
  break;
}
}


void handleArpTranspose(byte buttonPressed){
switch(buttonPressed){
  case leftBtnPressed:
    if(!minArpTransp){
      arpTransState = 0;
      return;
    }
  break;

  case rightBtnPressed:
    if(!maxArpTransp){
      arpTransState = 2;
      return;
    }
  break;
}
}
void handleNavForSystemInfo(){
  switch(buttonPressed){
    case backBtnPressed:{
      currentScreen = SETTINGS;
      screenSwitch();
      return;
    }
    break;
  }
}
void handleNavigation() {
  switch (currentScreen) {
   
    case DEVICE_SELECT:{
      encNav(encState);   
      genericListNav(selectedListIndex,displayedListEntries, deviceNames);      
      deviceSelectNavCommands();
    }
      break;
    
    case MENU:{
      // Navigation logic for MENU screen
      encNav(encState);   
      genericListNav(selectedListIndex,displayedListEntries, appNames);
      menuNavCommands(selectedListIndex);
      //handleNavigationForMenu();
    }
      break;
    
    case PATCHES:{
      // Navigation logic for PATCHES screen
      handleNavigationForFileList();
    }
      break;
    case TRIGGERPATCHES:{
      handleNavForPatchSelection();
      break;
    }
    
    case DELETE:{
      // Navigation logic for PATCHES screen
      handleNavigationForDelete();
    }
      break;
    
    case SAVENEWFILE:{
      // Navigation logic for PATCHES screen
      handleNavigationForSaveNewFile();
    }
      break;
    
    case ARPEGGIATOR:{
    handleNavigationForApp();
    extraArpeggiatorNavOptions();
    }
    break;
    case ARPEGGIATOROPTIONS:{
      handleNavigationForApp();
      if(selection == 0 && arpLatch && arpStarted){
      handleArpTranspose(buttonPressed);
      }
    }
    break;
    
    case ARPNOTE:{
    handleNavigationForApp();
    }
    break;
    
    case REMAP:{
    handleNavigationForRemap();
    }
    break;
    case REMAPLIST:{
    handleNavigationForFileList();
    }
    break;
    case AUTOCHORD:{
    handleNavigationForApp();
    }
    break;
    
    case MONITOR:{
    handleNavigationForMonitor();
    }
    break;
    
    case LFO:{
    handleNavigationForLfo();
    }
    break;
    case SEQUENCER:{
      handleNavigationForSequencer();
    }
    break;
    case OVERWRITE:{
      handleNavigationForOverWrite();
    }
    break;
    case SEQUENCES:{
      handleNavigationForFileList();
    }
    break;
    case SETTINGS:{
      encNav(encState);   
      genericListNav(selectedListIndex,displayedListEntries, settingsList);      
      settingsNavCommands(selectedListIndex);
    }
    break;
    case MIDIOUTCHANNELS:{
      handleNavigationForMidiOutChannel();
    }
    break;
    case USERBUTTONSELECT:
      encNav(encState);   
      genericListNav(selectedListIndex,displayedListEntries, userButtonNames);      
      userBtnSelectNavCommands(selectedListIndex);
    break;
    case USERBUTTONFUNCTIONS:
    encNav(encState);   
    genericListNav(selectedListIndex,displayedListEntries, userButtonNames);   
    userButtonFunctionsNavCommands();
    break;
    case USERBUTTONCHOOSEAPP:
      encNav(encState);   
      genericListsNav(selectedListIndex,displayedListEntries, toggleableAppNames,displayedListEntries2, chosenAppArray );    
      userBtnChooseAppNavCommands();
    break;
    case USERBUTTONCHOOSEPATCH:
    encNav(encState);
    handleNavForFileListLimited();
    break;
    case TRIGGERS:
    handleNavForTriggers();
    break;
    case TRIGGERFUNCTIONS:
    handleNavForTriggerFunctions();
    break;
    case TRIGGERCHOOSEAPP:
    encNav(encState);
    genericListsNav(selectedListIndex,displayedListEntries, toggleableAppNames,displayedListEntries2, chosenAppArray );
    triggerFunctionChooseAppNavCommands();
    break;
    case SEQCHORDENTRY:
    handleNavForSeqChordEntry();
    break;
    case CONTROLLER:
     handleNavForController();
    break;
    case ADDCONTROLLERPARAM:
    handleNavForAddControllerParam();
    break;
    case CONTROLLEROPTIONS:
    handleNavForControllerOptions();
    break;
    case MORPH:
    handleNavigationForMorph();
    break;
    case MORPHPATCHES:
    handleNavForPatchSelection();
    break;
    case SEQSWANG:
    handleNavForSeqSwang();
    break;
    case MIDITHRU:
    handleNavigationForApp();
    break;
    case CLOCKSOURCE:
    handleNavForClockSource();
    break;
    case SYSTEMINFO:
    handleNavForSystemInfo();
    break;
    case USERBUTTONCHOOSESESEQUENCE:
    encNav(encState);
    handleNavForFileListLimited();
    break;
    case TRIGGERCHOOSESEQUENCE:
    encNav(encState);
    handleNavForFileListLimited();
    break;
    
  }
}
