#include "r_fileFunctions.h"
void loadSettings() {
    displayMsg("Loading settings...");  // Display feedback message

    char settingsFileName[13] = {"SETTINGS.txt"};  // Name of the settings file
    
    // Create a file path to load the settings file
    char filePath[51];
    File file;

    // Build the file path
    strcpy(filePath, "/");
    strcat(filePath, deviceFolder);
    strcat(filePath, "/CONFIG/");
    strcat(filePath, settingsFileName);

    // Open the file for reading
    file = SD.open(filePath, FILE_READ);
    if (file) {
        // Temporary buffers for reading lines
        char line[50];  // Buffer to hold each line of the file
        char label[25]; // Buffer to store entry name/label
        char value[10]; // Buffer to store the value portion

        // Read file line by line
        while (file.available()) {
            file.readBytesUntil('\n', line, sizeof(line));  // Read a line from the file
            line[strcspn(line, "\r\n")] = 0;  // Remove any newline characters
            
            // Check if the line contains both label and value
            if (sscanf(line, "%s %s", label, value) == 2) {
                // Search for the matching entry in deviceSettings
                for (auto& entry : deviceSettings) {
                    // Match the name in the file to the name in the entry
                    if (strcmp(entry.name, label) == 0) {
                        // Update the appropriate variable based on the type
                        if (entry.valueBool != nullptr) {
                            *entry.valueBool = (strcmp(value, "ON") == 0);  // ON -> true, OFF -> false
                        } else if (entry.valueByte != nullptr) {
                            *entry.valueByte = atoi(value);  // Convert string to byte
                        } else if (entry.valueInt != nullptr) {
                            *entry.valueInt = atoi(value);  // Convert string to int
                        } else if (entry.valueUnLong != nullptr) {
                            *entry.valueUnLong = strtoul(value, NULL, 10);  // Use strtoul to convert string to unsigned long
                        }
                    }
                }
            }
            // Handle label-only lines (no value on the line)
            else if (sscanf(line, "%s", label) == 1) {
                // If it's just a label, we don't need to do anything, but this ensures we skip it
                continue;
            }
        }

        // Close the file
        file.close();
        //displayMsg("Settings loaded successfully!");  // Success message
    } else {
        displayMsg("Error loading settings!");  // Error message if file couldn't be opened
    }
}

void loadAndDisplaySplash() {


    // Render the bitmap
    display.clearDisplay();
    display.drawBitmap(0, 0, goblinSplash, 128, 64, SH110X_WHITE);
    display.display();
    delay(1000); // Display for 2 seconds
}



void saveSettings() {
    displayMsg("Saving settings...");  // Display message for feedback
    initializeDeviceSettings();

    char settingsFileName[13] = {"SETTINGS.txt"};  // Name of the settings file
    
    // Create a file path to store the settings file
    char filePath[51]; // Adjust the size to ensure it fits your folder and file names
    File file;  // Declare file for writing

    // Build the file path
    strcpy(filePath, "/");
    strcat(filePath, deviceFolder);  // Add the device folder
    strcat(filePath, "/CONFIG/");    // Add the CONFIG subfolder
    strcat(filePath, settingsFileName);  // Add the settings file name

    // Remove the file if it exists (optional, to overwrite the file)
    if (SD.exists(filePath)) {
        SD.remove(filePath);
    }

    // Open the file for writing
    file = SD.open(filePath, FILE_WRITE);
    if (file) {
        // Loop through the deviceSettings vector
        for (const auto& entry : deviceSettings) {
            // If it's a label, just print the name and move to the next line
            if (entry.label) {
                file.print(entry.name);
                file.println();  // Move to the next line
            } 
            // If it's not a label, print the name and associated values
            else {
                file.print(entry.name);  // Print the entry name
                
                // Check and print each value type if it's not nullptr
                if (entry.valueBool != nullptr) {
                    file.print(" ");
                    file.print(*entry.valueBool ? "ON" : "OFF");  // Print ON/OFF for bool values
                } else if (entry.valueByte != nullptr) {
                    file.print(" ");
                    file.print(*entry.valueByte);  // Print byte value
                } else if (entry.valueInt != nullptr) {
                    file.print(" ");
                    file.print(*entry.valueInt);  // Print int value
                } else if (entry.valueUnLong != nullptr) {
                    file.print(" ");
                    file.print(*entry.valueUnLong);  // Print unsigned long value
                }
                file.println();  // Move to the next line after printing the value
            }
        }

        // Close the file
        file.close();
        displayMsg("Settings saved successfully!");  // Success message
    } else {
        displayMsg("Error saving settings!");  // Error message if file couldn't be opened
    }
    deviceSettings.clear();
    saveTriggersFile();
    saveUserBtnsFile();
}



void populateMorphEnd(const std::vector<CharArray>& fileList, unsigned int selectedFile) {
    morphed = false;
    char patchfileName[MAX_FILE_NAME_LENGTH + 1];
    strncpy(patchfileName, fileList[selectedFile].data(), 20);
    
    patchfileName[sizeof(patchfileName) - 1] = '\0';
    char filePath[51];
    strcpy(filePath, "/");
    strcat(filePath, deviceFolder);
    strcat(filePath, "/PATCHES/");
    strcat(filePath, patchfileName);
    Serial.println("opening file");
    File file = SD.open(filePath);
    if (file) {
        bool endOfFileReached = false;
        while (file.available() && !endOfFileReached) {
            String line = file.readStringUntil('\n');
            if (line.length() == 0) { // Skip empty lines
                endOfFileReached = true;
            }

            String lowerLine = line.substring(0, 7);
            lowerLine.toLowerCase(); // Convert first 7 characters to lowercase

            if (lowerLine == "program") {
                // Skip program change logic; we don't need this part for morphEnd
            } else {
                int values[8]; // Array to store up to 8 values from the line
                int numValues = 0;

                // Parse the line into values
                while (line.length() > 0 && numValues < 8) {
                    int spaceIndex = line.indexOf(' ');
                    if (spaceIndex == -1) {
                        spaceIndex = line.length();
                    }
                    values[numValues++] = line.substring(0, spaceIndex).toInt();
                    if (spaceIndex < line.length()) {
                        line = line.substring(spaceIndex + 1);
                    } else {
                        line = "";
                    }
                }

                // If it's a standard CC message with two values, populate morphEnd
                if (numValues == 2) {
                  midiIndex[values[0]]->morphFinal = values[1];
                }
            }
        }
        file.close();
    } else {
        displayMsg("Error: File not found");
    }
    
}


void populateFileList(std::vector<CharArray>& list, const char* pathSuffix, const char* exclusions[], int numExclusions, int* fileCount, int fileListCapacity) {
    Serial.println("Entering populateFileList");
    char fullPath[100] = "/";
    *fileCount = 0; // Initialize the file count to zero

    // Determine the full path based on the type of files to load
    Serial.print("Checking path suffix: ");
    Serial.println(pathSuffix);
    if (strcmp(pathSuffix, "/SEQUENCES") == 0 || strcmp(pathSuffix, "/TRACKERSEQUENCES") == 0) {
        strcat(fullPath, pathSuffix);
    } else {
        strcat(fullPath, deviceFolder);
        strcat(fullPath, pathSuffix);
    }

    Serial.print("Full path to open: ");
    Serial.println(fullPath);
    File dir = SD.open(fullPath);
    if (!dir) {
        Serial.print("Failed to open directory: ");
        Serial.println(fullPath);
        return;
    }

    list.clear(); // Clear any existing contents in the vector

    bool fileLimitReached = false;
    while (!fileLimitReached) {
        File entry = dir.openNextFile();
        if (!entry) {
            Serial.println("No more files");
            break; // No more files
        }

        if (!entry.isDirectory()) {
            bool exclude = false;
            for (int i = 0; i < numExclusions; i++) {
                if (strcasecmp(entry.name(), exclusions[i]) == 0) {
                    Serial.print("Excluding file: ");
                    Serial.println(entry.name());
                    exclude = true;
                    break;
                }
            }
            if (!exclude) {
                if (*fileCount < fileListCapacity) {
                    CharArray filename;
                    strncpy(filename.data(), entry.name(), filename.size() - 1);
                    filename[filename.size() - 1] = '\0'; // Ensure null termination

                    list.push_back(filename);
                    (*fileCount)++; // Increment the file count for each added file
                    Serial.print("Added file: ");
                    Serial.println(entry.name());
                } else {
                    fileLimitReached = true;
                    displayMsg("File limit reached, not adding more files.");
                    
                }
            }
        }
        entry.close();
    }
    dir.close();
    Serial.println("Exiting populateFileList");
}


