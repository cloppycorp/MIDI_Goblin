#ifndef Q_VECTORS_H
#define Q_VECTORS_H
#include <vector>
#include <array>



#include <algorithm>
#include <string>
#include <cstring>
#include <Arduino.h>
#include "c_appVariables.h"
#include "d_selectionVariables.h"



struct SequencerArray {
byte numNotes; // zero == empty, 1 = single note, 2 = chord
byte seqStepNotes[8];
int8_t swang;
};

extern std::vector<SequencerArray> sequencerArray;

struct SettingEntry {
    bool label;
    char name[20];
    bool* valueBool;
    byte* valueByte;
    int* valueInt;
    unsigned long* valueUnLong;

    // Constructor
    SettingEntry(bool lbl, const char* nm, bool* vBool = nullptr, byte* vByte = nullptr, int* vInt = nullptr, unsigned long* vUnLong = nullptr)
    : label(lbl), valueBool(vBool), valueByte(vByte), valueInt(vInt), valueUnLong(vUnLong) {
        // Copy the name into the 'name' array, making sure it's null-terminated
        strncpy(name, nm, sizeof(name) - 1);
        name[sizeof(name) - 1] = '\0'; // Ensure null termination
    }
};



struct DeviceParamsStruct{
  byte index;
  bool nrpn;
  char name[15];
  byte ccNum;
  byte ccVal;
  byte msb;
  byte lsb;
  byte msbVal;
  byte lsbVal;
  bool obtained;
  bool reMapped;
  byte reMapCc;
  byte panelCcVal;
  byte panelMsbVal;
  byte panelLsbVal;
  byte morphStart;
  byte morphFinal;
  byte morphCalc;
  byte morphStep;

};
struct CtrlrStruct {
    uint8_t index;
    char ccName[15]; // Adjust the size as needed
    uint8_t ccNum;
    uint8_t ccVal;
};

struct CtrlrDispStruct{
  uint8_t* index;
  char* ccName;
  uint8_t* ccNum;
  uint8_t* ccVal;
  bool* obtainedVal;
};

struct UserButtonStruct{
  byte buttonNumber;
  byte buttonFunction;
  byte buttonApp;
  bool buttonPanic;
  char buttonPatch[25];
  char buttonSequence[25];
};


void initUserBtns();


struct TriggerStruct{
  uint8_t index;
  char name[10];
  uint8_t ccNum;
  uint8_t ccVal;
  bool status;
  byte functionNum;
  uint8_t appNum;
  int patch;
  int sequence;
  bool panic;
  char triggerPatch[25];
  char triggerSequence[25];
};

struct DispTriggerStruct{
  uint8_t* index;
  char* name;
  uint8_t* ccNum;
  uint8_t* ccVal;
  bool* status;
  byte* functionNum;
  uint8_t* appNum;
  int* patch;
  int* sequence;
  bool* panic;
};


extern std::vector<DeviceParamsStruct> deviceParams; //the device parameters vector
extern DeviceParamsStruct* midiIndex[128];  // a pointer array that is used to slam received values into the deviceparams vector
extern std::vector<UserButtonStruct> userButtons;

extern std::vector<DispTriggerStruct> triggerPtrs;

extern std::vector<CtrlrDispStruct> ctrlrPtrs;
extern std::vector<CtrlrStruct> ctrlrParamList;
extern std::vector<TriggerStruct> triggerList;
extern std::vector<SettingEntry> deviceSettings;



using CharArray = std::array<char, 25>;
using CharArray14 = std::array<char, 15>;
extern std::vector<CharArray> patchList;
extern std::vector<CharArray> reMapList;
extern std::vector<CharArray> sequenceList;
extern std::vector<CharArray> trackerSequenceList;

extern std::vector<uint8_t> controllerCcNums;
extern std::vector<uint8_t> controllerCcVals;
extern std::vector<CharArray14> controllerCcNames;



extern std::vector<CharArray>* selectedList;
extern const char* commonExclusions[];
extern const int commonExclusionsSize; 

void initializeDeviceSettings();
void initiateSequencerArray();

// Helper function to perform case-insensitive comparison
bool caseInsensitiveCompare(const std::string& a, const std::string& b);
bool customSort(const CharArray& a, const CharArray& b);
void sortVector(std::vector<CharArray>& list);


void sortVector(std::vector<CharArray>* listPtr);

void insertEntry(CtrlrStruct newEntry, std::vector<CtrlrStruct>& list);


#endif // Q_VECTORS_H
