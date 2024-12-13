#ifndef J_DISPLAYFUNCTIONS_H
#define J_DISPLAYFUNCTIONS_H
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h> //when creating default instance we need to use "Adafruit_SH1106G"
#include <Arduino.h>
#include "b_definitions.h"
#include "c_appVariables.h"
#include "d_selectionVariables.h"
#include "h_deviceVars.h"
#include "g_displayedVars.h"
#include "f_midiVars.h"
#include "q_vectors.h"
#include "o_sequencer.h"

void drawDeviceMonitor();
void drawSwangBar(byte swangBarX, byte swangBarY, int8_t* swangValue, int8_t seqSwangMin, int8_t seqSwangMax);
void drawMidiNoteMonitor();
void drawNrpnMonitor(byte msb, byte lsb, byte msbDat, byte lsbDat);
void drawMorphScreen();
void drawMidiThruScreen();
void drawSelectedTriggerApp(Grid* gridArray, byte x, byte selectedTriggerSelectedFunction, bool selected);
void scrollTriggerScreenDown(byte indexValue, std::vector<DispTriggerStruct>& destination, std::vector<TriggerStruct>& source);
void scrollTriggerScreenUp(byte indexValue, std::vector<DispTriggerStruct>& destination, std::vector<TriggerStruct>& source);
void drawTriggersScreen();
void drawSystemInfoScreen();

void scrollDispCtrlrUp(byte indexValue, std::vector<CtrlrDispStruct>& destination, std::vector<DeviceParamsStruct>& source);
void scrollDispCtrlrDown(byte indexValue, std::vector<CtrlrDispStruct>& destination, std::vector<DeviceParamsStruct>& source);
void scrollDispCtrlrUp(byte indexValue, std::vector<CtrlrDispStruct>& destination,std::vector<CtrlrStruct>& source );
void scrollDispCtrlrDown(byte indexValue, std::vector<CtrlrDispStruct>& destination,std::vector<CtrlrStruct>& source );
void drawChordEntryScreen(byte selectedArrayEntry);
void drawChosenApp(Grid* gridArray, byte x, char (*displayedCharListEntries)[5]);
void displayMsg(const char *message);
void displayIntArrayContents(int* array, int size);
void displayStringArrayContents(char** array, int size);
void drawTempoBar(byte tempoBarX, byte tempoBarY, unsigned long* selectedTempo);
void drawEncoderBar(byte encoderBarX, byte encoderBarY, int* selectedvalue, int encoderBarMin, int encoderBarMax);
void overwriteConfirm();
void drawSmallWhiteBox(byte x,byte y, char boxContent );
void drawBigCell(byte topLeftPixelx, byte topLeftPixely, const char* content, bool selected);
void drawBigCell(byte topLeftPixelx, byte topLeftPixely,byte number1, const char* words, byte number2, bool selected);
void drawWhiteBox(byte x, byte y, byte value);
void drawWhiteBox(byte x,byte y, const char* boxContent );
void drawWhiteBox(byte x, byte y, char specialChar);
void drawWhiteBox(byte x, byte y, unsigned long value);
void updateSelectedTempo(int step, unsigned long* selectedTempo);
void updateEncoderBar(int step, int* selectedValue, int valueMin, int valueMax);
void drawTable(byte topLeftPixelx, byte topLeftPixely, byte rows);
//for this table, try overload so you can plug in different things. 
void drawTableContents(
const char* tableColumnA_0,const char* tableColumnB_0,
const char* tableColumnA_1,const char* tableColumnB_1,
const char* tableColumnA_2,const char* tableColumnB_2,
const char* tableColumnA_3,const char* tableColumnB_3,
const char* tableColumnA_4,const char* tableColumnB_4,
byte rows);
void scrollMonitorUp(byte selectedDeviceParam);
void scrollMonitorDown(byte selectedDeviceParam);
void drawSelectionForChordEntry();
void drawRandomCcMonitor(byte previousCcReceived,byte previousCcValueReceived, byte displayedIncomingMidiChannel);
template <size_t N,size_t M>
void scrollListUp(int* selectedListIndex, char (&displayedArray)[5][N], char (*selectedArray)[M]) {
    char tempArrayValue[N];
    strncpy(tempArrayValue, selectedArray[*selectedListIndex], sizeof(tempArrayValue) - 1);
    tempArrayValue[sizeof(tempArrayValue) - 1] = '\0'; // Ensure null termination

    for (int i = 4; i > 0; --i) {
        strcpy(displayedArray[i], displayedArray[i - 1]);
    }
    strcpy(displayedArray[0], tempArrayValue);
}

template <size_t N,size_t M>
void scrollListDown(int* selectedListIndex, char (&displayedArray)[5][N], char (*selectedArray)[M]) {
    char tempArrayValue[N];
    strncpy(tempArrayValue, selectedArray[*selectedListIndex], sizeof(tempArrayValue) - 1);
    tempArrayValue[sizeof(tempArrayValue) - 1] = '\0'; // Ensure null termination

    for (int i = 0; i < 4; ++i) {
        strcpy(displayedArray[i], displayedArray[i + 1]);
    }
    strcpy(displayedArray[4], tempArrayValue);
}