void overwriteFile(std::vector<std::array<char, 25>>* selectedList, const char* selectedFileName) {
    char filePath[80];
    File file;

    switch (previousScreen) {
        case PATCHES: {
            if (selectedFile < patchFileCount) {
                strcpy(filePath, "/");
                strcat(filePath, deviceFolder);
                strcat(filePath, "/PATCHES/");
                strcat(filePath, selectedFileName);
                strcat(filePath, ".txt");

                file = SD.open(filePath, FILE_WRITE);
                if (file) {
                  file.print("DEVICE NAME: ");
                  //print the name of the synth on the first line. 
                  file.println(deviceFolder);
                    for (byte i = 0; i < deviceArraySize; ++i) {
                      if(deviceParams[i].obtained == true){
                          if (!reMapAppStatus) {
                              if (deviceParams[i].nrpn == false) {
                                file.print(deviceParams[i].ccNum);
                                file.print(" ");
                                file.println(deviceParams[i].ccVal);
                              }
                              if(deviceParams[i].nrpn == true){
                              file.print("99");
                              file.print(" ");
                              file.print(deviceParams[i].msb);
                              file.print(" ");
                              file.print("98");
                              file.print(" ");
                              file.print(deviceParams[i].lsb);
                              file.print(" ");
                              file.print("6");
                              file.print(" ");
                              file.print(deviceParams[i].msbVal);
                              file.print(" ");
                              file.print("38");
                              file.println(deviceParams[i].lsbVal);
                              }
                            } 
                          if (reMapAppStatus){
                              for(byte i = 0;i < deviceArraySize;i++){
                              if(deviceParams[i].reMapped == true && deviceParams[i].nrpn == false){
                              file.print(deviceParams[i].reMapCc);
                              file.print(" ");
                              file.println(deviceParams[i].ccVal);
                              }
                            }
                          }
                      }
                    }
                  file.close();
                  displayMsg("Patch file overwritten");

                  
                } else {
                    displayMsg("Error opening file for writing");
                }
            }
        break;
        }
        case SEQUENCES: {
            if (sequenceFileCount + 1 > sequenceFileCapacity) {
                Serial.println("Too many files in selected file list");
                return;
            }

            strcpy(filePath, "/SEQUENCES/");
            strcat(filePath, selectedFileName);
            strcat(filePath, ".txt");
            displayMsg("saving...");
            file = SD.open(filePath, FILE_WRITE);
            if (file) {
                file.print("LENGTH ");
                file.println(seqLength); // Write seqLength
                file.print("TEMPO ");
                file.println(sequencerTempo);  // Write seqTempo

                 // Write the sequencer array
                for (byte i = 0; i < 32; i++) {
                    file.print(sequencerArray[i].numNotes);  // Write numNotes
                    file.print(" ");  // Space between numNotes and the first step note

                    // Write the step notes
                    for (byte j = 0; j < 8; j++) {
                        file.print(sequencerArray[i].seqStepNotes[j]);  // Write each step note
                        if (j < 7) {
                            file.print(" ");  // Space between notes, but no space after the last note
                        }
                    }
                    file.print(" ");
                    file.println(seqSwang[i]);  // New line after each sequencer entry
                    
                }

                file.close();  // Close the file
            }
            

            
            

            
        }
        break;
        case REMAPLIST: {
            if (selectedFile < reMapFileCount) {
                strcpy(filePath, "/");
                strcat(filePath, deviceFolder);
                strcat(filePath, "/REMAPS/");
                strcat(filePath, selectedFileName);
                strcat(filePath, ".txt");

                file = SD.open(filePath, FILE_WRITE);
                if (file) {
                    for (byte i = 0; i < numEntries; ++i){
                        if (deviceParams[i].reMapped == true){
                        file.print(deviceParams[i].ccNum);
                        file.print(" ");
                        file.println(deviceParams[i].reMapCc);
                    }
                    }
                    file.close();
                    displayMsg("Remap file overwritten");

                    
                } else {
                    displayMsg("Error opening file for writing");
                }
            }
            break;
        }
        default:
            displayMsg("Unknown screen type");
            break;
    }

    selectedFile = 0;
    selection = 0;
}

void saveNewFile() {
      if (checkFileName(newFileNameArray)) {     
        if(previousScreen == PATCHES){   
        currentScreen = MESSAGE;
        screenSwitch();
        strcpy(currentScreenName, "ERROR");
        displayMsg("Cannot use file name PANEL");
        return;
        }
    }

    bool isNrpn = false;
    char newFileName[25];  // Updated size to 21
    memset(newFileName, '\0', sizeof(newFileName));

    
    // Find the last non-dash character
    int lastNonDashIndex = 19; // Assuming the array size is 20
    while (lastNonDashIndex >= 0 && newFileNameArray[lastNonDashIndex] == '-') {
        lastNonDashIndex--;
    }

    // Copy the valid part of the array to newFileName
    memcpy(newFileName, newFileNameArray, lastNonDashIndex + 1);
    strcat(newFileName, ".txt");

    char filePath[80]; // Size should be enough to hold all concatenated strings plus null terminator
    File file; // Declare file here for use in both cases

    switch (previousScreen) {
        case PATCHES: {
            if (patchFileCount + 1 > patchFileCapacity) {
                Serial.println("Too many files in selected file list");
                return;
            }

            strcpy(filePath, "/");
            strcat(filePath, deviceFolder);
            strcat(filePath, "/PATCHES/");
            strcat(filePath, newFileName);

            if (SD.exists(filePath)) {
                SD.remove(filePath);
            }

            file = SD.open(filePath, FILE_WRITE);
            if (file) {
              //print the name of the synth on the first line. 
              file.print("DEVICE NAME: ");
              file.println(deviceFolder);
                for (byte i = 0; i < deviceArraySize; ++i) {
                  if(deviceParams[i].obtained == true){
                    if (!reMapAppStatus) {
                      //only print values for parameters that have been received or loaded
                      
                        //if the parameter value is a CC, print number and value
                        if(deviceParams[i].nrpn == false){
                          file.print(deviceParams[i].ccNum);
                          file.print(" ");
                          file.println(deviceParams[i].ccVal);
                        }
                        //if the parameter is an nrpn value
                        if(deviceParams[i].nrpn == true){
                          file.print("99");
                          file.print(" ");
                          file.print(deviceParams[i].msb);
                          file.print(" ");
                          file.print("98");
                          file.print(" ");
                          file.print(deviceParams[i].lsb);
                          file.print(" ");
                          file.print("6");
                          file.print(" ");
                          file.print(deviceParams[i].msbVal);
                          file.print(" ");
                          file.print("38");
                          file.print(" ");
                          file.println(deviceParams[i].lsbVal);
                        }
                    }
                    if (reMapAppStatus) {
                      if (deviceParams[i].reMapped == true && deviceParams[i].nrpn == false){
                        file.println(deviceParams[i].reMapCc);
                        file.print(" ");
                        file.println(deviceParams[i].ccVal);
                      }
                    }
                  }
                }
                file.close();
                
            }
            displayMsg("saving...");

            // Update patch list vector and file count
            std::array<char, 25> newFileNameArray;
            std::copy(std::begin(newFileName), std::end(newFileName), newFileNameArray.begin());
            patchList.push_back(newFileNameArray);
            patchFileCount++;

        break;
        }
        case SEQUENCES: {
            if (sequenceFileCount + 1 > sequenceFileCapacity) {
                Serial.println("Too many files in selected file list");
                return;
            }

            strcpy(filePath, "/SEQUENCES/");
            strcat(filePath, newFileName);
            displayMsg("saving...");
            file = SD.open(filePath, FILE_WRITE);
            if (file) {
                file.print("LENGTH ");
                file.println(seqLength); // Write seqLength
                file.print("TEMPO ");
                file.println(sequencerTempo);  // Write seqTempo

                 // Write the sequencer array
                for (byte i = 0; i < 32; i++) {
                    file.print(sequencerArray[i].numNotes);  // Write numNotes
                    file.print(" ");  // Space between numNotes and the first step note

                    // Write the step notes
                    for (byte j = 0; j < 8; j++) {
                        file.print(sequencerArray[i].seqStepNotes[j]);  // Write each step note
                        if (j < 7) {
                            file.print(" ");  // Space between notes, but no space after the last note
                        }
                    }
                    file.print(" ");
                    file.println(seqSwang[i]);  // New line after each sequencer entry
                }

                file.close();  // Close the file
            }
            

            // Update sequence list vector and file count
            std::array<char, 25> newFileNameArray;
            std::copy(std::begin(newFileName), std::end(newFileName), newFileNameArray.begin());
            sequenceList.push_back(newFileNameArray);
            sequenceFileCount++;

            break;
        }
        case REMAPLIST: {
          
            if (reMapFileCount + 1 > reMapFileCapacity) {
                Serial.println("Too many files in selected file list");
                return;
            }
            displayMsg("saving...");
            strcpy(filePath, "/");
            strcat(filePath, deviceFolder);
            strcat(filePath, "/REMAPS/");
            strcat(filePath, newFileName);

            file = SD.open(filePath, FILE_WRITE);
            if (file) {

                for (byte i = 0; i < numEntries; ++i) {
                    if (deviceParams[i].reMapped == true){
                     file.print(deviceParams[i].ccNum);
                     file.print(" ");
                     file.println(deviceParams[i].reMapCc);
                    }
                }
                file.close();
                
            }


            // Update remap list vector and file count
            std::array<char, 25> newFileNameArray;
            std::copy(std::begin(newFileName), std::end(newFileName), newFileNameArray.begin());
            reMapList.push_back(newFileNameArray);
            reMapFileCount++;

            break;
        }
        // Add more cases if necessary

        default:
            // Optional: Handle unknown previousScreen case
            break;
    }

    // Display message about new file creation
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SH110X_WHITE);
    display.setCursor(0, 0);
    display.println("NEW FILE: ");
    display.println(newFileName);
    display.println(" SAVED!");
    display.display();
    alphabetIndex = 0;
    selectedFile = 0;
    selection = 0;
}

