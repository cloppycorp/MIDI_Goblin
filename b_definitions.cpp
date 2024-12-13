#include "b_definitions.h"

//create class for sd card
SPIClass SPI_HSPI(HSPI);
//create a class for oled
SPIClass SPI_VSPI(VSPI);
// Initialize the SH1106 display using SPI
Adafruit_SH1106G display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);
HardwareSerial MIDI_SERIAL(1);
MIDI_CREATE_INSTANCE(HardwareSerial, MIDI_SERIAL, MIDI);
bool usbInitialized = false;
int menuY = 0;
int menuLineHeight = 0;
WaveState waveForm = TRIANGLE;
void defineListGridColumn(Grid* gridArray, byte x, byte lineHeight) {
    byte y;
    for(byte i = 0; i < 5; i++) {
        y = (i * lineHeight) + (HEADER_AREA_HEIGHT + 1);
        gridArray[i] = {x, y};
    }
}
ScreenState currentScreen = DEVICE_SELECT;
ScreenState previousScreen = DEVICE_SELECT;
ReceivedNrpn receivedNrpn[1] = {255,255,255,255};
ReceivedNrpn previousNrpn[1] = {255,255,255,255};
bool receivedNrpnMatch = false;
bool nrpnBufferReset = true;
EncChange encState = unchanged;
Grid listGridColumnA[5];
Grid listGridColumnB[5];
Grid listGridColumnC[5];
Grid listGridColumnD[5];
Grid listGridColumnE[5];
byte nrpnBuffer[4] = {255,255,255,255};
Grid newFileNameGrid[20];
Grid tableColumnA[5];
Grid seqGrid[32];
Grid seqSubMenuGrid[17];
Grid tableColumnB[5];
Grid lfoGridPoint[11];

Grid chordEntryGrid[15];
Grid controllerBoxGrid[21];
Grid controllerCcNumGrid[12];
Grid controllerCcValGrid[12];

Grid addCcParamGrid[17];

IndexedGrid ctrlrGrid[21];

void defineCtrlrgrid(){
  ctrlrGrid[0] = {0,0,0,1,0};
  ctrlrGrid[1] = {1,1,0,27,0};
  ctrlrGrid[2] = {2,2,0,53,0};
  ctrlrGrid[3] = {3,3,0,79,0};
  ctrlrGrid[4] = {4,4,0,105,0};
  
  ctrlrGrid[5] = {5,0,1,1,13};
  ctrlrGrid[6] = {6,1,1,27,13};
  ctrlrGrid[7] = {7,2,1,53,13};
  ctrlrGrid[8] = {8,3,1,79,13};
  ctrlrGrid[9] = {9,4,1,105,13};
  
  ctrlrGrid[10] = {10,0,2,1,26};
  ctrlrGrid[11] = {11,1,2,27,26};
  ctrlrGrid[12] = {12,2,2,53,26};
  ctrlrGrid[13] = {13,3,2,79,26};
  ctrlrGrid[14] = {14,4,2,105,26};
  
  ctrlrGrid[15] = {15,0,3,1,39};
  ctrlrGrid[16] = {16,1,3,27,39};
  ctrlrGrid[17] = {17,2,3,53,39};
  ctrlrGrid[18] = {18,3,3,79,39};
  ctrlrGrid[19] = {19,4,3,105,39};

  ctrlrGrid[20] = {20,0,4,0,51};
}

