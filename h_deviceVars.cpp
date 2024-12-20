#include "h_deviceVars.h"

byte seqClockMode = 1; //experimental clock syncing for sequencer
bool DS;

bool patchLoaded = false;
int dynamicMaxFileNameLength = 20;  // Start with a default value
//device variables
byte numEntries = 0;
byte* deviceCcNums = nullptr;

byte (*deviceMsbNums) = nullptr;
byte (*deviceLsbNums) = nullptr;
byte (*deviceCcValues) = nullptr;
byte (*deviceMsbValues) = nullptr;
byte (*deviceLsbValues) = nullptr;
byte (*panelCcValues) = nullptr;
byte (*panelMsbValues) = nullptr;
byte (*panelLsbValues) = nullptr;

byte* selectedUserBtn = nullptr;
byte* selectedUserBtnFnctn = nullptr;
byte* selectedUserBtnApp = nullptr;
bool* selectedUserBtnPanic = nullptr;
char* selectedUserBtnPatch = nullptr;
char* selectedUserBtnSequence = nullptr;


char (*deviceCcNames)[15] = nullptr;
const char* goblinSsid = "MIDIGOBLIN";
const char* goblinPassword = "Gr33nGobL1n";
IPAddress local_IP(192, 168, 0, 1);       // Set ESP32 AP IP
IPAddress gateway(192, 168, 0, 1);        // Set gateway to the same as local IP in AP mode
IPAddress subnet(255, 255, 255, 0);       // Subnet mask
unsigned int patchFileCapacity = 0;
unsigned int sequenceFileCapacity = 0;
unsigned int reMapFileCapacity = 0;
unsigned int trackerFileCapacity = 0;
unsigned int numberOfDevicePatchFiles = 0;
unsigned int numberOfDeviceSequenceFiles = 0;
const char panelRule[MAX_FILE_NAME_LENGTH + 1] = "PANEL---------------";
const char panelRule2[10] = "PANEL.txt";
char buttonFunctions[][MAX_FILE_NAME_LENGTH + 1] = {
"NOTHING",
"SEND PATCH",
"LOAD SEQUENCE",
"APP ON/OFF",
"SYNC",
"PANIC" 
};
char userButtonNames[3][7]{
  "USER 1",
  "USER 2",
  "USER 3"
};
char functionNames[5][MAX_FILE_NAME_LENGTH + 1] = {
  "NOTHING",
  "TOGGLE APP ON/OFF",
  "SEND PATCH",
  "LOAD SEQUENCE",
  "PANIC",
};

int selectedUserButtonIndex;
int patchFileCount = 0;
int sequenceFileCount = 0;
int reMapFileCount = 0;
int trackerFileCount = 0;
bool checkFileName(const char* newFileNameArray) {
    // Compare the first MAX_FILE_NAME_LENGTH characters
    return memcmp(newFileNameArray, panelRule, MAX_FILE_NAME_LENGTH) == 0;
    
}
bool checkFileName2(const char* selectedFileName) {
    // Compare the first MAX_FILE_NAME_LENGTH characters
    return memcmp(selectedFileName, panelRule2, 10) == 0;
}

unsigned long messageStartTime;
unsigned long messageTimeout;
bool deviceHasNrpn = false;
bool nrpnInOrder = false;
int functionIndex = 0;
byte button1ChosenFunction = 0;
byte button2ChosenFunction = 0;
byte button3ChosenFunction = 0;
bool* button1ChosenApp = nullptr;
bool* button2ChosenApp = nullptr;
bool* button3ChosenApp = nullptr;
int* selectedButtonAppToToggle = nullptr;
int button1AppToToggle = 0;
int button2AppToToggle = 0;
int button3AppToToggle = 0;
int toggleableAppIndex = 0;
byte toggleableAppIndexMax = 5;
byte* selectedButtonSelectedFunction = nullptr;
bool functionSelectionLeftRight;
char displayedToggleAbleAppNames[5][21];
char tableColumnA_0[10] = " ";
char tableColumnA_1[10] = " ";
char tableColumnA_2[10] = " ";
char tableColumnA_3[10] = " ";
char tableColumnA_4[10] = " ";
char tableColumnB_0[10] = " ";
char tableColumnB_1[10] = " ";
char tableColumnB_2[10] = " ";
char tableColumnB_3[10] = " ";
char tableColumnB_4[10] = " ";

char button1ChosenPatch[MAX_FILE_NAME_LENGTH + 1] = {"NOTHING"};
char button2ChosenPatch[MAX_FILE_NAME_LENGTH + 1] = {"NOTHING"};
char button3ChosenPatch[MAX_FILE_NAME_LENGTH + 1] = {"NOTHING"};
char* selectedButtonChosenPatch;

char triggerNames[5][10]{
  "TRIG 1",
  "TRIG 2",
  "TRIG 3",
  "TRIG 4",
  "TRIG 5"
};