//gabagool
void sendPatch(char chosenPatch[]) {
bool fileMatchesDevice = false;
    char patchfileName[25];
    strncpy(patchfileName, chosenPatch, 25);
    patchfileName[sizeof(patchfileName) - 1] = '\0';
    if(!patchLoaded){
      if (!checkFileName2(patchfileName)) {     
  savePanel();
        }
    }
    
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 3);
  display.print("sending: ");
  display.println(selectedDisplayedNames[selection]);
  display.display();

  

    
    char filePath[51];
    strcpy(filePath, "/");
    strcat(filePath, deviceFolder);
    strcat(filePath, "/PATCHES/");
    strcat(filePath, patchfileName);

    File file = SD.open(filePath);
    if (file) {
      String firstLine = file.readStringUntil('\n');
      String prefix = "DEVICE NAME: ";
      if(firstLine.startsWith(prefix)){
        String fileDeviceName = firstLine.substring(prefix.length());
        // Remove leading/trailing whitespace (like newlines or spaces)
        fileDeviceName.trim();  
        if(strcmp(fileDeviceName.c_str(),deviceFolder) == 0){
          fileMatchesDevice = true;
        }else{
          fileMatchesDevice = false;
        }
      }else {
            // Convert first line to lowercase for checking "program"
            String lowerLine = firstLine;
            lowerLine.toLowerCase();

            if (lowerLine.startsWith("program")) {
                int spaceIndex = firstLine.indexOf(' ', 7); // Find space after "program"
                int bankNumber = 0;
                int programNumber;

                if (spaceIndex != -1) {
                    // Extract numbers for program change
                    int nextSpaceIndex = firstLine.indexOf(' ', spaceIndex + 1);
                    if (nextSpaceIndex == -1) {
                        programNumber = firstLine.substring(spaceIndex + 1).toInt();
                    } else {
                        bankNumber = firstLine.substring(spaceIndex + 1, nextSpaceIndex).toInt();
                        programNumber = firstLine.substring(nextSpaceIndex + 1).toInt();

                        if (bankNumber > 0) {
                            int msb = bankNumber >> 7;
                            int lsb = bankNumber & 0x7F;
                            MIDI.sendControlChange(0, msb, patchesOutChannel);  // CC0 for Bank Select MSB
                            delay(1);
                            MIDI.sendControlChange(32, lsb, patchesOutChannel); // CC32 for Bank Select LSB
                            delay(1);
                        }
                    }

                    // Send Program Change message
                    MIDI.sendProgramChange(programNumber, patchesOutChannel);
                    delay(1);
                }

                // Close file after processing the "program" line
                file.close();
                return;
            }
        }



      bool endOfFileReached = false;
      while (file.available() && !endOfFileReached) {
        String line = file.readStringUntil('\n');
        if (line.length() == 0) { // Skip empty lines
          endOfFileReached = true;
        }

        String lowerLine = line.substring(0, 7);
        lowerLine.toLowerCase(); // Convert first 7 characters to lowercase

           // Check if line starts with "sysex"
        if (line.startsWith("sysex")) {
          Serial.println("Detected 'sysex' line"); // Debugging print

          line = line.substring(5); // Remove "sysex" part
          line.trim(); // Trim any remaining spaces
          
          // Fix: Remove any leading commas after "sysex"
          if (line.startsWith(",")) {
            line = line.substring(1); // Remove leading comma if it exists
          }
          line.trim(); // Trim again to ensure no spaces before the first hex value

          Serial.print("Parsed SysEx line: ");
          Serial.println(line); // Debug the remaining part of the line

          // Split the line into SysEx message components
          std::vector<byte> sysexMessage;
          while (line.length() > 0) {
            int commaIndex = line.indexOf(',');
            String part;
            if (commaIndex == -1) {
              part = line; // Last part
              line = "";
            } else {
              part = line.substring(0, commaIndex);
              line = line.substring(commaIndex + 1);
            }
            part.trim(); // Remove spaces
            byte value = (byte)strtol(part.c_str(), NULL, 16); // Convert hex to byte

            // Debugging each parsed byte
            Serial.print("Parsed byte: ");
            Serial.println(value, HEX);

            sysexMessage.push_back(value);
          }

          // Ensure message starts with 0xF0 and ends with 0xF7
          if (sysexMessage.size() > 2 && sysexMessage[0] == 0xF0 && sysexMessage.back() == 0xF7) {
            // Debugging the full message before sending
            Serial.println("Sending SysEx message:");
            for (byte i = 0; i < sysexMessage.size(); i++) {
              Serial.print(sysexMessage[i], HEX);
              Serial.print(" ");
            }
            Serial.println();

            MIDI.sendSysEx(sysexMessage.size(), sysexMessage.data(), true); // Send SysEx message
          } else {
            displayMsg("Invalid SysEx format");
            file.close();
            return;
          }
        } else if  (lowerLine == "program") {
          int spaceIndex = line.indexOf(' ', 7); // Find space after "program"
          int bankNumber = 0;
          int programNumber;

          if (spaceIndex != -1) {
            // There's at least one number
            int nextSpaceIndex = line.indexOf(' ', spaceIndex + 1);
            if (nextSpaceIndex == -1) {
              // Only one number provided (program number)
              programNumber = line.substring(spaceIndex + 1).toInt();
            } else {
              // Two numbers provided (bank and program number)
              bankNumber = line.substring(spaceIndex + 1, nextSpaceIndex).toInt();
              programNumber = line.substring(nextSpaceIndex + 1).toInt();

              if (bankNumber > 0) {
                int msb = bankNumber >> 7;
                int lsb = bankNumber & 0x7F;
                MIDI.sendControlChange(0, msb, patchesOutChannel);  // CC0 for Bank Select MSB
                delay(1);
                MIDI.sendControlChange(32, lsb, patchesOutChannel); // CC32 for Bank Select LSB
                delay(1);
              }
            }

            // Send Program Change message
            MIDI.sendProgramChange(programNumber, patchesOutChannel);
            delay(1);
          }
        } else {
          int values[8]; // Array to store up to 8 values from the line
          int numValues = 0; // Number of values in the line

          // Parse the line into values
          while (line.length() > 0 && numValues < 8) {
            int spaceIndex = line.indexOf(' ');
            if (spaceIndex == -1) { // Last value in the line
              spaceIndex = line.length();
            }
            values[numValues++] = line.substring(0, spaceIndex).toInt();
            if (spaceIndex < line.length()) {
              line = line.substring(spaceIndex + 1);
            } else {
              line = ""; // Clear the line
            }
          }

          if (numValues == 2) {
                delayMicroseconds(10);
                MIDI.sendControlChange(values[0], values[1], patchesOutChannel);
                if(fileMatchesDevice){
                midiIndex[values[0]]->ccVal = values[1];
                midiIndex[values[0]]->obtained = true;
                midiIndex[values[0]]->morphStart = values[1];
                midiIndex[values[0]]->morphCalc = values[1];
                }
          }

          else if (numValues > 2 && !deviceHasNrpn) {
            displayMsg("error, line in patch file has too many values");
            file.close();
            delay(2000);
            return; 
          } else if (numValues == 8 && deviceHasNrpn) {
            delayMicroseconds(10);
            MIDI.sendControlChange(values[0], values[1], patchesOutChannel);
            delayMicroseconds(10);
            MIDI.sendControlChange(values[2], values[3], patchesOutChannel);
            delayMicroseconds(10);
            MIDI.sendControlChange(values[4], values[5], patchesOutChannel);
            delayMicroseconds(10);
            MIDI.sendControlChange(values[6], values[7], patchesOutChannel);
            if(fileMatchesDevice && deviceHasNrpn){
                // Update deviceMsbValues and deviceLsbValues arrays
                for (byte i = 0; i < numEntries; i++) {
                    if(deviceParams[i].nrpn == true && deviceParams[i].msb == values[1] && deviceParams[i].lsb == values[3]){
                    deviceParams[i].obtained = true;
                    deviceParams[i].msbVal = values[5];
                    deviceParams[i].lsbVal = values[7];
                    break;
                    }
                }
            }
          }
        }
      }
      file.close();
      patchLoaded = true;
      morphed = false;
      calcMorph(numEntries,morphPosition);
      // [Your existing finalization code...]
    }
   




}




