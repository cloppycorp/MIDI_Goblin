#include "i_initializers.h"



void insertTriggerEntry(){
    if (triggerList.size() > 0) {
      TriggerStruct newEntry = {255,"XXX",0,0,false,0,0,999,999,false,"NOTHING","NOTHING"};
      uint8_t tempTriggerIndex = triggerList.size();
      char tempTriggerIndexChar[4];
      snprintf(tempTriggerIndexChar, sizeof(tempTriggerIndexChar), "%d", tempTriggerIndex);
      char tempTriggerName[10];
      strcpy(tempTriggerName, "TRIG_");
       strcat(tempTriggerName, tempTriggerIndexChar);
       tempTriggerName[10] = '\0';
       newEntry.index = tempTriggerIndex;
       strcpy(newEntry.name, tempTriggerName);
      
       
        // Insert the new entry before the last "END" entry
        triggerList.insert(triggerList.end() - 1, newEntry);
    } else {
        // Create the "END" entry
        initTriggerList();
        // Add the new entry
       
    }
}

void initTriggerPtrs(){
  if(triggerPtrs.size() == 0){
  for (byte i = 0; i < 5; i++){
    DispTriggerStruct newEntry = {nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr,nullptr};
    triggerPtrs.push_back(newEntry);  
 
  }
}
}

void initTriggerList(){

 TriggerStruct endEntry = {255,"END",255,255,false,255,255,999,999,false};
  triggerList.push_back(endEntry);
}

void populateTriggerPtrs(){
  byte listSize = triggerList.size();
  if (listSize < 5){
  for (byte i = 0; i < listSize; i++){
    triggerPtrs[i].index = &triggerList[i].index;
    triggerPtrs[i].name = triggerList[i].name;
    triggerPtrs[i].ccNum = &triggerList[i].ccNum;
    triggerPtrs[i].ccVal = &triggerList[i].ccVal;
    triggerPtrs[i].status = &triggerList[i].status;
    triggerPtrs[i].appNum = &triggerList[i].appNum;
    triggerPtrs[i].patch = &triggerList[i].patch;
    triggerPtrs[i].sequence = &triggerList[i].sequence;
    triggerPtrs[i].panic = &triggerList[i].panic;
  }
  return;
  }
  if (listSize >=5){
  for (byte i = 0; i < 5; i++){
    triggerPtrs[i].index = &triggerList[i].index;
    triggerPtrs[i].name = triggerList[i].name;
    triggerPtrs[i].ccNum = &triggerList[i].ccNum;
    triggerPtrs[i].ccVal = &triggerList[i].ccVal;
    triggerPtrs[i].status = &triggerList[i].status;
    triggerPtrs[i].appNum = &triggerList[i].appNum;
    triggerPtrs[i].patch = &triggerList[i].patch;
    triggerPtrs[i].sequence = &triggerList[i].sequence;
    triggerPtrs[i].panic = &triggerList[i].panic;
  }    
  }
}

void resetCtrlrDispPtrs(){
  for (byte i = 0; i<5;i++){
    ctrlrPtrs[i].index = nullptr;
    ctrlrPtrs[i].ccName = NULL;
    ctrlrPtrs[i].ccNum = nullptr;
    ctrlrPtrs[i].ccVal = nullptr;
    ctrlrPtrs[i].obtainedVal = nullptr;
  }
}

void initCtrlrDispPtrs(){
  
  for (byte i = 0; i<5;i++){
    CtrlrDispStruct newEntry;
    newEntry.index = nullptr;
    newEntry.ccName = NULL;
    newEntry.ccNum = nullptr;
    newEntry.ccVal = nullptr;
    newEntry.obtainedVal = nullptr;
    ctrlrPtrs.push_back(newEntry);
  }
}



void populateDispPtrs(std::vector<CtrlrDispStruct>& destination,std::vector<CtrlrStruct>& source ){
byte sourceSize = source.size();
if(sourceSize < 5){
  for (byte i = 0; i < sourceSize; i++){
    destination[i].index = &source[i].index;
    destination[i].ccName = source[i].ccName;
    destination[i].ccNum = &source[i].ccNum;
    destination[i].ccVal = &source[i].ccVal;
  }
  
}
if (sourceSize >= 5){
  for (byte i = 0; i < 5; i++){
    destination[i].index = &source[i].index;
    destination[i].ccName = source[i].ccName;
    destination[i].ccNum = &source[i].ccNum;
    destination[i].ccVal = &source[i].ccVal;
  }
}
}


