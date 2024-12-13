#include "c_appVariables.h"

//arpeggiator-------------------------------
byte arpTransState = 1;
bool maxArpTransp = false;
bool minArpTransp = false;
byte arpTranspAmt = 2;
byte arpTranspPlus[8] = {255,255,255,255,255,255,255,255};
byte arpTranspMinus[8] = {255,255,255,255,255,255,255,255};


bool arpOverFlow = false;
byte arpOutChannel2 = 2;
bool arpStepSync = false;
unsigned long* syncNextStep = nullptr;
bool arpDirectionPlaying = true; 
bool arpArrayUpdated = false;
unsigned long arpFirstNoteTime = 0;
byte arpPlayPosition = 0;
byte arpOutChannel = 2;
byte arpeggiatiorNotePlaying = 255;
bool arpStarted = false;
const byte ARP_ARRAY_SIZE = 8;
byte arpArray[8] = {255,255,255,255,255,255,255,255};
void initializeArpArray() {
    for (byte i = 0; i < ARP_ARRAY_SIZE; i++) {
        arpArray[i] = 255; // Initialize each element with 255
    }
}
byte arpVel = 127;
byte arpNotesHeld = 0;
bool arpAppStatus = false;
byte arpLengthVal = 3;
byte arpLengthMin = 1;
byte arpLengthMax = 8;
byte arpLengthMaxMin;
byte arpLengthMaxMax;
byte arpModeVal = 0;
byte arpModeMin = 0;
byte arpModeMax = 3;
const char arpModes[4][8] = {"--->","<---","--><--","RANDOM"};
bool arpLatch = false;
unsigned long arpeggiatorStartTime = 0;
unsigned long arpeggiatorTempo = 400;
unsigned long nextArpeggiatorTime = 0; 

//arpnote-------------------------------

extern unsigned long arpNoteNextSequenceTime = 0;
byte arpNoteSequenceNotePosition = 0;
unsigned long arpNoteStartTime = 0;
byte arpNoteNotePlaying = 0;
byte arpNotePlayingChordArray[8] = {0,0,0,0,0,0,0,0};
byte arpNoteMajorChordArray[8] = {0,0,0,0,0,0,0,0};
byte arpNoteMinorChordArray[8] = {0,0,0,0,0,0,0,0};
bool arpNoteSequencePlaying = false; 
byte arpNoteRootNote = 0;
bool firstArpNoteNotePlaying = false;
bool arpNoteAppStatus = false;
bool arpNoteKey = true;
byte arpNoteLength=3;
byte arpNoteLengthMin = 2;
byte arpNoteLengthMax = 8;
bool arpNoteDirection = true;
unsigned long arpNoteTempo = 400;
byte arpNoteOutChannel = 2;
//autochord-------------------------------
byte autoChordOutChannel = 2;
bool autoChordAppStatus = false;
bool autoChordKey = true;
byte autoChordLength = 3;
byte autoChordLengthMin = 2;
byte autoChordLengthMax = 7;
byte autoChordVelocity = 127;
byte autoChordVelocityMin = 0;
byte autoChordVelocityMax = 127;
byte autoChordRootNote;
bool autoChordHeld = false;
byte autoChordPlaying[7] = {0,0,0,0,0,0,0};
byte autoChordMajorChordArray[7] = {0,0,0,0,0,0,0};
byte autoChordMinorChordArray[7] = {0,0,0,0,0,0,0};

//lfo-------------------------------------
bool lfoAppStatus = false;
byte lfoWaveForm = 0;
byte lfoWaveFormMin = 0;
byte lfoWaveFormMax = 2;
unsigned long lfoIncrementTime = 0;
byte lfoFreq = 50;
byte lfoFreqMin = 1;
byte lfoFreqMax = 100;
byte lfoDepth = 50;
byte lfoDepthMin = 1;
byte lfoDepthMax = 100;
unsigned long lfoTempo = 0;
bool lfoUpDown = true;
bool previousLfoUpDown = false; 
unsigned long lfoEndTime = 0;
char lfoTaskBar[4][13] = {"ON/OFF", "CC", "FREQ", "DEPTH"};
byte lfoOscVal;
byte lfoDeviceCcMin = 0;
byte lfoDeviceCcMax;
byte tempDeviceLfoIndex = 0;
byte tempDeviceCcNum = 0;
byte tempDeviceCcVal = 0;
byte lfoOutChannel = 3;
//remap-------------------------------------
byte reMapOutChannel = 2;
byte receivedDeviceCc = 255;
byte receivedDeviceCcIndex = 0;
bool reMapAppStatus = false;