void defineControllerBoxGrid(){
    controllerBoxGrid[0] = {1,0}; //top left corner of boxes
    controllerBoxGrid[1] = {27,0};
    controllerBoxGrid[2] = {53,0};
    controllerBoxGrid[3] = {79,0};
    controllerBoxGrid[4] = {105,0};
    controllerBoxGrid[5] = {1,13}; //top left corner of boxes
    controllerBoxGrid[6] = {27,13};
    controllerBoxGrid[7] = {53,13};
    controllerBoxGrid[8] = {79,13};
    controllerBoxGrid[9] = {105,13};    
    controllerBoxGrid[10] = {1,26}; //top left corner of boxes
    controllerBoxGrid[11] = {27,26};
    controllerBoxGrid[12] = {53,26};
    controllerBoxGrid[13] = {79,26};
    controllerBoxGrid[14] = {105,26};
    controllerBoxGrid[15] = {1,39}; //top left corner of boxes
    controllerBoxGrid[16] = {27,39};
    controllerBoxGrid[17] = {53,39};
    controllerBoxGrid[18] = {79,39};
    controllerBoxGrid[19] = {105,39};
    controllerBoxGrid[20] = {0,51};//where name of midi parameter is drawn

}


void defineCcParamGrid(){
    addCcParamGrid[0] = {0,HEADER_AREA_HEIGHT +1}; //where to draw the table of the cc number
    
    addCcParamGrid[1] = {34,25};//where to draw the letters of the temp cc name
    addCcParamGrid[2] = {40,25};
    addCcParamGrid[3] = {46,25};
    addCcParamGrid[4] = {52,25};
    addCcParamGrid[5] = {58,25};
    addCcParamGrid[6] = {64,25};
    addCcParamGrid[7] = {70,25};
    addCcParamGrid[8] = {76,25};
    addCcParamGrid[9] = {82,25};
    addCcParamGrid[10] = {88,25};
    addCcParamGrid[11] = {94,25};
    addCcParamGrid[12] = {100,25};
    addCcParamGrid[13] = {106,25};
    addCcParamGrid[14] = {112,25};
    
    addCcParamGrid[15] = {0,38}; //cancel button
    addCcParamGrid[16] = {0,51}; // confirm button


}



void defineChordEntryGrid(){
  //gridpoints for the chord entry screen
  chordEntryGrid[0] = {2, 22}; // note 1 position
  chordEntryGrid[1] = {18, 22}; // note 2 position
  chordEntryGrid[2] = {34, 22}; // note 3 position
  chordEntryGrid[3] = {50, 22}; // note 4 position
  chordEntryGrid[4] = {66, 22}; // note 5 position 
  chordEntryGrid[5] = {82, 22}; // note 6 position
  chordEntryGrid[6] = {98, 22}; // note 7 position
  chordEntryGrid[7] = {114, 22}; // note 8 position
  chordEntryGrid[8] = {26, 12}; // instruction text
  chordEntryGrid[9] = {0, 20}; // box around chord array
  chordEntryGrid[10] = {0, 34}; // reset button
  chordEntryGrid[11] = {18, 36}; // reset button text
  chordEntryGrid[12] = {66, 34}; // cancel button
  chordEntryGrid[13] = {81, 36}; // cancel button text 
  chordEntryGrid[14] = {0, 48}; // confirm button
}
void defineLfoGrid() {
  //this grid defines the startpoint for selections (upper left corner of box that will be drawn in white with black text(selection)
  lfoGridPoint[0] = {0, 11}; // startpoint for waveform - this will be a rectangle drawn around the outer edges of the waveform box
  lfoGridPoint[1] = {5, 54}; // lfostatus value
  lfoGridPoint[2] = {44, 54}; // lfoCCnum value
  lfoGridPoint[3] = {70, 54}; // lfofreq value
  lfoGridPoint[4] = {105, 54}; // lfodepth value
  lfoGridPoint[5] = {0, 44}; // lfostatus lable
  lfoGridPoint[6] = {44, 44}; // lfoCCnum lable
  lfoGridPoint[7] = {64, 44}; // lfoFreq lable
  lfoGridPoint[8] = {95, 44}; // lfodepth lable
  lfoGridPoint[9] = {2,64};//left waveform arrow
  lfoGridPoint[10] ={120,64};//right waveform arrow
 
}