template <size_t N,size_t M>
void scrollListUp(byte selection, char (&displayedArray)[5][N], char (*selectedArray)[M]) {
    char tempArrayValue[N];
    strncpy(tempArrayValue, selectedArray[selection], sizeof(tempArrayValue) - 1);
    tempArrayValue[sizeof(tempArrayValue) - 1] = '\0'; // Ensure null termination

    for (int i = 4; i > 0; --i) {
        strcpy(displayedArray[i], displayedArray[i - 1]);
    }
    strcpy(displayedArray[0], tempArrayValue);
}

template <size_t N,size_t M>
void scrollListDown(byte selection, char (&displayedArray)[5][N], char (*selectedArray)[M]) {
    char tempArrayValue[N];
    strncpy(tempArrayValue, selectedArray[selection], sizeof(tempArrayValue) - 1);
    tempArrayValue[sizeof(tempArrayValue) - 1] = '\0'; // Ensure null termination

    for (int i = 0; i < 4; ++i) {
        strcpy(displayedArray[i], displayedArray[i + 1]);
    }
    strcpy(displayedArray[4], tempArrayValue);
}
template <size_t N,size_t M>
void scrollListUp(int* selectedListIndex, char (&displayedArray)[5][N], std::vector<std::string>& selectedVector) {
  // Populate displayedArray with the new content from the vector
  for (int i = 0; i < 5; i++) {
    if ((*selectedListIndex) + i < selectedVector.size()) {
      strncpy(displayedArray[i], selectedVector[(*selectedListIndex) + i].c_str(), N);
    } else {
      memset(displayedArray[i], ' ', N);  // Fill with spaces if we run out of entries
    }
  }
}
template <size_t N,size_t M>
void scrollListDown(int* selectedListIndex, char (&displayedArray)[5][N], std::vector<std::string>& selectedVector) {
  // Populate displayedArray with the new content from the vector
  for (int i = 0; i < 5; i++) {
    if ((*selectedListIndex) + i < selectedVector.size()) {
      strncpy(displayedArray[i], selectedVector[(*selectedListIndex) + i].c_str(), N);
    } else {
      memset(displayedArray[i], ' ', N);  // Fill with spaces if we run out of entries
    }
  }
}



void scrollListUp(byte selection,byte dislayedArray[5], byte selectedArray[5]);

void scrollListDown(byte selection,byte dislayedArray[5], byte selectedArray[5]);

void scrollListUp(byte selection,byte dislayedArray[5], byte* const selectedArray[9],bool isPointer);

void scrollListDown(byte selection,byte dislayedArray[5], byte* const selectedArray[9],bool isPointer);



void drawHeader();
void displayFileList();
void fileNameEntry();
void updateDisplayNamesBasedOnScreen(char displayedNames[][21]);
void deleteConfirm();
void drawColumn(Grid* gridArray, byte x,bool* boolArray[5]);
void drawWaveform();
void drawLfoTaskBar();
void drawSeqNotes();
void drawSeqLength();
void drawSeqSubMenu();
void drawColumn(Grid* gridArray, byte x,const byte* valueArray);
template <size_t N>
void drawColumn(Grid* gridArray, byte x,char valueArray[][N],byte entriesToDraw){
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i < entriesToDraw;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  display.print(valueArray[i]);
 }   
}
template <size_t N>
void drawColumns(Grid* gridArray1, Grid* gridArray2, byte x1, byte x2, char valueArray1[][N], char valueArray2[], byte entriesToDraw) {
    display.setTextColor(SH110X_WHITE);
    
    // Set up grid arrays for both columns
    defineListGridColumn(gridArray1, x1, 10); 
    defineListGridColumn(gridArray2, x2, 10); 
    
    // Iterate through the entries
    for (byte i = 0; i < entriesToDraw; i++) {
        // Draw the first value array (full strings)
        display.setCursor(gridArray1[i].x, gridArray1[i].y);
        display.print(valueArray1[i]);
        
        // Check if the corresponding value in valueArray2 is 'X'
        if (valueArray2[i] == 'X') {
            // Draw the 'X' next to the corresponding entry from valueArray1
            display.setCursor(gridArray2[i].x, gridArray2[i].y);
            display.print(valueArray2[i]);
        }
    }
}
void drawColumn(Grid* gridArray, byte x,byte* valueArray[5]);
void drawColumn(Grid* gridArray, byte x, byte* byteArray[5], byte linesToDraw);
void drawColumn(Grid* gridArray, byte x,char charArray[5], byte linesToDraw);
void drawFunctionSmily(Grid* gridArray, byte x, int index, bool selected);
void drawFunctionSmily(Grid* gridArray, byte x, int* selectedButtonSelectedFunction, bool selected);
void drawColumn(Grid* gridArray, byte x,byte value);
void drawColumn(Grid* gridArray, byte x,char charArray[5]);
void scrollDisplayedFileListDown(const std::vector<CharArray>& fileList, char displayedNames[][21], int selectedFile);
void scrollDisplayedFileListUp(const std::vector<CharArray>& fileList, char displayedNames[][21], int selectedFile);
void drawSmallCell(byte boxX, byte boxY,byte textX, byte textY, byte boxWidth, byte boxHeight,const char* words, bool selected);
void drawSmallCell(byte boxX, byte boxY,byte textX, byte textY, byte boxWidth, byte boxHeight,byte value, bool boxSelected);
#endif //J_DISPLAYFUNCTIONS_H