void sendPatch(const std::vector<CharArray>& fileList, int selectedFile) {
bool fileMatchesDevice = false;
    char patchfileName[MAX_FILE_NAME_LENGTH + 1];
    strncpy(patchfileName, fileList[selectedFile].data(), 20);
    if(!patchLoaded){
      if (!checkFileName2(patchfileName)) {     
  savePanel();
        }
    }
    
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 3);
  display.print("sending: ");
  display.println(selectedDisplayedNames[selection]);
  display.display();

  

    patchfileName[sizeof(patchfileName) - 1] = '\0';
    char filePath[51];
    strcpy(filePath, "/");
    strcat(filePath, deviceFolder);
    strcat(filePath, "/PATCHES/");
    strcat(filePath, patchfileName);

    File file = SD.open(filePath);
    if (file) {
      String firstLine = file.readStringUntil('\n');
      String prefix = "DEVICE NAME: ";
      if(firstLine.startsWith(prefix)){
        String fileDeviceName = firstLine.substring(prefix.length());
        // Remove leading/trailing whitespace (like newlines or spaces)
        fileDeviceName.trim();  
        if(strcmp(fileDeviceName.c_str(),deviceFolder) == 0){
          fileMatchesDevice = true;
        }else{
          fileMatchesDevice = false;
        }
      }else {
        // Convert first line to lowercase for checking "program"
        String lowerLine = firstLine;
        lowerLine.toLowerCase();

        if (lowerLine.startsWith("program")) {
            int spaceIndex = firstLine.indexOf(' ', 7); // Find space after "program"
            int bankNumber = 0;
            int programNumber;

            if (spaceIndex != -1) {
                // Extract numbers for program change
                int nextSpaceIndex = firstLine.indexOf(' ', spaceIndex + 1);
                if (nextSpaceIndex == -1) {
                    programNumber = firstLine.substring(spaceIndex + 1).toInt();
                } else {
                    bankNumber = firstLine.substring(spaceIndex + 1, nextSpaceIndex).toInt();
                    programNumber = firstLine.substring(nextSpaceIndex + 1).toInt();

                    if (bankNumber > 0) {
                        int msb = bankNumber >> 7;
                        int lsb = bankNumber & 0x7F;
                        MIDI.sendControlChange(0, msb, patchesOutChannel);  // CC0 for Bank Select MSB
                        delay(1);
                        MIDI.sendControlChange(32, lsb, patchesOutChannel); // CC32 for Bank Select LSB
                        delay(1);
                    }
                }

                // Send Program Change message
                MIDI.sendProgramChange(programNumber, patchesOutChannel);
                delay(1);
            }

            // Close file after processing the "program" line
            file.close();
            return;
        }
    }



      bool endOfFileReached = false;
      while (file.available() && !endOfFileReached) {
        String line = file.readStringUntil('\n');
        if (line.length() == 0) { // Skip empty lines
          endOfFileReached = true;
        }

        String lowerLine = line.substring(0, 7);
        lowerLine.toLowerCase(); // Convert first 7 characters to lowercase

           // Check if line starts with "sysex"
        if (line.startsWith("sysex")) {
          Serial.println("Detected 'sysex' line"); // Debugging print

          line = line.substring(5); // Remove "sysex" part
          line.trim(); // Trim any remaining spaces
          
          // Fix: Remove any leading commas after "sysex"
          if (line.startsWith(",")) {
            line = line.substring(1); // Remove leading comma if it exists
          }
          line.trim(); // Trim again to ensure no spaces before the first hex value

          Serial.print("Parsed SysEx line: ");
          Serial.println(line); // Debug the remaining part of the line

          // Split the line into SysEx message components
          std::vector<byte> sysexMessage;
          while (line.length() > 0) {
            int commaIndex = line.indexOf(',');
            String part;
            if (commaIndex == -1) {
              part = line; // Last part
              line = "";
            } else {
              part = line.substring(0, commaIndex);
              line = line.substring(commaIndex + 1);
            }
            part.trim(); // Remove spaces
            byte value = (byte)strtol(part.c_str(), NULL, 16); // Convert hex to byte

            // Debugging each parsed byte
            Serial.print("Parsed byte: ");
            Serial.println(value, HEX);

            sysexMessage.push_back(value);
          }

          // Ensure message starts with 0xF0 and ends with 0xF7
          if (sysexMessage.size() > 2 && sysexMessage[0] == 0xF0 && sysexMessage.back() == 0xF7) {
            // Debugging the full message before sending
            Serial.println("Sending SysEx message:");
            for (byte i = 0; i < sysexMessage.size(); i++) {
              Serial.print(sysexMessage[i], HEX);
              Serial.print(" ");
            }
            Serial.println();

            MIDI.sendSysEx(sysexMessage.size(), sysexMessage.data(), true); // Send SysEx message
          } else {
            displayMsg("Invalid SysEx format");
            file.close();
            return;
          }
        } else if  (lowerLine == "program") {
          int spaceIndex = line.indexOf(' ', 7); // Find space after "program"
          int bankNumber = 0;
          int programNumber;

          if (spaceIndex != -1) {
            // There's at least one number
            int nextSpaceIndex = line.indexOf(' ', spaceIndex + 1);
            if (nextSpaceIndex == -1) {
              // Only one number provided (program number)
              programNumber = line.substring(spaceIndex + 1).toInt();
            } else {
              // Two numbers provided (bank and program number)
              bankNumber = line.substring(spaceIndex + 1, nextSpaceIndex).toInt();
              programNumber = line.substring(nextSpaceIndex + 1).toInt();

              if (bankNumber > 0) {
                int msb = bankNumber >> 7;
                int lsb = bankNumber & 0x7F;
                MIDI.sendControlChange(0, msb, patchesOutChannel);  // CC0 for Bank Select MSB
                delay(1);
                MIDI.sendControlChange(32, lsb, patchesOutChannel); // CC32 for Bank Select LSB
                delay(1);
              }
            }

            // Send Program Change message
            MIDI.sendProgramChange(programNumber, patchesOutChannel);
            delay(1);
          }
        } else {
          int values[8]; // Array to store up to 8 values from the line
          int numValues = 0; // Number of values in the line

          // Parse the line into values
          while (line.length() > 0 && numValues < 8) {
            int spaceIndex = line.indexOf(' ');
            if (spaceIndex == -1) { // Last value in the line
              spaceIndex = line.length();
            }
            values[numValues++] = line.substring(0, spaceIndex).toInt();
            if (spaceIndex < line.length()) {
              line = line.substring(spaceIndex + 1);
            } else {
              line = ""; // Clear the line
            }
          }

          if (numValues == 2) {
                delayMicroseconds(10);
                MIDI.sendControlChange(values[0], values[1], patchesOutChannel);
                if(fileMatchesDevice){
                midiIndex[values[0]]->ccVal = values[1];
                midiIndex[values[0]]->obtained = true;
                midiIndex[values[0]]->morphStart = values[1];
                midiIndex[values[0]]->morphCalc = values[1];
                }
          }

          else if (numValues > 2 && !deviceHasNrpn) {
            displayMsg("error, line in patch file has too many values");
            file.close();
            delay(2000);
            return; 
          } else if (numValues == 8 && deviceHasNrpn) {
            delayMicroseconds(10);
            MIDI.sendControlChange(values[0], values[1], patchesOutChannel);
            delayMicroseconds(10);
            MIDI.sendControlChange(values[2], values[3], patchesOutChannel);
            delayMicroseconds(10);
            MIDI.sendControlChange(values[4], values[5], patchesOutChannel);
            delayMicroseconds(10);
            MIDI.sendControlChange(values[6], values[7], patchesOutChannel);
            if(fileMatchesDevice && deviceHasNrpn){
                // Update deviceMsbValues and deviceLsbValues arrays
                for (byte i = 0; i < numEntries; i++) {
                    if(deviceParams[i].nrpn == true && deviceParams[i].msb == values[1] && deviceParams[i].lsb == values[3]){
                    deviceParams[i].obtained = true;
                    deviceParams[i].msbVal = values[5];
                    deviceParams[i].lsbVal = values[7];
                    break;
                    }
                }
            }
          }
        }
      }
      file.close();
      patchLoaded = true;
      morphed = false;
      calcMorph(numEntries,morphPosition);
      // [Your existing finalization code...]
    }
   



}

