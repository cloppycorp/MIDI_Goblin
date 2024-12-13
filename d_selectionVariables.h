#ifndef D_SELECTIONVARIABLES_H
#define D_SELECTIONVARIABLES_H
#include <Arduino.h>
#include "b_definitions.h"
//selection 
extern byte displayIndex;
extern byte displayIndexMin;
extern byte displayIndexMax;
extern byte chordEntryNavXY[2];
extern byte selection;
extern int selectedDevice;
extern byte selectionMinimum;
extern byte selectionMaximum;
extern byte foldersCount;
extern byte* selectedParameterVal;
extern byte* selectedParameterMin;
extern byte* selectedParameterMax;
extern bool* selectedBool;
extern byte selectedCcParameter;
extern byte selectedDeviceParam;
extern byte selectedApp;
extern int appMenuIndex;
extern char selectedFileName[MAX_FILE_NAME_LENGTH + 1];
extern int* selectedListIndex;
extern int selectedListIndexMax;
extern int selectedPatchFile;
extern int selectedRemapFile;
extern int selectedSequenceFile;
extern int selectedTrackerFile;
extern int* selectedListFileCount;
extern int selectedListFileCapacity;
extern char (*selectedDisplayedNames)[21]; // Declaring as a pointer to an array of 21 chars
extern int* selectedValue;
extern char selectedLetter;
extern byte alphabetIndex;
extern unsigned int selectedFile; //value of the pointer for that points to the selected file in the linked list  
extern const char* selectedPathSuffix;
extern bool* selectedAppStatus;
extern unsigned long* selectedTempo;

extern byte navX;
extern byte navY;
extern byte navXMin;
extern byte navXMax;
extern byte navYMin;
extern byte navYMax;

extern byte deviceLfoIndex;
extern byte selectedLfoCcNum;
void changeBoolState(bool* selectedBool);

//controller 
extern int ctrlrIndex;
extern byte selectedArrayEntry;
extern byte selectedControllerParam;
extern byte** selectedControllerParamNum;
extern byte** selectedControllerParamVal;
extern int ctrlrIndexMax;

extern byte ctrlrX;
extern byte ctrlrXMin;
extern byte ctrlrXMax;
extern byte ctrlrY;
extern byte ctrlrYMin;
extern byte ctrlrYMax;

extern byte selectedTriggerIndex;
extern int8_t* selectedSeqSwang;
extern byte previousSeqSelection;
#endif // D_SELECTIONVARIABLES_H
