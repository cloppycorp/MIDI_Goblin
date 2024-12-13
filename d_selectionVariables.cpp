#include "d_selectionVariables.h"
byte chordEntryNavXY[2] = {0,0};
byte displayIndex = 0;
byte displayIndexMin = 0;
byte displayIndexMax = 4;
//selection 
byte selection = 0;
int selectedDevice = 0;
byte selectionMinimum = 0;
byte selectionMaximum = 0;
byte foldersCount = 0;
byte* selectedParameterVal = nullptr;
byte* selectedParameterMin = nullptr;
byte* selectedParameterMax = nullptr;
bool* selectedBool = nullptr;
byte selectedCcParameter = 0;
byte selectedDeviceParam = 0;
byte selectedApp = 0;
int appMenuIndex;
char selectedFileName[MAX_FILE_NAME_LENGTH + 1];
int* selectedListIndex;
int selectedPatchFile = 0;
int selectedRemapFile = 0;
int selectedSequenceFile = 0;
int selectedTrackerFile = 0;
int* selectedListFileCount = nullptr;
int selectedListFileCapacity = 0;
char (*selectedDisplayedNames)[21] = nullptr; // Declaring as a pointer to an array of 21 chars
int selectedListIndexMax;
byte deviceLfoIndex;
byte selectedLfoCcNum = 0;
byte navX = 0;
byte navY = 0;
byte navXMin = 0;
byte navXMax = 0;
byte navYMin = 0;
byte navYMax = 0;
int* selectedValue = nullptr;


char selectedLetter;
byte alphabetIndex = 0;
unsigned int selectedFile = 0; //value of the pointer for that points to the selected file in the linked list  
const char* selectedPathSuffix = nullptr;
bool* selectedAppStatus = nullptr;
unsigned long* selectedTempo = nullptr;

void changeBoolState(bool* selectedBool) {
    if (selectedBool) { // Check if the pointer is not null
        *selectedBool = !(*selectedBool); // Toggle the bool value
        
    }
}

//controller 
byte ctrlrX = 0;
byte ctrlrXMin = 0;
byte ctrlrXMax = 0;

byte ctrlrY = 0;
byte ctrlrYMin = 0;
byte ctrlrYMax = 0;

int ctrlrIndex = 0;
byte selectedArrayEntry = 0;
byte selectedControllerParam = 0;
int ctrlrIndexMax = 0;

byte** selectedControllerParamNum = nullptr;
byte** selectedControllerParamVal = nullptr;

byte selectedTriggerIndex = 0;
int8_t* selectedSeqSwang = nullptr;
byte previousSeqSelection = 0;