byte reMapNewCcNumMin = 0;
byte reMapDeviceCcIndexMax;
byte reMapNewCcNum = 0;
byte reMapNewCcNumMax = 127;

//sequencer---------------------------------
//SEQUENCER
bool swangBarVisible = false;
int8_t seqSwangMin = -75;
int8_t seqSwangMax = 75;
int8_t seqSwang[32];
bool seqStarted = false;
byte seqPlayPosition = 0;
extern bool sequencerUpdated = false;
bool seqAppStatus = false;
byte seqRecordMode = 0;
byte seqRecordModeMin = 0;
byte seqRecordModeMax = 2;
bool seqRingout = false;
bool seqTie = false;
byte subMenuSelection;
byte subMenuSelectionMin;
byte subMenuSelectionMax;
byte seqVel = 127;
byte seqVelMin = 0;
byte seqVelMax = 127;
char seqSubMenuLables[8][12] = {"RECORD","PLAYBACK","Play/Rec","Play Only","Rec Only","Ringout:","Tie:","Vel:"};
char seqPlaybackOptionsText[3][12] = {"Ringout:","Tie:","Vel:"};
char seqRecordModes[3][12] = {"Play/Rec","Play Only","Rec Only"};
bool seqSubMenuVisible = false;
byte seqLength = 8;
byte seqLengthMin = 2;
byte seqLengthMax = 32;


unsigned long sequencerTempo = 400;
unsigned long nextSeqTime = 0;
byte seqNotePlaying = 0;
byte seqOutChannel = 2;
//tracker-----------------------------------
unsigned long trackerTempo = 0;
//patches--------------
byte patchesOutChannel = 2;
//general
byte majorChordPattern [8] = {4,7,11,14,17,21,24,28};
byte minorChordPattern [8] = {3,7,10,14,17,21,25,28};
bool tempoBarVisible = false;
bool encoderBarVisible = false;
unsigned long tempoMin = 25;
unsigned long tempoMax = 750;
//settings----------------------
bool midiNoteThru = false;
bool midiCcThru = false;
char settingsList[6][21]{
  "SAVE SETTINGS",
  "USER BUTTONS",
  "MIDI OUT CHANNELS",
  "MIDI THRU",
  "CLOCK SOURCE",
  "SYSTEM INFO",
};

byte appOutChannels[7] = {patchesOutChannel,arpOutChannel,autoChordOutChannel,arpNoteOutChannel,seqOutChannel,lfoOutChannel,reMapOutChannel};
byte midiNoteThruOutChannel = 2;
byte midiCcThruOutChannel = 2;
char midiOutChannelNames[11][15] = {
"PATCHES",
"ARPEGGIATOR_1",
"AUTOCHORD",
"ARP-NOTE",
"SEQUENCER",
"LFO",
"REMAP",
"CONTROLLER",
"MORPH",
"NOTE THRU",
"CC THRU"  
};

int selectedSetting = 0;
byte selectedSettingMin = 0;
byte selectedSettingMax = 0;

byte* midiChannels[11] = {
&patchesOutChannel,
&arpOutChannel,
&autoChordOutChannel,
&arpNoteOutChannel,
&seqOutChannel,
&lfoOutChannel,
&reMapOutChannel,
&controllerOutChannel,
&morphOutChannel,
&midiNoteThruOutChannel,
&midiCcThruOutChannel
};
byte displayedMidiOutChannels[5];
byte midiOutChannelMin = 1;
byte midiOutChannelMax = 16;

bool* appStatuses[6]{
&arpAppStatus,
&autoChordAppStatus,
&arpNoteAppStatus,
&seqAppStatus,
&lfoAppStatus,
&reMapAppStatus
};

//controller---------------
bool ctrlrUseDeviceParams = true;
bool controllerAppStatus = true;
byte controllerOutChannel = 2;
byte controllerParamsCount = 0;
byte tempCcNum = 127;
char tempCcName[15] = {'1','2','3','4','5','6','7','8','9','0','1','2','3','4'};
byte tempCcNameAlphaIndex[14] = {255,255,255,255,255,255,255,255,255,255,255,255,255,255};
//morph
bool morphed = false;
int previousMorphPosition = 0;
int morphPosition = 0;
unsigned long morphMin = 0;
unsigned long morphMax = 100;
unsigned int selectedMorphPatch = 999;


bool morphFileChosen = false;
unsigned long morphChangeTime = 0;
unsigned long morphDebounceTime = 400;
bool morphTimerStarted = false;
byte morphOutChannel = 2;