void savePanel(){
      displayMsg("saving panel values");  
char panelFileName[11] = {"PANEL.txt"};
  

  char filePath[51]; // Size should be enough to hold all concatenated strings plus null terminator
  File file; // Declare file here for use in both cases

      strcpy(filePath, "/");
      strcat(filePath, deviceFolder);
      strcat(filePath, "/PATCHES/");
      strcat(filePath, panelFileName);

      if (SD.exists(filePath)) {
        SD.remove(filePath);
      }

      file = SD.open(filePath, FILE_WRITE);
      if (file) {
        for (byte i = 0; i < deviceArraySize; ++i) {
        if (deviceCcNums[i] != 99 || deviceCcNums[i] == 99 && deviceMsbNums[i] == 255){
          file.print(deviceCcNums[i]);
          file.print(" ");
          file.println(panelCcValues[i]);
        }
        if (deviceCcNums[i] == 99 && deviceMsbNums[i] != 255){
          file.print(deviceCcNums[i]);
            file.print(" ");
            file.print(deviceMsbNums[i]);
            file.print(" ");
            file.print("98 ");
            file.print(deviceLsbNums[i]);
            file.print(" ");
            file.print("6 ");
            file.print(deviceMsbValues[i]);
            file.print(" ");
            file.print("38 ");
            file.println(deviceLsbValues[i]);
        }
        }
        file.close();
        
      }

}

void loadSequence(char sequence[]) {
char filePath[100];
strcpy(filePath, "/SEQUENCES/");
strcat(filePath, sequence);
if (!SD.exists(filePath)) {
            displayMsg("file not found");
            delay(500);
            currentScreen = previousScreen;
            screenSwitch();
        }else{
                // Open the file for reading
                File file = SD.open(filePath);
                if (file) {
                    selection = 0;

                    // Clear and reinitialize the sequencerArray
                    sequencerArray.clear();
                    initiateSequencerArray();  // Reinitialize the sequencer array to default values
                    // Initialize seqSwang to zero
                    memset(seqSwang, 0, sizeof(seqSwang));

                    // Read seqLength (first line)
                    char buffer[50];
                    file.readBytesUntil('\n', buffer, sizeof(buffer));  // Read the LENGTH line
                    sscanf(buffer, "LENGTH %hhu", &seqLength);  // Parse seqLength

                    // Read seqTempo (second line)
                    file.readBytesUntil('\n', buffer, sizeof(buffer));  // Read the TEMPO line
                    sscanf(buffer, "TEMPO %lu", &sequencerTempo);  // Parse sequencerTempo

                    // Now read the sequence data into sequencerArray
                    for (byte i = 0; i < 32; ++i) {
                        if (file.available()) {
                            // Read the entire line for the current step in the sequence
                            file.readBytesUntil('\n', buffer, sizeof(buffer));

                            // Parse numNotes first, then the seqStepNotes array
                            int parsedValues[10];  // numNotes + 8 step notes = 9 values
                            sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d",
                                  &parsedValues[0], &parsedValues[1], &parsedValues[2], &parsedValues[3],
                                  &parsedValues[4], &parsedValues[5], &parsedValues[6], &parsedValues[7],
                                  &parsedValues[8], &parsedValues[9]);

                            sequencerArray[i].numNotes = parsedValues[0];  // First value is numNotes

                            // Set the step notes from parsed values
                            for (byte j = 0; j < 8; ++j) {
                                sequencerArray[i].seqStepNotes[j] = parsedValues[j + 1];
                            }
                            seqSwang[i] = parsedValues[9];
                        }
                    }

                    file.close();
                    displayMsg("Sequence loaded successfully!");
                } else {
                    displayMsg("Error opening sequence file");
                }

                selectedFile = 0;
                
            }
}

void loadSequence(std::vector<CharArray>& fileList, int selectedFile) {

    if (selectedFile < sequenceFileCount) {
        char filePath[75];
        if (currentScreen == SEQUENCES) {
            strcpy(filePath, "/SEQUENCES/");
        }
        char sequenceFileName[MAX_FILE_NAME_LENGTH + 1];
        strncpy(sequenceFileName, fileList[selectedFile].data(), 20);
        strcat(filePath, sequenceFileName);

        // Check if the file exists
        if (SD.exists(filePath)) {
            displayMsg("LOADING...");
        } else {
            displayMsg("SEQUENCE FILE NOT FOUND");
            delay(500);
            currentScreen = previousScreen;
            screenSwitch();
            return;
        }

        // Open the file for reading
        File file = SD.open(filePath);
        if (file) {
            selection = 0;

            // Clear and reinitialize the sequencerArray
            sequencerArray.clear();
            initiateSequencerArray();  // Reinitialize the sequencer array to default values
            // Initialize seqSwang to zero
            memset(seqSwang, 0, sizeof(seqSwang));

            // Read seqLength (first line)
            char buffer[50];
            file.readBytesUntil('\n', buffer, sizeof(buffer));  // Read the LENGTH line
            sscanf(buffer, "LENGTH %hhu", &seqLength);  // Parse seqLength

            // Read seqTempo (second line)
            file.readBytesUntil('\n', buffer, sizeof(buffer));  // Read the TEMPO line
            sscanf(buffer, "TEMPO %lu", &sequencerTempo);  // Parse sequencerTempo

            // Now read the sequence data into sequencerArray
            for (byte i = 0; i < 32; ++i) {
                if (file.available()) {
                    // Read the entire line for the current step in the sequence
                    file.readBytesUntil('\n', buffer, sizeof(buffer));

                    // Parse numNotes first, then the seqStepNotes array
                    int parsedValues[10];  // numNotes + 8 step notes = 9 values
                    sscanf(buffer, "%d %d %d %d %d %d %d %d %d %d",
                           &parsedValues[0], &parsedValues[1], &parsedValues[2], &parsedValues[3],
                           &parsedValues[4], &parsedValues[5], &parsedValues[6], &parsedValues[7],
                           &parsedValues[8], &parsedValues[9]);

                    sequencerArray[i].numNotes = parsedValues[0];  // First value is numNotes

                    // Set the step notes from parsed values
                    for (byte j = 0; j < 8; ++j) {
                        sequencerArray[i].seqStepNotes[j] = parsedValues[j + 1];
                    }
                    seqSwang[i] = parsedValues[9];
                }
            }

            file.close();
            displayMsg("Sequence loaded successfully!");
        } else {
            displayMsg("Error opening sequence file");
        }

        selectedFile = 0;
        currentScreen = SEQUENCER;
        screenSwitch();
    }
}


