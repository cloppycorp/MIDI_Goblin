#ifndef G_DISPLAYEDVARS_H
#define G_DISPLAYEDVARS_H
#include <Arduino.h>
#include "b_definitions.h"

extern char currentScreenName[22];
//extern char visibleDeviceNames[5][21];
extern char (*deviceNames)[21];
extern char deviceFolder[21];
extern byte deviceArraySize;
extern char visibleAppNames[5][21];
extern byte visibleAppIndex;
extern char appNames[12][21];
extern byte numberOfApps;
extern const char** boxContent;
extern char toggleableAppNames[6][21];
//array for storing note names
extern const char* noteTable[128][2];
extern const char alphabet[37];
//newfile
extern char newFileNameArray[25];// Initialize the character array with "-----\0"
extern byte newFileNameArrayIndexes[MAX_FILE_NAME_LENGTH + 1];
extern char displayedSettingsNames[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedPatchNames[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedDeviceCcNames[5][15];
extern byte displayedDeviceCcNums[5];
extern byte displayedDeviceCcValues[5];
extern char deviceMonitorNames[5][15];
extern bool* deviceMonitorObtained[5];
extern byte* deviceMonitorByteColumnA[5];
extern byte* deviceMonitorByteColumnB[5];
extern char displayedSequenceNames[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedReMapFileNames[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedTrackerFileNames[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedListEntries[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedListEntries2[5][MAX_FILE_NAME_LENGTH + 1];
extern char displayedCharListEntries[5];
extern char displayedButton1ChosenApp[6];
extern char displayedButton2ChosenApp[6];
extern char displayedButton3ChosenApp[6];
extern char *selectedButtonChosenAppArray;
extern int displayedListCount;
extern byte selectedMidiOutChannel;
extern byte selectedMidiOutChannelMax;
extern byte selectedMidiOutChannelMin;
extern char chosenAppArray[6][3] ;
extern char* dispdCharPtrsA[5];
extern char* dispdCharPtrsB[5];
extern char* dispdCharPtrsC[5];
extern byte* dispdBytePtrsA[5];
extern byte* dispdBytePtrsB[5];
extern byte* dispdBytePtrsC[5];
extern uint8_t goblinSplash [];
#endif // G_DISPLAYEDVARS_H
