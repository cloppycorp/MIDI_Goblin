#ifndef R_FILEFUNCTIONS_H
#define R_FILEFUNCTIONS_H
#include <SD.h>
#include <string.h>
#include <array>
#include "d_selectionVariables.h"
#include "q_vectors.h"
#include "b_definitions.h"
#include "h_deviceVars.h"
#include "g_displayedVars.h"
#include "c_appVariables.h"
#include "j_displayFunctions.h"
#include "p_handleNav.h"
void loadSettings();
void saveSettings();
void printVectorContents(const std::vector<CharArray>& list);
void populateFileList(std::vector<CharArray>& list, const char* pathSuffix, const char* exclusions[], int numExclusions, int* fileCount, int fileListCapacity);
void createFileList(std::vector<CharArray>& fileList, const char* directory, const char** exclusions, size_t exclusionCount, int* fileCount, int fileCapacity, char displayedNames[][MAX_FILE_NAME_LENGTH + 1]);
void saveNewFile();
void loadSequence(char sequence[]);
void overwriteFile(std::vector<std::array<char, 25>>* selectedList, const char* selectedFileName);
void sendPatch(const std::vector<CharArray>& fileList, int selectedFile);
void sendPatch(char chosenPatch[]);
void savePanel();
void loadSequence(std::vector<CharArray>& fileList,int selectedFile);
void deleteFile(std::vector<CharArray>& fileList, unsigned int fileIndex, const char* pathSuffix, int* fileCountPtr);
void loadRemap(std::vector<CharArray>& fileList, int selectedFile);
void getSynthMidiInfo();
void getDeviceSettings();
void populateDevices();
void createPatchList();
void loadAndDisplaySplash();
void saveTriggersFile();
// Declare the helper function
String formatTrigger(const TriggerStruct& trigger);
String formatUserBtns(const UserButtonStruct& button);

bool parseTriggerLine(const String& line, TriggerStruct& trigger);
void loadTriggerFile();
bool checkFileExists(const String& filePath);
void saveCtrlFile();
void saveUserBtnsFile();
void loadUserButtonsFile();
void createRemapList();
bool parseUserButtonLine(const String& line, UserButtonStruct& button);
void createSequenceList();

void createTrackerList();

void populateMorphEnd(const std::vector<CharArray>& fileList, unsigned int selectedFile);


#endif // R_FILEFUNCTIONS_H