void deleteFile(std::vector<CharArray>& fileList, unsigned int fileIndex, const char* pathSuffix, int* fileCountPtr) {
    if (fileIndex >= fileList.size()) {
        Serial.println("Invalid file index.");
        return;
    }
    Serial.print("looking for: ");
    Serial.println(fileList[fileIndex].data());
    std::string filePath;
    // Construct the full path for the file
    if (previousScreen == PATCHES || previousScreen == REMAPLIST){
    filePath = "/"; // Adjust the base path as needed
    filePath += deviceFolder;
    filePath += pathSuffix;
    filePath += "/";
    filePath += fileList[fileIndex].data();
    Serial.println(fileList[fileIndex].data());
    }
    if (previousScreen == SEQUENCES){
    filePath = "/SEQUENCES/"; // Adjust the base path as needed
    filePath += fileList[fileIndex].data();
    Serial.println(fileList[fileIndex].data());
    }

    

    // Delete the file from the SD card
    if (SD.exists(filePath.c_str())) {
        if (SD.remove(filePath.c_str())) {
            Serial.println("File deleted successfully from SD card.");
        } else {
            Serial.println("Error deleting file from SD card.");
            return;
        }
    } else {
        Serial.println("File does not exist on SD card.");
        return;
    }

    // Remove the entry from the vector list
    fileList.erase(fileList.begin() + fileIndex);
    if (fileCountPtr) {
        (*fileCountPtr)--; // Decrement the file count via pointer
    }
    Serial.println("Entry removed from the list.");
}

void loadRemap(std::vector<CharArray>& fileList, int selectedFile) {
    if (selectedFile < reMapFileCount) {
        char filePath[75];
        strcpy(filePath, "/");
        strcat(filePath, deviceFolder);
        strcat(filePath, "/REMAPS/");
        char reMapfileName[MAX_FILE_NAME_LENGTH + 1];
        strncpy(reMapfileName, fileList[selectedFile].data(), 20);
        strcat(filePath, reMapfileName);
        
        if (SD.exists(filePath)) {
            displayMsg("LOADING...");
        } else {
            displayMsg("REMAP FILE NOT FOUND");
            delay(500);
            currentScreen = previousScreen;
            screenSwitch();
            return;
        }

        File file = SD.open(filePath);
        if (file) {
            byte linesRead = 0;
            while (file.available() && linesRead < numEntries) {
                String line = file.readStringUntil('\n');
                line.trim(); // Trim any leading or trailing whitespace
                int firstNumber = -1;
                int secondNumber = -1;
                
                int spaceIndex = line.indexOf(' ');
                if (spaceIndex != -1) {
                    firstNumber = line.substring(0, spaceIndex).toInt();
                    secondNumber = line.substring(spaceIndex + 1).toInt();
                }

                if (firstNumber != 255 && secondNumber != 255) {
                    midiIndex[firstNumber]->reMapCc = secondNumber;
                    midiIndex[firstNumber]->reMapped = true;
                   
                } else {
                    Serial.println("Error, remap file line missing value");
                }

                ++linesRead;
            }
            file.close();
        }

        currentScreen = previousScreen;
        screenSwitch();
    }
}

void getSynthMidiInfo() {

  // Construct the path to the DEVICECONFIG.txt file
  String filePath = "/" + String(deviceFolder) + "/CONFIG/MIDI_INFO.txt";
  Serial.println(filePath);

  // Open the file
  File file = SD.open(filePath.c_str());
  if (!file) {
    Serial.println("Error opening MIDI_INFO.txt");
    return;
  }

  // Count the number of lines in the file - the number of lines = the number of parameters defined by the user. 
  int lineCount = 0;
  while (file.available()) {
    if (file.readStringUntil('\n').length() > 0) {
      lineCount++;
      deviceArraySize++;
    }
  }
  Serial.print("Number of lines: ");
  Serial.println(lineCount); // Debug print

  // Reset the file pointer to the beginning of the file
  file.seek(0);

  // Check if lineCount is valid
  if (lineCount <= 0) {
    Serial.println("No entries in MIDI_INFO.txt");
    file.close();
    return;
  }

  // Allocate memory for device arrays
  numEntries = lineCount; // the number of parameter values of the synth
  deviceCcNums = new byte[numEntries];
  deviceMsbNums = new byte[numEntries];
  deviceLsbNums = new byte[numEntries];
  deviceCcValues = new byte[numEntries];
  deviceMsbValues = new byte[numEntries];
  deviceLsbValues = new byte[numEntries];
  panelCcValues = new byte[numEntries];
  panelMsbValues = new byte[numEntries];
  panelLsbValues = new byte[numEntries];
  deviceCcNames = new char[numEntries][15]; // Array for parameter names

  // Populate arrays
  String line;
  for (int i = 0; i < numEntries; i++) {

    DeviceParamsStruct newEntry;
    
    line = file.readStringUntil('\n');
    line.trim(); // Remove any leading/trailing whitespace
    line.toUpperCase(); // Convert the line to uppercase

    // Split line based on whether it starts with "CC" or "NRPN"
    if (line.startsWith("NRPN")) {
      // NRPN entry: Format -> "NRPN [name] [MSB] [LSB]"
      newEntry.index = i;
      deviceHasNrpn = true;
      newEntry.nrpn = true;
      newEntry.obtained = false;

      // Extract the name and MSB/LSB values
      int firstSpace = line.indexOf(' ');
      int secondSpace = line.indexOf(' ', firstSpace + 1);
      int thirdSpace = line.indexOf(' ', secondSpace + 1);

      String name = line.substring(firstSpace + 1, secondSpace);
      name.trim();  // Trim the extracted substring

      byte msb = (byte)line.substring(secondSpace + 1, thirdSpace).toInt();
      byte lsb = (byte)line.substring(thirdSpace + 1).toInt();

      // Copy the name to arrays, ensuring null termination
      name.toCharArray(deviceCcNames[i], 15);
      deviceCcNames[i][14] = '\0'; // Null terminate

      name.toCharArray(newEntry.name, 15);
      newEntry.name[14] = '\0'; // Null terminate

      // Assign MSB and LSB values
      deviceCcNums[i] = 99; // Special value for NRPN
      newEntry.ccNum = 99; // Experimental for NRPN

      deviceMsbNums[i] = msb;
      newEntry.msb = msb;

      deviceLsbNums[i] = lsb;
      newEntry.lsb = lsb;
      

    } else if (line.startsWith("CC")) {
        newEntry.nrpn = false;

  // Skip "CC " (length of "CC " is 3 characters)
  int firstSpace = line.indexOf(' ', 3);  // Find the space after the name
  
  // Extract the name (everything between "CC " and the next space)
  String name = line.substring(3, firstSpace); 
  name.trim();  // Trim the extracted substring

  // Extract the CC number (everything after the space following the name)
  byte ccNum = (byte)line.substring(firstSpace + 1).toInt(); // Get CC number
  
  // Copy the name to arrays, ensuring null termination
  name.toCharArray(deviceCcNames[i], 15);
  deviceCcNames[i][14] = '\0'; // Null terminate

  name.toCharArray(newEntry.name, 15);
  newEntry.name[14] = '\0'; // Null terminate

  // Assign CC number
  deviceCcNums[i] = ccNum;
  newEntry.ccNum = ccNum;

  // Flag non-NRPN values for MSB and LSB
  deviceMsbNums[i] = 255;
  newEntry.msb = 255;

  deviceLsbNums[i] = 255;
  newEntry.lsb = 255;
}


    // Initialize values for corresponding array values
    newEntry.index = i;    
    newEntry.ccVal = 0;
    newEntry.msbVal = 0;
    newEntry.lsbVal = 0;
    newEntry.obtained = false;
    newEntry.reMapped = false;
    newEntry.reMapCc = 255;
    newEntry.panelCcVal = 255;
    newEntry.panelLsbVal = 255;
    newEntry.panelMsbVal = 255;
    newEntry.morphStart = 255;
    newEntry.morphFinal = 255;
    newEntry.morphCalc = 255;
    newEntry.morphStep = 255;
    deviceCcValues[i] = 0;
    panelCcValues[i] = 0;
    deviceMsbValues[i] = 0;
    deviceLsbValues[i] = 0;
    

    panelMsbValues[i] = 0;
    panelLsbValues[i] = 0;

    deviceParams.push_back(newEntry);
  }
  
  linkMidiIndexToDeviceParams();
  file.close();
  Serial.println("Device configuration populated");
}