void populateDispPtrs(std::vector<CtrlrDispStruct>& destination,std::vector<DeviceParamsStruct>& source ){
byte sourceSize = source.size();
if(sourceSize < 5){
  for (byte i = 0; i < sourceSize; i++){
    destination[i].index = &source[i].index;
    destination[i].ccName = source[i].name;
    destination[i].ccNum = &source[i].ccNum;
    destination[i].ccVal = &source[i].ccVal;
    destination[i].obtainedVal = &source[i].obtained;
  }
  
}
if (sourceSize >= 5){
  for (byte i = 0; i < 5; i++){
    destination[i].index = &source[i].index;
    destination[i].ccName = source[i].name;
    destination[i].ccNum = &source[i].ccNum;
    destination[i].ccVal = &source[i].ccVal;
    destination[i].obtainedVal = &source[i].obtained;
  }
}
}

//duh. just use pointers and print the pointers to the screen lol 
void initDispdCharsPtrs(char* dispdCharPtrs[5], std::vector<CharArray>& list){
  int vectorSize = list.size();
  
  if (vectorSize < 5){
    for (byte i = 0; i < vectorSize;i++){
    dispdCharPtrs[i] = list[i].data();  
    }
    return;
  }else{
  for(byte i = 0; i < 5; i++){
    dispdCharPtrs[i] = list[i].data();
  }
  }
}

void initDispdBytePtrs(byte* dispdBytePtrs[5], std::vector<uint8_t>& list){
  int vectorSize = list.size();
  byte emptyCells = 5;
  byte filledCells = 0;
  if (vectorSize < 5){
    for (byte i = 0; i < vectorSize;i++){
    dispdBytePtrs[i] = &list[i];  
    emptyCells--;
    filledCells++;
    }
    for(byte i = filledCells; i<emptyCells;i++){

    dispdBytePtrs[i] = nullptr;
    }
    return;
  }else{
  for(byte i = 0; i < 5; i++){
    dispdBytePtrs[i] = &list[i];
  }
  }
}


void initSelectedButtonChosenAppArray(){
    for (byte i = 0; i< 6;i++){
      if(i != *selectedButtonAppToToggle){
      selectedButtonChosenAppArray[i] = ' ';  
      }else{
      selectedButtonChosenAppArray[i] = 'X';  
      }
    }
  
 
}



void linkMidiIndexToDeviceParams() {
    // First, initialize all entries of midiIndex to nullptr
    for (byte i = 0; i < 128; i++) {
        midiIndex[i] = nullptr;  // Ensure all pointers start as nullptr
    }

    // Loop through the deviceParams vector
    for (int i = 0; i < deviceParams.size(); i++) {
        byte ccNum = deviceParams[i].ccNum;  // Get the ccNum of the current entry

        // Ensure ccNum is within the valid MIDI CC range (0-127)
        if (ccNum < 128) {
            // Assign the pointer in midiIndex[ccNum] to point to the current DeviceParamsStruct
            midiIndex[ccNum] = &deviceParams[i];
        }
    }
    for(byte i = 0; i < 128; i++){
      Serial.print(i);
      Serial.print(" ");
      if(midiIndex[i] != nullptr){
        byte tempNum = midiIndex[i]->ccNum;
        byte tempindex = midiIndex[i]->index;
      Serial.print(tempNum);
      Serial.print(" ");
      Serial.println(tempindex);
      }else{
        Serial.println("null");
      }
    }
}


void initiateDisplayedCharArray(){
for (byte i=0;i<5;i++){
  
  displayedCharListEntries[i] = selectedButtonChosenAppArray[i];
}
  
 
}




void updateChosenAppArray(int* selectedButtonAppToToggle) {
  for (byte i = 0; i < sizeof(chosenAppArray) / sizeof(chosenAppArray[0]); i++) {
    if (i != *selectedButtonAppToToggle) {
      strcpy(chosenAppArray[i], "  ");
    } else {
      strcpy(chosenAppArray[i], "X ");
    }
  }
}
void updateChosenAppArray(byte selectedButtonAppToToggle) {
  for (byte i = 0; i < sizeof(chosenAppArray) / sizeof(chosenAppArray[0]); i++) {
    if (i != selectedButtonAppToToggle) {
      strcpy(chosenAppArray[i], "  ");
    } else {
      strcpy(chosenAppArray[i], "X ");
    }
  }
}

void updateChosenAppArray(byte* selectedTriggerSelectedApp) {
  for (byte i = 0; i < sizeof(chosenAppArray) / sizeof(chosenAppArray[0]); i++) {
    if (i != *selectedTriggerSelectedApp) {
      strcpy(chosenAppArray[i], "  ");
    } else {
      strcpy(chosenAppArray[i], "X ");
    }
  }
}



