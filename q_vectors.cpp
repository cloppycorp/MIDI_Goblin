#include "q_vectors.h"

std::vector<DeviceParamsStruct> deviceParams;
DeviceParamsStruct* midiIndex[128];  // 

// Define the vectors here, so they are only defined once.
std::vector<CharArray> patchList;
std::vector<CharArray> reMapList;
std::vector<CharArray> sequenceList;
std::vector<CharArray> trackerSequenceList;

std::vector<uint8_t> controllerCcNums;
std::vector<uint8_t> controllerCcVals;
std::vector<CharArray14> controllerCcNames;
std::vector<CtrlrDispStruct> ctrlrPtrs;
std::vector<CharArray>* selectedList = nullptr;

std::vector<CtrlrStruct> ctrlrParamList;
std::vector<TriggerStruct> triggerList;
std::vector<DispTriggerStruct> triggerPtrs;

std::vector<SettingEntry> deviceSettings;
std::vector<SequencerArray> sequencerArray;
std::vector<UserButtonStruct> userButtons;

void initUserBtns(){
for(byte i = 0; i < 3; i++){
 UserButtonStruct newEntry = {i,0,0,false,"NOTHING","NOTHING"};
  userButtons.push_back(newEntry);
}

}

void initiateSequencerArray(){
  for(byte i = 0; i < 32; i++){
    SequencerArray newEntry = {0,255,255,255,255,255,255,255,255,0};
    sequencerArray.push_back(newEntry);
  }
  byte tempSize = sequencerArray.size();
  Serial.print("sequencer array entries: ");
  Serial.println(tempSize);
}



void initializeDeviceSettings() {
    deviceSettings.push_back(SettingEntry(true, "PATCHES", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "PATCHES_CH", nullptr, &patchesOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "ARPEGGIATOR", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_STATUS", &arpAppStatus, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_LATCH", &arpLatch, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_MODE", nullptr, &arpModeVal, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_MAX_SIZE", nullptr, &arpLengthMax, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_SPEED", nullptr, nullptr, nullptr, &arpeggiatorTempo));
    deviceSettings.push_back(SettingEntry(false, "ARP_SEQ_SYNC", &arpStepSync, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_CHANNEL_1", nullptr, &arpOutChannel, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_VELOCITY", nullptr, &arpVel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "AUTO-CHORD", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "AUTO-CHORD_STATUS", &autoChordAppStatus, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "AUTO-CHORD_KEY", &autoChordKey, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "AUTO-CHORD_LENGTH", nullptr, &autoChordLength, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "AUTO-CHORD_VELOCITY", nullptr, &autoChordVelocity, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "AUTO-CHORD_CH", nullptr, &autoChordOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "ARP-NOTE", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP-NOTE_STATUS", &arpNoteAppStatus, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_NOTE_KEY", &arpNoteKey, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_NOTE_LENGTH", nullptr, &arpNoteLength, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_NOTE_DIRECTION", &arpNoteDirection, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "ARP_NOTE_SPEED", nullptr, nullptr, nullptr, &arpNoteTempo));
    deviceSettings.push_back(SettingEntry(false, "ARP_NOTE_OUT_CHANNEL", nullptr, &arpNoteOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "SEQUENCER", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_LENGTH", nullptr, &seqLength, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_SPEED", nullptr, nullptr, nullptr, &sequencerTempo));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_REC_MODE", nullptr, &seqRecordMode, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_RINGOUT", &seqRingout, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_TIE", &seqTie, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_VELOCITY", nullptr, &seqVel, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "SEQUENCER_CH", nullptr, &seqOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "LFO", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "LFO_STATUS", &lfoAppStatus, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "LFO_WAVEFORM", nullptr, &lfoWaveForm, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "LFO_SELECTED_CC", nullptr, &selectedLfoCcNum, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "LFO_FREQ", nullptr, &lfoFreq, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "LFO_DEPTH", nullptr, &lfoDepth, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "LFO_SPEED", nullptr, nullptr, nullptr, &lfoTempo));
    deviceSettings.push_back(SettingEntry(false, "LFO_CH", nullptr, &lfoOutChannel, nullptr, nullptr));


    deviceSettings.push_back(SettingEntry(true, "CONTROLLER", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "CONTROLLER_CH", nullptr, &controllerOutChannel, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "CONTROL_DEVICE", &ctrlrUseDeviceParams, nullptr, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "REMAP", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "REMAP_CH", nullptr, &reMapOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "MORPH", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "MORPH_CH", nullptr, &morphOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "NOTE_THRU", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "NOTE_THRU_STATUS", &midiNoteThru, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "NOTE_THRU_CH", nullptr, &midiNoteThruOutChannel, nullptr, nullptr));

    deviceSettings.push_back(SettingEntry(true, "CC_THRU", nullptr, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "CC_THRU_STATUS", &midiCcThru, nullptr, nullptr, nullptr));
    deviceSettings.push_back(SettingEntry(false, "CC_THRU_CH", nullptr, &midiCcThruOutChannel, nullptr, nullptr));

}





const char* commonExclusions[] = {"README.TXT"};
const int commonExclusionsSize = sizeof(commonExclusions) / sizeof(commonExclusions[0]);





// Helper function to perform case-insensitive comparison
bool caseInsensitiveCompare(const std::string& a, const std::string& b) {
    std::string lowerA = a;
    std::string lowerB = b;
    std::transform(lowerA.begin(), lowerA.end(), lowerA.begin(), ::tolower);
    std::transform(lowerB.begin(), lowerB.end(), lowerB.begin(), ::tolower);
    return lowerA < lowerB;
}

bool customSort(const CharArray& a, const CharArray& b) {
    // Special case for "PANEL.txt"
    if (strcmp(a.data(), "PANEL.txt") == 0) return true;
    if (strcmp(b.data(), "PANEL.txt") == 0) return false;

    // Numeric files should come before alphabetic
    bool aIsDigit = std::isdigit(a.data()[0]);
    bool bIsDigit = std::isdigit(b.data()[0]);

    if (aIsDigit && !bIsDigit) return true;
    if (!aIsDigit && bIsDigit) return false;

    // For files that are both digits or both alphabetic, sort alphabetically (case-insensitive)
    return caseInsensitiveCompare(a.data(), b.data());
}

void sortVector(std::vector<CharArray>& list) {
    std::sort(list.begin(), list.end(), customSort);
}


void sortVector(std::vector<CharArray>* listPtr) {
    if (listPtr) {
        std::sort(listPtr->begin(), listPtr->end(), customSort);
    }
}

void insertEntry(CtrlrStruct newEntry, std::vector<CtrlrStruct>& list) {
    if (list.size() > 0) {
        // Insert the new entry before the last "END" entry
        list.insert(list.end() - 1, newEntry);
    } else {
        // Create the "END" entry
        CtrlrStruct endEntry = {255, "END", 255, 255};
        list.push_back(endEntry);
        // Add the new entry
        list.insert(list.end() - 1, newEntry);
    }
}





