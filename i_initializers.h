#ifndef I_INITIALIZERS_H
#define I_INITIALIZERS_H
#include <Arduino.h>
#include "b_definitions.h"
#include "c_appVariables.h"
#include "g_displayedVars.h"
#include "h_deviceVars.h"
#include "d_selectionVariables.h"
#include "q_vectors.h"
void insertTriggerEntry();
void initTriggerPtrs();//creates the ptrs list for the trigger list
void initTriggerList();//creates the end entry for triggerList
void populateTriggerPtrs();//populates the triggerptr list with the triggers list
void resetCtrlrDispPtrs();
void initCtrlrDispPtrs();
void populateDispPtrs(std::vector<CtrlrDispStruct>& destination,std::vector<CtrlrStruct>& source );
void populateDispPtrs(std::vector<CtrlrDispStruct>& destination,std::vector<DeviceParamsStruct>& source );
void initDispdCharsPtrs(char* dispdCharPtrs[5],std::vector<CharArray>& list);
void initDispdBytePtrs(byte* dispdBytePtrs[5], std::vector<uint8_t>& list);
void linkMidiIndexToDeviceParams();
template <size_t N>
void findIndexFromNav(IndexedGrid (&gridArray)[N], byte &index, byte navX, byte navY) {
  index = 255;  // Use 255 as an invalid index placeholder

  for (byte i = 0; i < N; i++) {
    if (gridArray[i].xIndex == navX && gridArray[i].yIndex == navY) {
      index = i;
      break;  // Exit the loop as soon as the match is found
    }
  }

  // Handle the index outside the function, based on the value of 'index'.
}
void initSelectedButtonChosenAppArray();
void updateChosenAppArray(byte selectedButtonAppToToggle);

template <size_t N,size_t M>
void initiateDisplayedListEntries(char (*displayedListEntries)[N], char (*selectedArray)[M]) {
    
    // Check if the selectedArray has no entries
    if (N == 0) {
        strcpy(displayedListEntries[0], "no entries");
        // Fill the rest with blank spaces
        for (byte i = 1; i < 5; i++) {
            strcpy(displayedListEntries[i], " ");
        }
        return;
    }
    
    // Populate displayedListEntries with up to 5 entries from selectedArray
    for (byte i = 0; i < 5; i++) {
        if (i < N) {
            // Use strncpy to ensure we respect the size of displayedListEntries
            strncpy(displayedListEntries[i], selectedArray[i], MAX_FILE_NAME_LENGTH);
            displayedListEntries[i][MAX_FILE_NAME_LENGTH] = '\0';  // Ensure null-termination
            Serial.println(displayedListEntries[i]);
        } else {
            // Fill remaining entries with blank spaces if fewer than 5 entries
            strcpy(displayedListEntries[i], " ");
            Serial.println("error");
        }
    }
}

void initiateDisplayedCharArray();
void initDeviceMonitor();
void initiateDisplayedMidiOutChannels();
void updateChosenAppArray(int* selectedButtonAppToToggle);

void updateChosenAppArray(byte* selectedTriggerSelectedApp);

void initializeDisplayedFileNames(const std::vector<CharArray>& list, char displayedNames[][MAX_FILE_NAME_LENGTH + 1]);



void defineNewFileNameGrid();
void defineTableColumnA();
void defineTableColumnB();


// For byte type
void valueMinMax(byte &valueName, byte valueValue, byte &minValueName, byte minValueValue, byte &maxValueName, byte maxValueValue);

// For int type
void valueMinMax(int &valueName, int valueValue, int &minValueName, int minValueValue, int &maxValueName, int maxValueValue);

// For unsigned long type
void valueMinMax(unsigned long &valueName, unsigned long valueValue, unsigned long &minValueName, unsigned long minValueValue, unsigned long &maxValueName, unsigned long maxValueValue);
void initializeNewFileNameArray();

#endif // I_INITIALIZERS_H