void initDeviceMonitor(){
byte tmpListSize = deviceParams.size();

  if(tmpListSize == 5 || tmpListSize > 5){
    for (byte i = 0; i < 5; i++) {
      strcpy(deviceMonitorNames[i], deviceParams[i].name);
      deviceMonitorObtained[i] = &deviceParams[i].obtained;
      if(deviceParams[i].nrpn == false){
      deviceMonitorByteColumnA[i] = &deviceParams[i].ccNum;
      deviceMonitorByteColumnB[i] = &deviceParams[i].ccVal;
      }
      if(deviceParams[i].nrpn == true){
      deviceMonitorByteColumnA[i] = &deviceParams[i].msbVal;
      deviceMonitorByteColumnB[i] = &deviceParams[i].lsbVal;
      }
      
    }  
  }
  if(tmpListSize < 5){
    for(byte i = 0; i < tmpListSize - 1;i++){
      strcpy(deviceMonitorNames[i], deviceParams[i].name);
      deviceMonitorObtained[i] = &deviceParams[i].obtained;
      
      if(deviceParams[i].nrpn == false){
      deviceMonitorByteColumnA[i] = &deviceParams[i].ccNum;
      deviceMonitorByteColumnB[i] = &deviceParams[i].ccVal;
      }
      if(deviceParams[i].nrpn == true){
      deviceMonitorByteColumnA[i] = &deviceParams[i].msbVal;
      deviceMonitorByteColumnB[i] = &deviceParams[i].lsbVal;
      }
      
    }
  }
}

void initiateDisplayedMidiOutChannels(){
  for (byte i = 0; i <5; i++){
    displayedMidiOutChannels[i] = *midiChannels[i];
  }
}



void initializeDisplayedFileNames(const std::vector<CharArray>& list, char displayedNames[][MAX_FILE_NAME_LENGTH + 1]) {
    int listCount = list.size(); // Get the size of the list
    if (listCount == 0) {
        // If there are no files, set the first entry to a special message and clear the rest.
        strcpy(displayedNames[0], "No files (yet)");
        for (int i = 1; i < 5; i++) {
            memset(displayedNames[i], ' ', MAX_FILE_NAME_LENGTH);  // Fill with spaces
            displayedNames[i][MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null termination
        }
        return;
    }

    for (int i = 0; i < 5; i++) {
        if (i < listCount) {
            char fileName[MAX_FILE_NAME_LENGTH + 1];
            strncpy(fileName, list[i].data(), MAX_FILE_NAME_LENGTH);
            fileName[MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null termination

            // Remove the file extension (assuming it's always ".txt")
            char* dotPosition = strchr(fileName, '.');
            if (dotPosition != nullptr) {
                *dotPosition = '\0';  // Truncate the string at the position of the dot
            }

            strcpy(displayedNames[i], fileName);
        } else {
            // If there are fewer items than display slots, fill the remaining displayed names with spaces
            memset(displayedNames[i], ' ', MAX_FILE_NAME_LENGTH);  // Fill with spaces
            displayedNames[i][MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null termination
        }
    }
}



void defineNewFileNameGrid() {
for (byte i = 0; i < sizeof(newFileNameGrid); i++){
  newFileNameGrid[i] = {i*6, 25};
}
}



void defineTableColumnA(){
  tableColumnA[0] = {0,0};
  tableColumnA[1] = {0,0};
  tableColumnA[2] = {0,0};
  tableColumnA[3] = {0,0};
  tableColumnA[4] = {0,0};
}

void defineTableColumnB(){
  tableColumnB[0] = {0,0};
  tableColumnB[1] = {0,0};
  tableColumnB[2] = {0,0};
  tableColumnB[3] = {0,0};
  tableColumnB[4] = {0,0};
}
// For byte type
void valueMinMax(byte &valueName, byte valueValue, byte &minValueName, byte minValueValue, byte &maxValueName, byte maxValueValue) {
    valueName = valueValue;
    minValueName = minValueValue;
    maxValueName = maxValueValue;
}

// For int type
void valueMinMax(int &valueName, int valueValue, int &minValueName, int minValueValue, int &maxValueName, int maxValueValue) {
    valueName = valueValue;
    minValueName = minValueValue;
    maxValueName = maxValueValue;
}

// For unsigned long type
void valueMinMax(unsigned long &valueName, unsigned long valueValue, unsigned long &minValueName, unsigned long minValueValue, unsigned long &maxValueName, unsigned long maxValueValue) {
    valueName = valueValue;
    minValueName = minValueValue;
    maxValueName = maxValueValue;
}

void initializeNewFileNameArray() {
    for (byte i = 0; i < MAX_FILE_NAME_LENGTH; i++) {
        newFileNameArray[i] = '-'; // Initialize each element with a space
        newFileNameArrayIndexes[i] = 0;
    }
    newFileNameArray[MAX_FILE_NAME_LENGTH] = '\0'; // Null-terminate the string
}
