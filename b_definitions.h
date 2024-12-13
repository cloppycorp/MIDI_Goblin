#ifndef B_DEFINITIONS_H
#define B_DEFINITIONS_H
#include "a_libraries.h"


#define SCREENWIDTH 128
#define SCREENHEIGHT 64
//define sd card pins
#define SD_CS_PIN  33
#define SD_MOSI    18
#define SD_MISO    19
#define SD_SCK     23

// Define the pins used
#define OLED_SDA    21
#define OLED_CLK    22
#define OLED_DC     17
#define OLED_CS     5
#define OLED_RESET  4

// Define the pins used for the multiplexer
#define MUX_S0      27
#define MUX_S1      14
#define MUX_S2      13
#define MUX_S3      12
#define MUX_SIG     34

//define character array for names of buttons
#define MAX_BTN_NAME_LENGTH 9 + 1
#define BTN_NAMES 16

//define pins for the encoder
#define ENC_A     39
#define ENC_B     36

//define header values
#define HEADER_AREA_WIDTH 128
#define HEADER_AREA_HEIGHT 10
#define HEADER_AREA_X 0
#define HEADER_AREA_Y 0

#define TABLECELLHEIGHT 11

#define MAX_FILE_NAME_LENGTH 20  // As a macro

#define TEMPOBAR_WIDTH 103
#define TEMPOBAR_HEIGHT 8


#define WAVEBOXWIDTH 128
#define WAVEBOXHEIGHT 33
#define WAVEBOX_X 0
#define WAVEBOX_Y 9

//define area of screen where taskbar will be drawn
#define LFOTASKBARWIDTH 128
#define LFOTASKBARHEIGHT 10
#define LFOTASKBAR_X 0
#define LFOTASKBAR_Y 43

#define SEQ_BOX_HEIGHT 15 //16-1 so it doesnt touch next grid line?
#define SEQ_BOX_WIDTH 15 //16-1

#define SUBMENUWIDTH 128
#define SUBMENUHEIGHT 45
#define SUBMENUHEADERHEIGHT 11
 
struct ValueEditConfig {
    bool* configSelectedBool;
    uint8_t* configSelectedParameterVal;
    uint8_t* configSelectedParameterMin;
    uint8_t* configSelectedParameterMax;
};



extern byte nrpnBuffer[4];



struct ValueEditConfig2 {
  bool* selectedBool;
  uint8_t* selectedParameterVal;
  uint8_t* selectedParameterMin;
  uint8_t* selectedParameterMax;
};

struct IndexedGrid{
  byte selectedEntry;
  byte xIndex;
  byte yIndex;
  byte xGridPoint;
  byte yGridPoint;
};

struct ReceivedNrpn{
  byte MSB;
  byte LSB;
  byte MSBDAT;
  byte LSBDAT;
};
extern ReceivedNrpn receivedNrpn[1];
extern ReceivedNrpn previousNrpn[1];
extern bool nrpnBufferReset;
extern bool receivedNrpnMatch;
struct Grid {
  byte x;
  byte y;
};

enum WaveState {
  TRIANGLE = 0,
  SINE = 1,
  SQUARE = 2,
};
extern WaveState waveForm;





enum userButtonFunction{
  NOTHING = 0,
  TOGGLE_APP = 1,
  SEND_PATCH = 2,
  LOAD_SEQUENCE = 3,
  PANIC = 4,
};
//enumeration of ScreenSate
enum ScreenState {
  DEVICE_SELECT = 0,
  MENU = 1,
  PATCHES = 2,
  ARPEGGIATOR = 3,
  AUTOCHORD = 4,
  ARPNOTE = 5,
  SEQUENCER = 6,
  LFO = 7,
  REMAP = 8,
  MONITOR = 9,
  TRIGGERS = 10,
  CONTROLLER = 11,
  MORPH = 12,
  SETTINGS = 13,
  SAVENEWFILE = 14,
  TROUBLESHOOT = 15,
  DELETE = 16,
  OVERWRITE = 17,
  SAVESEQ = 18,
  LOADSEQ = 19,
  SEQUENCES = 20,
  REMAPLIST = 21,
  MONITOR2 = 22,
  DEVICECCVALMONITOR = 23,
  REMAPCCNUMMONITOR = 24,
  REMAPCCVALMONITOR = 25,
  INCOMINGCCMONITOR = 26,
  SEQCHORDENTRY = 27,
  TRACKERLIST = 28,
  MESSAGE = 29,
  MIDIOUTCHANNELS = 30,
  USERBUTTONSELECT = 31,
  USERBUTTONFUNCTIONS = 32,
  USERBUTTONCHOOSEAPP = 33,
  USERBUTTONCHOOSEPATCH = 34,
  TRIGGERFUNCTIONS = 35,
  TRIGGERPATCHES = 36,
  TRIGGERCHOOSEAPP = 37,
  ADDCONTROLLERPARAM = 38,
  CONTROLLEROPTIONS = 39,
  MORPHPATCHES = 40,
  SEQSWANG = 41,
  ARPEGGIATOROPTIONS = 42,
  MIDITHRU = 43,
  CLOCKSOURCE = 44,
  SYSTEMINFO = 45,
  USERBUTTONCHOOSESESEQUENCE = 46,
  TRIGGERCHOOSESEQUENCE = 47,
};
extern ScreenState currentScreen;
extern ScreenState previousScreen;
enum EncChange {
  increased,
  unchanged,
  decreased,
};

extern EncChange encState;

//create class for sd card
extern SPIClass SPI_HSPI;
// Declare the SPI and OLED display
extern SPIClass SPI_VSPI;
extern Adafruit_SH1106G display;
extern HardwareSerial MIDI_SERIAL;
extern midi::MidiInterface<midi::SerialMIDI<HardwareSerial>> MIDI;
extern int menuY;
extern int menuLineHeight;
void defineListGridColumn(Grid* gridArray, byte x, byte lineHeight);
extern Grid listGridColumnA[5];
extern Grid listGridColumnB[5];
extern Grid listGridColumnC[5];
extern Grid listGridColumnD[5];
extern Grid listGridColumnE[5];
extern Grid newFileNameGrid[20];
extern Grid tableColumnA[5];
extern Grid seqGrid[32];
extern Grid tableColumnB[5];
extern Grid controllerBoxGrid[21];
extern Grid controllerCcNumGrid[12];
extern Grid controllerCcValGrid[12];

extern Grid lfoGridPoint[11];
extern Grid seqSubMenuGrid[17];
extern Grid chordEntryGrid[15];
extern Grid addCcParamGrid[17];
extern IndexedGrid ctrlrGrid[21];
void defineCcParamGrid();


void defineSeqSubMenuGrid();
void defineSeqGrid();
void defineLfoGrid();
void defineChordEntryGrid();

void defineControllerBoxGrid();
void defineControllerCcNumGrid();
void defineControllerCcValGrid();
void defineCtrlrgrid();
// Custom map function for unsigned long
unsigned long mapUL(unsigned long x, unsigned long in_min, unsigned long in_max, unsigned long out_min, unsigned long out_max);

template <size_t N>
bool doesArrayContainNumber(byte (&arr)[N], byte number) {
    for (byte i = 0; i < N; i++) {
        if (arr[i] == number) {
            return true;  // Return true if number is found
        }
    }
    return false;  // Return false if number is not found
}

#endif // B_DEFINITIONS_H