byte selectedTrigger = 0;
byte triggerCcNums[5] = {0,0,0,0,0};
byte triggerCcVals[5] = {0,0,0,0,0};
bool trigger1OnOffStatus = false;
bool trigger2OnOffStatus = false;
bool trigger3OnOffStatus = false;
bool trigger4OnOffStatus = false;
bool trigger5OnOffStatus = false;

bool* triggerOnOffStatuses[5] = {&trigger1OnOffStatus,&trigger2OnOffStatus,&trigger3OnOffStatus,&trigger4OnOffStatus,&trigger5OnOffStatus};
byte midiMin = 0;
byte midiMax = 127;
byte trigger1ChosenFunction = 0;
byte trigger2ChosenFunction = 0;
byte trigger3ChosenFunction = 0;
byte trigger4ChosenFunction = 0;
byte trigger5ChosenFunction = 0;
byte* selectedTriggerSelectedApp = nullptr;
byte trigger1ChosenApp = 0;
byte trigger2ChosenApp = 0;
byte trigger3ChosenApp = 0;
byte trigger4ChosenApp = 0;
byte trigger5ChosenApp = 0;
byte midiChannelMin = 1;
byte midiChannelMax = 16;

byte* selectedTriggerSelectedFunction = nullptr;
char triggerOnOffStatus[5] = {' ',' ',' ',' ',' '};
byte triggerNavXY[2] = {0,0};

bool* executeTrigStatus = nullptr;
byte* executeTrigFunction = nullptr;
byte* executeTrigApp = nullptr;

void increaseParameterValue(int8_t &selectedParameterVal, int8_t selectedParameterMax) {
    if (selectedParameterVal < selectedParameterMax) {
        selectedParameterVal++;
    }
}

void decreaseParameterValue(int8_t &selectedParameterVal, int8_t selectedParameterMin) {
    if (selectedParameterVal > selectedParameterMin) {
        selectedParameterVal--;
    }
}

void increaseParameterValue(byte &selectedParameterVal, byte selectedParameterMax) {
  if (selectedParameterVal < selectedParameterMax) {
    selectedParameterVal++;
    
  }
}

void decreaseParameterValue(byte &selectedParameterVal, byte selectedParameterMin) {
  if (selectedParameterVal > selectedParameterMin) {
    selectedParameterVal--;
    
  }
}
void increaseParameterValue(byte* selectedParameterVal, byte* selectedParameterMax) {
  if (*selectedParameterVal < *selectedParameterMax) {
    (*selectedParameterVal)++;
    
  }
}
void decreaseParameterValue(unsigned long &selectedParameterVal, unsigned long selectedParameterMin) {
  if (selectedParameterVal > selectedParameterMin) {
    selectedParameterVal--;
    
  }
}

void decreaseParameterValue(byte* selectedParameterVal, byte* selectedParameterMin) {
  if (*selectedParameterVal > *selectedParameterMin) {
    (*selectedParameterVal)--;
    
  }
}

void increaseParameterValue(int &selectedParameterVal, int selectedParameterMax) {
  if (selectedParameterVal < selectedParameterMax) {
    selectedParameterVal++;
    
  }
}
void increaseParameterValue(unsigned long &selectedParameterVal, unsigned long selectedParameterMax) {
  if (selectedParameterVal < selectedParameterMax) {
    selectedParameterVal++;
    
  }
}
void decreaseParameterValue(int &selectedParameterVal, int selectedParameterMin) {
  if (selectedParameterVal > selectedParameterMin) {
    selectedParameterVal--;
    
  }
}











JustReceived msgJustReceived = Nothing;
JustReceived lastMsgReceived = Nothing;

byte lastNrpnMsgValue[4] = {255,255,255,255};
NrpnState nrpnState = NRPN_IDLE;
unsigned long midiDebounceInterval = 40;
unsigned long lastMidiMessageTime = 0;
bool nrpnStarted = false;
unsigned long lastNrpnMsgTime = 0;
unsigned long nrpnStartTime = 0; 
unsigned long nrpnTimeout = 100;
byte previousCcReceived = 255;
byte previousCcValueReceived = 255;
byte incomingCcNum = 255;
byte incomingCcValue = 255;
byte nrpnMsbNum = 255;
byte nrpnLsbNum = 255;
byte nrpnMsbDat = 255;
byte nrpnLsbDat = 255;

byte lastMidiNoteNumberReceived = 255;
byte lastMidiNoteVelocityReceived = 255;
byte lastMidiNoteChannelReceived = 255;


void sendNrpn(byte MSB, byte LSB, byte MSBDAT, byte LSBDAT){
          MIDI.sendControlChange(99,MSB,midiCcThruOutChannel);
          delayMicroseconds(10);
          MIDI.sendControlChange(99,LSB,midiCcThruOutChannel);
          delayMicroseconds(10);
          MIDI.sendControlChange(99,MSBDAT,midiCcThruOutChannel);
          delayMicroseconds(10);
          MIDI.sendControlChange(99,LSBDAT,midiCcThruOutChannel);
          delayMicroseconds(10);
}

byte maxTriggers = 25;  
float systemVersion = 1.02;
