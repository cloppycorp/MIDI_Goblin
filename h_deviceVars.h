#ifndef H_DEVICEVARS_H
#define H_DEVICEVARS_H
#include <Arduino.h>
#include "b_definitions.h"
#include "q_vectors.h"

extern byte seqClockMode;

extern bool DS;

extern bool patchLoaded;
extern int dynamicMaxFileNameLength;  // Start with a default value
//device variables
extern byte numEntries;
extern byte* deviceCcNums ;
extern byte (*deviceMsbNums);
extern byte (*deviceLsbNums);
extern byte (*deviceCcValues);
extern byte (*deviceMsbValues) ;
extern byte (*deviceLsbValues);
extern byte (*panelCcValues);
extern byte (*panelMsbValues);
extern byte (*panelLsbValues);
extern char (*deviceCcNames)[15];
extern const char* goblinSsid;
extern const char* goblinPassword;
extern IPAddress local_IP;       // Set ESP32 AP IP
extern IPAddress gateway;        // Set gateway to the same as local IP in AP mode
extern IPAddress subnet;       // Subnet mask

extern byte* selectedUserBtn;
extern byte* selectedUserBtnFnctn;
extern byte* selectedUserBtnApp;
extern bool* selectedUserBtnPanic;
extern char* selectedUserBtnPatch;
extern char* selectedUserBtnSequence;


extern unsigned int patchFileCapacity;
extern unsigned int sequenceFileCapacity;
extern unsigned int reMapFileCapacity;
extern unsigned int trackerFileCapacity;
extern unsigned int numberOfDevicePatchFiles;
extern unsigned int numberOfDeviceSequenceFiles;
extern const char panelRule[MAX_FILE_NAME_LENGTH + 1];
extern const char panelRule2[10];
extern int patchFileCount;
extern int sequenceFileCount;
extern int reMapFileCount;
extern int trackerFileCount;
bool checkFileName(const char* newFileNameArray);
bool checkFileName2(const char* selectedFileName);
extern unsigned long messageStartTime;
extern unsigned long messageTimeout;
extern char userButtonFunctions[][MAX_FILE_NAME_LENGTH + 1];
extern char userButtonNames[3][7];
extern int selectedUserButtonIndex;
extern bool deviceHasNrpn;
extern char functionNames[5][MAX_FILE_NAME_LENGTH + 1];
extern int functionIndex; 
extern byte button1ChosenFunction;
extern byte button2ChosenFunction;
extern byte button3ChosenFunction;
extern bool* button1ChosenApp;
extern bool* button2ChosenApp;
extern bool* button3ChosenApp;
extern int button1AppToToggle;
extern int button2AppToToggle;
extern int button3AppToToggle;
extern byte* selectedButtonSelectedFunction;
extern bool functionSelectionLeftRight;
extern int toggleableAppIndex;
extern byte toggleableAppIndexMax;
extern char displayedToggleAbleAppNames[5][21];
extern int* selectedButtonAppToToggle;
extern char tableColumnA_0[10];
extern char tableColumnA_1[10];
extern char tableColumnA_2[10];
extern char tableColumnA_3[10];
extern char tableColumnA_4[10];
extern char tableColumnB_0[10];
extern char tableColumnB_1[10];
extern char tableColumnB_2[10];
extern char tableColumnB_3[10];
extern char tableColumnB_4[10];

extern char button1ChosenPatch[MAX_FILE_NAME_LENGTH + 1];
extern char button2ChosenPatch[MAX_FILE_NAME_LENGTH + 1];
extern char button3ChosenPatch[MAX_FILE_NAME_LENGTH + 1];
extern char* selectedButtonChosenPatch;
extern byte selectedTrigger;
extern byte triggerNumber[5];
extern char triggerNames[5][10];
extern byte triggerCcNums[5];
extern byte triggerCcVals[5];
extern bool trigger1OnOffStatus;
extern bool trigger2OnOffStatus;
extern bool trigger3OnOffStatus;
extern bool trigger4OnOffStatus;
extern bool trigger5OnOffStatus;
extern bool* triggerOnOffStatuses[5];
extern byte midiMin;
extern byte midiMax;
extern char triggerOnOffStatus[5];
extern byte triggerNavXY[2];
extern byte* selectedTriggerSelectedFunction;
extern byte trigger1ChosenFunction;
extern byte trigger2ChosenFunction;
extern byte trigger3ChosenFunction;
extern byte trigger4ChosenFunction;
extern byte trigger5ChosenFunction;

extern byte* selectedTriggerSelectedApp;
extern byte trigger1ChosenApp;
extern byte trigger2ChosenApp;
extern byte trigger3ChosenApp;
extern byte trigger4ChosenApp;
extern byte trigger5ChosenApp;
extern byte* executeTrigFunction;
extern byte* executeTrigApp;
extern bool* executeTrigStatus;

extern byte midiChannelMin;
extern byte midiChannelMax;
void increaseParameterValue(byte &selectedParameterVal, byte selectedParameterMax);

void decreaseParameterValue(byte &selectedParameterVal, byte selectedParameterMin);

void increaseParameterValue(byte* selectedParameterVal, byte* selectedParameterMax);

void decreaseParameterValue(unsigned long &selectedParameterVal, unsigned long selectedParameterMin);
void increaseParameterValue(unsigned long &selectedParameterVal, unsigned long selectedParameterMax);


void decreaseParameterValue(byte* selectedParameterVal, byte* selectedParameterMin);
void increaseParameterValue(int &selectedParameterVal, int selectedParameterMax);

void decreaseParameterValue(int &selectedParameterVal, int selectedParameterMin);

void increaseParameterValue(int8_t &selectedParameterVal, int8_t selectedParameterMax);

void decreaseParameterValue(int8_t &selectedParameterVal, int8_t selectedParameterMin);



enum NrpnState{
  NRPN_IDLE,
  NRPN_AWAITING_98,
  NRPN_AWAITING_6,
  NRPN_AWAITING_38
};
extern bool nrpnInOrder;

enum JustReceived{
  MidiCC,
  MidiNote,
  Nrpn,
  Nothing,
  randomNrpn,
  randomCc
};

extern JustReceived msgJustReceived;
extern JustReceived lastMsgReceived;
extern byte maxTriggers;

extern byte lastNrpnMsgValue[4];
extern NrpnState nrpnState;
extern unsigned long midiDebounceInterval;
extern unsigned long lastMidiMessageTime;
extern bool nrpnStarted;
extern unsigned long lastNrpnMsgTime;
extern unsigned long nrpnStartTime; 
extern unsigned long nrpnTimeout;
extern byte previousCcReceived;
extern byte previousCcValueReceived;
extern byte incomingCcNum;
extern byte incomingCcValue;
extern byte nrpnMsbNum;
extern byte nrpnLsbNum;
extern byte nrpnMsbDat;
extern byte nrpnLsbDat;
extern byte lastMidiNoteNumberReceived;
extern byte lastMidiNoteVelocityReceived;
extern byte lastMidiNoteChannelReceived;
void sendNrpn(byte MSB, byte LSB, byte MSBDAT, byte LSBDAT);
extern float systemVersion;

#endif // H_DEVICEVARS_H