void getDeviceSettings() {
  String filePath = "/" + String(deviceFolder) + "/CONFIG/DEVICE_SETTINGS.txt";
  File file = SD.open(filePath);
  if (!file) {
    file = SD.open(filePath, FILE_WRITE);
            if (file) {
            file.println("MAX_FILE_NAME_LENGTH 20");
            file.println("patchFileCapacity 220");
            file.println("sequencerFileCapacity 100");
            file.println("remapFileCapacity 20");
            file.println("trackerFileCapacity 20");
            file.println("maxTriggers 25");
            file.close();
            }
    
    
  }

  while (file.available()) {
    String line = file.readStringUntil('\n');
    int idx = line.indexOf(' ');
    if (idx == -1) continue; // Skip if no space found

    String key = line.substring(0, idx);
    int value = line.substring(idx + 1).toInt();

    if (key == "MAX_FILE_NAME_LENGTH") dynamicMaxFileNameLength = value;
    else if (key == "patchFileCapacity") patchFileCapacity = value;
    else if (key == "sequencerFileCapacity") sequenceFileCapacity = value;
    else if (key == "remapFileCapacity") reMapFileCapacity = value;
    else if (key == "trackerFileCapacity") trackerFileCapacity = value;
    else if (key == "maxTriggers") maxTriggers = value;
    
  }

  file.close();
  Serial.println("Settings loaded successfully");
}

void populateDevices() {

  foldersCount = 0;
  File root = SD.open("/");
  byte index = 0;

  // First pass: count valid folders
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break; // No more files, exit loop
    }
    if (entry.isDirectory()) {
      String folderName = entry.name();
      
      // Convert folderName to a temporary lowercase string for comparison
      String folderNameLower = folderName;
      folderNameLower.toLowerCase();
      
      // Check if the folder should be ignored (case-insensitive filtering)
      if (!(folderNameLower.startsWith("system") || folderNameLower.startsWith("sequences") || folderNameLower.startsWith("trackersequences") || folderNameLower.startsWith("config"))) {
        foldersCount++;
      }
    }
    entry.close();
  }

  // Allocate memory for deviceNames
  deviceNames = new char[foldersCount][21];
  
  root = SD.open("/"); // Re-open root directory to iterate again
  index = 0;

  // Second pass: store valid folder names with original case
  while (true) {
    File entry = root.openNextFile();
    if (!entry) {
      break; // No more files, exit loop
    }
    if (entry.isDirectory()) {
      String folderName = entry.name();
      
      // Convert to lowercase for comparison, but store the original case-sensitive name
      String folderNameLower = folderName;
      folderNameLower.toLowerCase();

      // Check if the folder should be ignored (case-insensitive filtering)
      if (!(folderNameLower.startsWith("system") || folderNameLower.startsWith("sequences") || folderNameLower.startsWith("trackersequences") || folderNameLower.startsWith("config"))) {
        // Ensure the folder name fits in the deviceNames array
        if (folderName.length() > 20) {
          folderName.remove(20); // Truncate the string to 20 characters
        }
        strncpy(deviceNames[index], folderName.c_str(), 20);
        deviceNames[index][20] = '\0'; // Ensure null-termination
        index++;
      }
    }
    entry.close();
  }

  // Debugging code
  for (byte i = 0; i < foldersCount; i++) {
    Serial.print("deviceNames[");
    Serial.print(i);
    Serial.print("]: ");
    Serial.println(deviceNames[i]);
  }
}



void printVectorContents(const std::vector<CharArray>& list) {
    Serial.println("Displaying Vector Contents:");
    for (const auto& item : list) {
        Serial.println(item.data());
    }
}

void createFileList(std::vector<CharArray>& fileList, const char* directory, const char** exclusions, size_t exclusionCount, int* fileCount, int fileCapacity, char displayedNames[][MAX_FILE_NAME_LENGTH + 1]) {
    populateFileList(fileList, directory, exclusions, exclusionCount, fileCount, fileCapacity);
    printVectorContents(fileList);
    sortVector(fileList);
    printVectorContents(fileList);
    initializeDisplayedFileNames(fileList, displayedNames);
}

void createPatchList() {
    createFileList(patchList, "/PATCHES", commonExclusions, commonExclusionsSize, &patchFileCount, patchFileCapacity, displayedPatchNames);
}

void createRemapList() {
    createFileList(reMapList, "/REMAPS", commonExclusions, commonExclusionsSize, &reMapFileCount, reMapFileCapacity, displayedReMapFileNames);
}

void createSequenceList() {
    createFileList(sequenceList, "/SEQUENCES", commonExclusions, commonExclusionsSize, &sequenceFileCount, sequenceFileCapacity, displayedSequenceNames);
}

void createTrackerList() {
    createFileList(trackerSequenceList, "/TRACKERSEQUENCES", commonExclusions, commonExclusionsSize, &trackerFileCount, trackerFileCapacity, displayedTrackerFileNames);
}
// helper function for saving triggers(makes the for loop work a bit nicer in savetriggerfile)
String formatTrigger(const TriggerStruct& trigger) {
    String formatted = "INDEX " + String(trigger.index) +
                       " NAME " + String(trigger.name) +
                       " CCNUM " + String(trigger.ccNum) +
                       " CCVAL " + String(trigger.ccVal) +
                       " STATUS " + String(trigger.status) +
                       " FUNCTION " + String(trigger.functionNum) +
                       " APP " + String(trigger.appNum) +
                       " PANIC " + String(trigger.panic) +
                       " PATCH " + String(trigger.triggerPatch) +
                       " SEQUENCE " + String(trigger.triggerSequence);
    return formatted;
}

// helper function for saving triggers(makes the for loop work a bit nicer in savetriggerfile)

String formatUserBtns(const UserButtonStruct& button) {
    String formatted = "BUTTON " + String(button.buttonNumber) +
                       " FUNCTION " + String(button.buttonFunction) +
                       " APP " + String(button.buttonApp) +
                       " PANIC " + String(button.buttonPanic) +
                       " PATCH " + String(button.buttonPatch) +
                       " SEQUENCE " + String(button.buttonSequence);
    return formatted;
}

void saveTriggersFile() {
    String filePath = "/" + String(deviceFolder) + "/CONFIG/TRIGGERS.txt";
    byte tmpTriggerSize = triggerList.size() - 1;

    if (tmpTriggerSize > 0) {
        if (SD.exists(filePath)) {
            SD.remove(filePath);
        }

        File file = SD.open(filePath, FILE_WRITE);

        if (file) {
            for (byte i = 0; i < tmpTriggerSize; i++) {
                file.println(formatTrigger(triggerList[i]));
            }
            file.close();
        }
    }
}