void defineSeqSubMenuGrid() {
  seqSubMenuGrid[0] = {3,6}; // "recording" header
  seqSubMenuGrid[1] = {69,6}; // "playback" header
  seqSubMenuGrid[2] = {3,18};//play/record
  seqSubMenuGrid[3] = {3,28};//play only
  seqSubMenuGrid[4] = {3,38};//record only
  seqSubMenuGrid[5] = {69,18};//ringout?
  seqSubMenuGrid[6] = {69,28};//tie?
  seqSubMenuGrid[7] = {69,38};//velocity:
  seqSubMenuGrid[8] = {118,18};//ringout y/n
  seqSubMenuGrid[9] = {118,28};//tie y/n
  seqSubMenuGrid[10] = {108,38};//velocity
  seqSubMenuGrid[11] = {0,4};//top left corner
  seqSubMenuGrid[12] = {64,4};//middle line
  seqSubMenuGrid[13] = {57,18};//play/record
  seqSubMenuGrid[14] = {57,28};//play only
  seqSubMenuGrid[15] = {57,38};//record only
  seqSubMenuGrid[16] = {0,53};//save / load 
}


void defineControllerCcNumGrid(){

  controllerCcNumGrid[0]= {2,2};
  controllerCcNumGrid[1]= {34,2};
  controllerCcNumGrid[2] = {66,0};
  controllerCcNumGrid[3] = {98,0};
  controllerCcNumGrid[4] = {2,16};
  controllerCcNumGrid[5] = {34,16};
  controllerCcNumGrid[6] = {66,16};
  controllerCcNumGrid[7] = {98,16};
  controllerCcNumGrid[8] = {2,32};
  controllerCcNumGrid[9] = {34,32};
  controllerCcNumGrid[10] = {66,32};
  controllerCcNumGrid[11] = {98,32};
}

void defineControllerCcValGrid(){
  controllerCcNumGrid[0]= {2,2};
  controllerCcNumGrid[1]= {48,2};
  controllerCcNumGrid[2] = {80,0};
  controllerCcNumGrid[3] = {112,0};
  controllerCcNumGrid[4] = {2,16};
  controllerCcNumGrid[5] = {48,16};
  controllerCcNumGrid[6] = {80,16};
  controllerCcNumGrid[7] = {112,16};
  controllerCcNumGrid[8] = {2,32};
  controllerCcNumGrid[9] = {48,32};
  controllerCcNumGrid[10] = {80,32};
  controllerCcNumGrid[11] = {112,32};
}


void defineSeqGrid(){
    seqGrid[0] = {0,0};
    seqGrid[1] = {16,0};
    seqGrid[2] = {32,0};
    seqGrid[3] = {48,0};
    seqGrid[4] = {64,0};
    seqGrid[5] = {80,0};
    seqGrid[6] = {96,0};
    seqGrid[7] = {112,0};
    seqGrid[8] = {0,16};
    seqGrid[9] = {16,16};
    seqGrid[10] = {32,16};
    seqGrid[11] = {48,16};
    seqGrid[12] = {64,16};
    seqGrid[13] = {80,16};
    seqGrid[14] = {96,16};
    seqGrid[15] = {112,16};    
    seqGrid[16] = {0,32};
    seqGrid[17] = {16,32};
    seqGrid[18] = {32,32};
    seqGrid[19] = {48,32};
    seqGrid[20] = {64,32};
    seqGrid[21] = {80,32};
    seqGrid[22] = {96,32};
    seqGrid[23] = {112,32};
    seqGrid[24] = {0,48};
    seqGrid[25] = {16,48};
    seqGrid[26] = {32,48};
    seqGrid[27] = {48,48};
    seqGrid[28] = {64,48};
    seqGrid[29] = {80,48};
    seqGrid[30] = {96,48};
    seqGrid[31] = {112,48};
}
// Custom map function for unsigned long
unsigned long mapUL(unsigned long x, unsigned long in_min, unsigned long in_max, unsigned long out_min, unsigned long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


