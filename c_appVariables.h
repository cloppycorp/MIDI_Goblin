#ifndef C_APPVARIABLES_H
#define C_APPVARIABLES_H
#include <Arduino.h>
//arpeggiator-------------------------------
extern byte arpTransState;
extern bool maxArpTransp;
extern bool minArpTransp;
extern byte arpTranspAmt;
extern byte arpTranspPlus[8];
extern byte arpTranspMinus[8];

extern byte arpOutChannel2;
extern bool arpStepSync;
extern unsigned long* syncNextStep;
extern bool arpDirectionPlaying;
extern bool arpArrayUpdated;
extern unsigned long arpeggiatorStartTime;
extern unsigned long arpFirstNoteTime;
extern byte arpPlayPosition;
extern byte arpOutChannel;
extern byte arpeggiatiorNotePlaying;
extern bool arpStarted;
extern const byte ARP_ARRAY_SIZE;
extern byte arpArray[8];
void initializeArpArray();
extern bool arpAppStatus;
extern byte arpLengthVal;
extern byte arpLengthMaxMin;
extern byte arpLengthMaxMax;
extern byte arpLengthMin;
extern byte arpLengthMax;
extern byte arpModeVal;
extern byte arpModeMin;
extern byte arpModeMax;
extern const char arpModes[4][8];
extern bool arpLatch;
extern unsigned long arpeggiatorTempo;
extern unsigned long nextArpeggiatorTime; 
extern byte arpNotesHeld;
//arpnote-------------------------------

extern unsigned long arpNoteNextSequenceTime;
extern bool arpOverFlow;
extern byte arpVel;
extern byte arpNoteSequenceNotePosition;
extern unsigned long arpNoteStartTime;
extern unsigned long firstArpNotePressTime;
extern byte arpNoteNotePlaying;
extern byte arpNoteMajorChordArray[8];
extern byte arpNoteMinorChordArray[8];
extern bool arpNoteSequencePlaying; 
extern byte arpNoteRootNote;
extern bool firstArpNoteNotePlaying;
extern bool arpNoteAppStatus;
extern bool arpNoteKey ;
extern byte arpNoteLength;
extern byte arpNoteLengthMin;
extern byte arpNoteLengthMax;
extern bool arpNoteDirection;
extern unsigned long arpNoteTempo;
extern byte arpNotePlayingChordArray[8];
extern byte arpNoteOutChannel;

//autochord-------------------------------
extern byte autoChordPlaying[7];
extern byte autoChordOutChannel;
extern bool autoChordAppStatus;
extern bool autoChordKey;
extern byte autoChordLength;
extern byte autoChordLengthMin;
extern byte autoChordLengthMax;
extern byte autoChordVelocity;
extern byte autoChordVelocityMin;
extern byte autoChordVelocityMax;
extern byte autoChordRootNote;
extern bool autoChordHeld;
extern byte autoChordMajorChordArray[7];
extern byte autoChordMinorChordArray[7];
//lfo-------------------------------------
extern bool lfoUpDown;
extern bool previousLfoUpDown;
extern unsigned long lfoEndTime;
extern bool lfoAppStatus;
extern byte lfoWaveForm;
extern byte lfoWaveFormMin;
extern byte lfoWaveFormMax;
extern unsigned long lfoIncrementTime;
extern byte lfoOscVal;
extern byte lfoFreq;
extern byte lfoFreqMin;
extern byte lfoFreqMax;
extern byte lfoDepth;
extern byte lfoDepthMin;
extern byte lfoDepthMax;
extern unsigned long lfoTempo;
extern char lfoTaskBar[4][13];
extern byte lfoDeviceCcMin;
extern byte lfoDeviceCcMax;
extern byte tempDeviceLfoIndex;
extern byte tempDeviceCcNum;
extern byte tempDeviceCcVal;
extern byte lfoOutChannel;
//remap-------------------------------------
extern byte reMapOutChannel;
extern byte receivedDeviceCc;
extern bool reMapAppStatus;
extern byte reMapNewCcNum;
//sequencer---------------------------------
extern bool swangBarVisible;
extern int8_t seqSwangMin;
extern int8_t seqSwangMax;
extern int8_t seqSwang[32];
extern bool seqStarted;
extern byte seqPlayPosition;
extern bool sequencerUpdated;
extern bool seqAppStatus;
extern byte seqRecordMode;
extern byte seqRecordModeMin;
extern byte seqRecordModeMax;
extern bool seqRingout;
extern bool seqTie;
extern byte seqVel;
extern byte seqVelMin;
extern byte seqVelMax;
extern byte subMenuSelection;
extern byte subMenuSelectionMin;
extern byte subMenuSelectionMax;
extern char seqSubMenuLables[8][12];
extern char seqPlaybackOptionsText[3][12];
extern char seqRecordModes[3][12];
extern bool seqSubMenuVisible;
extern byte seqLength;
extern byte seqLengthMin;
extern byte seqLengthMax;


extern unsigned long sequencerTempo;
extern unsigned long nextSeqTime;
extern byte seqNotePlaying;
extern byte seqOutChannel;
//tracker-----------------------------------
extern unsigned long trackerTempo;
//patches-------------------
extern byte patchesOutChannel;
//THRU---------------
extern byte midiNoteThruOutChannel;
extern byte midiCcThruOutChannel;
//general--
extern bool tempoBarVisible;
extern bool encoderBarVisible;
extern byte majorChordPattern[8];
extern byte minorChordPattern [8];
extern unsigned long tempoMin;
extern unsigned long tempoMax;
//settings--------------
extern bool midiNoteThru;
extern bool midiCcThru;

extern char settingsList[6][21];
extern byte appOutChannels[7];
extern int selectedSetting;
extern byte selectedSettingMin;
extern byte selectedSettingMax;
extern byte* midiChannels[11];
extern byte midiOutChannelMin;
extern byte midiOutChannelMax;
extern bool* appStatuses[6];
extern char midiOutChannelNames[11][15];
extern byte displayedMidiOutChannels[5];
//controller--------------
extern bool ctrlrUseDeviceParams;
extern bool controllerAppStatus;
extern byte controllerOutChannel;

extern byte controllerParamsCount;
extern byte tempCcNum;
extern char tempCcName[15];
extern byte tempCcNameAlphaIndex[14];
//MORPH
extern bool morphed;
extern int previousMorphPosition;
extern int morphPosition;
extern unsigned long morphMin;
extern unsigned long morphMax;
extern unsigned int selectedMorphPatch;


extern bool morphFileChosen;
extern unsigned long morphChangeTime;
extern unsigned long morphDebounceTime;
extern bool morphTimerStarted;
extern byte morphOutChannel;
#endif //C_APPVARIABLES_H