bool parseTriggerLine(const String& line, TriggerStruct& trigger) {
    // Manually reset fields
    trigger.index = 255;
    strcpy(trigger.name, "");
    trigger.ccNum = 255;
    trigger.ccVal = 255;
    trigger.status = false;
    trigger.functionNum = 255;
    trigger.appNum = 255;
    trigger.patch = 999;
    trigger.sequence = 999;
    trigger.panic = false;
    strcpy(trigger.triggerPatch, "");
    strcpy(trigger.triggerSequence, "");

    // Continue parsing as before...
    int index = 0;
    String tokens[24];
    int start = 0, end = 0;

    while ((end = line.indexOf(' ', start)) != -1) {
        tokens[index++] = line.substring(start, end);
        start = end + 1;
    }
    tokens[index++] = line.substring(start); // Add the last token

    if (index >= 20) {
        trigger.index = tokens[1].toInt();
        tokens[3].toCharArray(trigger.name, sizeof(trigger.name));
        trigger.ccNum = tokens[5].toInt();
        trigger.ccVal = tokens[7].toInt();
        trigger.status = tokens[9].toInt() == 1;
        trigger.functionNum = tokens[11].toInt();
        trigger.appNum = tokens[13].toInt();
        trigger.panic = tokens[15].toInt() == 1;
        tokens[17].toCharArray(trigger.triggerPatch, sizeof(trigger.triggerPatch));
        tokens[19].toCharArray(trigger.triggerSequence, sizeof(trigger.triggerSequence));
        return true;
    }

    Serial.println("Failed to parse trigger line: " + line);
    return false;
}

void loadTriggerFile() {
    String filePath = "/" + String(deviceFolder) + "/CONFIG/TRIGGERS.txt";

    if (!SD.exists(filePath)) {
        Serial.println("Trigger file not found!");
        return;
    }

    File file = SD.open(filePath, FILE_READ);
    if (!file) {
        Serial.println("Failed to open trigger file for reading.");
        return;
    }

    // Temporary storage for valid lines
    String validLines = "";
    int newIndex = 0; // To reassign indices for valid entries

    // Clear the trigger list but keep the placeholder "end" entry
    triggerList.clear();
    initTriggerList();

    while (file.available()) {
        String line = file.readStringUntil('\n');
        line.trim(); // Remove extra whitespace

        if (line.length() > 0) {
            TriggerStruct trigger;
            if (parseTriggerLine(line, trigger)) {
                // Check existence of associated files
                bool patchExists = checkFileExists("/" + String(deviceFolder) + "/PATCHES/" + String(trigger.triggerPatch));
                bool sequenceExists = checkFileExists("/SEQUENCES/" + String(trigger.triggerSequence));

                // Add entry only if files exist or if fields are "NOTHING"
                if ((String(trigger.triggerPatch) == "NOTHING" || patchExists) &&
                    (String(trigger.triggerSequence) == "NOTHING" || sequenceExists)) {

                    // Update index for the new entry
                    trigger.index = newIndex;

                    // Update name to match new index
                    snprintf(trigger.name, sizeof(trigger.name), "TRIG_%d", newIndex);

                    triggerList.insert(triggerList.end() - 1, trigger);

                    // Rebuild validLines with updated index and name
                    validLines += "INDEX " + String(trigger.index) +
                                  " NAME " + String(trigger.name) +
                                  " CCNUM " + String(trigger.ccNum) +
                                  " CCVAL " + String(trigger.ccVal) +
                                  " STATUS " + String(trigger.status) +
                                  " FUNCTION " + String(trigger.functionNum) +
                                  " APP " + String(trigger.appNum) +
                                  " PANIC " + String(trigger.panic) +
                                  " PATCH " + String(trigger.triggerPatch) +
                                  " SEQUENCE " + String(trigger.triggerSequence) + "\n";

                    newIndex++;
                } else {
                    Serial.println("Skipping invalid entry due to missing file: " + line);
                }
            }
        }
    }

    file.close();

    // Overwrite triggers.txt with valid lines
    File outFile = SD.open(filePath, FILE_WRITE);
    if (outFile) {
        outFile.print(validLines);
        outFile.close();
    } else {
        Serial.println("Failed to write updated trigger file.");
    }

    Serial.println("Trigger file loaded successfully.");
}

bool checkFileExists(const String& filePath) {
    return SD.exists(filePath);
}

void saveCtrlFile();
void saveUserBtnsFile(){
  String filePath = "/" + String(deviceFolder) + "/CONFIG/USERBTNS.txt";
  
        if (SD.exists(filePath)) {
            SD.remove(filePath);
        }

        File file = SD.open(filePath, FILE_WRITE);

        if (file) {
            for (byte i = 0; i < 3; i++) {
                file.println(formatUserBtns(userButtons[i]));
            }
            file.close();
        }
    
}

void loadUserButtonsFile() {
    String filePath = "/" + String(deviceFolder) + "/CONFIG/USERBTNS.txt";

    if (!SD.exists(filePath)) {
        Serial.println("User Buttons file not found, initializing with defaults.");
        initUserBtns(); // Initialize the vector with defaults
        saveUserBtnsFile(); // Create a default file
        return;
    }

    File file = SD.open(filePath, FILE_READ);
    if (!file) {
        Serial.println("Failed to open User Buttons file for reading.");
        return;
    }

    String validLines = ""; // Temporary storage for valid lines

    for (byte i = 0; i < 3; i++) {
        String line = file.readStringUntil('\n');
        line.trim(); // Remove whitespace

        // Initialize default values for this button
        UserButtonStruct button = {i, 0, 0, false, "NOTHING", "NOTHING"};

        if (line.length() > 0) {
            if (parseUserButtonLine(line, button)) {
                // Validate files for patch and sequence
                bool patchExists = checkFileExists("/" + String(deviceFolder) + "/PATCHES/" + String(button.buttonPatch));
                bool sequenceExists = checkFileExists("/SEQUENCES/" + String(button.buttonSequence));

                if (!patchExists) {
                    Serial.println("Missing patch file: " + String(button.buttonPatch));
                    strcpy(button.buttonPatch, "NOTHING");
                    
                }

                if (!sequenceExists) {
                    Serial.println("Missing sequence file: " + String(button.buttonSequence));
                    strcpy(button.buttonSequence, "NOTHING");
                    
                }
            } else {
                Serial.println("Failed to parse line, resetting to defaults: " + line);
            }
        } else {
            Serial.println("Empty line in file, resetting to defaults.");
        }

        // Update vector entry and file content
        userButtons[i] = button;
        validLines += formatUserBtns(button) + "\n";
    }

    file.close();

    // Overwrite the file with updated content
    File outFile = SD.open(filePath, FILE_WRITE);
    if (outFile) {
        outFile.print(validLines);
        outFile.close();
    } else {
        Serial.println("Failed to write updated User Buttons file.");
    }

    Serial.println("User Buttons file loaded successfully.");
}

bool parseUserButtonLine(const String& line, UserButtonStruct& button) {
    // Reset fields to defaults
    button.buttonFunction = 0;
    button.buttonApp = 0;
    button.buttonPanic = false;
    strcpy(button.buttonPatch, "NOTHING");
    strcpy(button.buttonSequence, "NOTHING");

    String tokens[12]; // Increase size for safety
    int tokenCount = 0;
    int start = 0, end = 0;

    // Tokenize the line by spaces
    while ((end = line.indexOf(' ', start)) != -1 && tokenCount < 12) {
        tokens[tokenCount++] = line.substring(start, end);
        start = end + 1;
    }
    tokens[tokenCount++] = line.substring(start); // Add the last token

    // Map tokens to struct fields
    if (tokenCount >= 12) {
        button.buttonNumber = tokens[1].toInt();
        button.buttonFunction = tokens[3].toInt();
        button.buttonApp = tokens[5].toInt();
        button.buttonPanic = tokens[7].toInt() == 1;
        tokens[9].toCharArray(button.buttonPatch, sizeof(button.buttonPatch));
        tokens[11].toCharArray(button.buttonSequence, sizeof(button.buttonSequence));
        return true;
    }

    return false; // Return false if the line couldn't be parsed
}