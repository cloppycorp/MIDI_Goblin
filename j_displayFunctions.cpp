#include "j_displayFunctions.h"
void drawNrpnMonitor(byte msb, byte lsb, byte msbDat, byte lsbDat){
  if(msb != 255 && lsb != 255 && msbDat != 255 && lsbDat != 255){
  display.setTextColor(SH110X_WHITE);
  defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
  defineListGridColumn(listGridColumnB, 90, 10); // Example x coordinate
  display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
  display.print("NRPN MSG RECEIVED");

  display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
  display.print("Msb Number: ");
  display.setCursor(listGridColumnC[1].x,listGridColumnC[1].y);
  display.print(msb);

  display.setCursor(listGridColumnA[2].x,listGridColumnA[2].y);
  display.print("Msb Value: ");
  display.setCursor(listGridColumnC[2].x,listGridColumnC[2].y);
  display.print(msbDat);

  display.setCursor(listGridColumnA[3].x,listGridColumnA[3].y);
  display.print("Lsb Number: ");
  display.setCursor(listGridColumnC[3].x,listGridColumnC[3].y);
  display.print(lsb);

  display.setCursor(listGridColumnA[4].x,listGridColumnA[4].y);
  display.print("Lsb Value: ");
  display.setCursor(listGridColumnC[4].x,listGridColumnC[4].y);
  display.print(lsbDat);
  }
}
void drawRandomCcMonitor(byte previousCcReceived,byte previousCcValueReceived, byte displayedIncomingMidiChannel){
  display.setTextColor(SH110X_WHITE);
  
  defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
  defineListGridColumn(listGridColumnB, 64, 10); // Example x coordinate 

  display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
  display.print("CC NUMBER: ");
  display.setCursor(listGridColumnB[0].x,listGridColumnB[0].y);
  display.print(previousCcReceived);

  display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
  display.print("CC VALUE: ");
  display.setCursor(listGridColumnB[1].x,listGridColumnB[1].y);
  display.print(previousCcValueReceived);
   
  display.setCursor(listGridColumnA[2].x,listGridColumnA[2].y);
  display.print("CHANNEl: ");
  display.setCursor(listGridColumnB[2].x,listGridColumnB[2].y);
  display.print(displayedIncomingMidiChannel); 
}
void drawMidiNoteMonitor(){
  display.setTextColor(SH110X_WHITE);
  defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
  defineListGridColumn(listGridColumnB, 90, 10); // Example x coordinate
  display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
  display.print("NOTE MSG RECEIVED");

  display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
  display.print("Note byte: ");
  display.setCursor(listGridColumnB[1].x,listGridColumnB[1].y);
  display.print(lastMidiNoteNumberReceived);

  display.setCursor(listGridColumnA[2].x,listGridColumnA[2].y);
  display.print("Music  note: ");
  display.setCursor(listGridColumnB[2].x,listGridColumnB[2].y);
  display.print(byteToNote(lastMidiNoteNumberReceived));
  
  display.setCursor(listGridColumnA[3].x,listGridColumnA[3].y);
  display.print("Velocity: ");
  display.setCursor(listGridColumnB[3].x,listGridColumnB[3].y);
  display.print(lastMidiNoteVelocityReceived);

  display.setCursor(listGridColumnA[4].x,listGridColumnA[4].y);
  display.print("Channel: ");
  display.setCursor(listGridColumnB[4].x,listGridColumnB[4].y);
  display.print(lastMidiNoteChannelReceived);

}
void drawMidiThruScreen(){
  
  display.setTextColor(SH110X_WHITE);
  display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
  display.print("MIDI NOTE THRU: ");
  display.setCursor(listGridColumnB[0].x,listGridColumnB[0].y);
  display.print(midiNoteThru ?  "ON" : "OFF");
  display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
  display.print("MIDI CC THRU: ");
  display.setCursor(listGridColumnB[1].x,listGridColumnB[1].y);
  display.print(midiCcThru ?  "ON" : "OFF"); 
  display.setCursor(listGridColumnA[2].x,listGridColumnA[2].y);
  display.print("NOTE THRU CHANNEL: ");
  display.setCursor(listGridColumnB[2].x,listGridColumnB[2].y);
  display.print(midiNoteThruOutChannel); 
  display.setCursor(listGridColumnA[3].x,listGridColumnA[3].y);
  display.print("CC THRU CHANNEL: ");
  display.setCursor(listGridColumnB[3].x,listGridColumnB[3].y);
  display.print(midiCcThruOutChannel); 
}

void drawMorphScreen(){
  defineListGridColumn(listGridColumnA, 0, 10);
  if(selectedMorphPatch == 999){
  drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, "CHOOSE PATCH", false);
  }
  if(selectedMorphPatch != 999){
    char morphPatchName[24];
    strncpy(morphPatchName, patchList[selectedMorphPatch].data(), 20);
    morphPatchName[20] = '\0';  // Ensure it's null-terminated
    drawBigCell(listGridColumnA[0].x, listGridColumnA[0].y, morphPatchName, false);
  }
  drawBigCell(listGridColumnA[1].x, listGridColumnA[1].y+2, "RESET", false);
  display.setCursor(listGridColumnA[3].x + 21,listGridColumnA[3].y );
  display.print("MORPHNESS");
  drawEncoderBar(listGridColumnA[4].x, listGridColumnA[4].y, selectedValue,0,100);
  
  
}


void scrollTriggerScreenDown(byte indexValue, std::vector<DispTriggerStruct>& destination, std::vector<TriggerStruct>& source) {

  // Shift elements down
  for (byte i = 0; i < 4; i++) {
    destination[i].index = destination[i + 1].index;
    destination[i].name = destination[i + 1].name;
    destination[i].ccNum = destination[i + 1].ccNum;
    destination[i].ccVal = destination[i + 1].ccVal;
    destination[i].status = destination[i + 1].status;
  }
  
  // Set the last element to the new source value
  destination[4].index = &source[indexValue].index;
  destination[4].name = source[indexValue].name;
  destination[4].ccNum = &source[indexValue].ccNum;
  destination[4].ccVal = &source[indexValue].ccVal;
  destination[4].status = &source[indexValue].status;
}

void scrollTriggerScreenUp(byte indexValue, std::vector<DispTriggerStruct>& destination, std::vector<TriggerStruct>& source) {

  // Shift elements up
  for (byte i = 4; i > 0; i--) {
    destination[i].index = destination[i - 1].index;
    destination[i].name = destination[i - 1].name;
    destination[i].ccNum = destination[i - 1].ccNum;
    destination[i].ccVal = destination[i - 1].ccVal;
    destination[i].status = destination[i - 1].status;
  }

  // Set the first element to the new source value
  destination[0].index = &source[indexValue].index;
  destination[0].name = source[indexValue].name;
  destination[0].ccNum = &source[indexValue].ccNum;
  destination[0].ccVal = &source[indexValue].ccVal;
  destination[0].status = &source[indexValue].status;
}


void drawTriggersScreen(){
display.setTextColor(SH110X_WHITE);
//define the grid
defineListGridColumn(listGridColumnA, 1, 10);
defineListGridColumn(listGridColumnB, 49, 10);
defineListGridColumn(listGridColumnC, 73, 10);
defineListGridColumn(listGridColumnD, 102, 10);
Serial.println("trigger grids defined");
//print the contents of the dispTriggerPointers, if index is 255 print the button, if index == null - dont print 
for(byte i = 0; i < 5; i++){
  if(triggerPtrs[i].index != nullptr && *triggerPtrs[i].index != 255){
    display.setCursor(listGridColumnA[i].x,listGridColumnA[i].y);
    display.print(triggerPtrs[i].name);
    display.setCursor(listGridColumnB[i].x,listGridColumnB[i].y);
    display.print(*triggerPtrs[i].ccNum);
    display.setCursor(listGridColumnC[i].x,listGridColumnC[i].y);
    display.print(*triggerPtrs[i].ccVal);
    display.setCursor(listGridColumnD[i].x,listGridColumnD[i].y);
    display.print((*triggerPtrs[i].status) ?  "ON" : "OFF");
  }
  if(triggerPtrs[i].index != nullptr && *triggerPtrs[i].index == 255){
     drawBigCell(listGridColumnA[i].x-1, listGridColumnA[i].y, "+", false);
  }
}
}


void scrollDispCtrlrUp(byte indexValue, std::vector<CtrlrDispStruct>& destination, std::vector<DeviceParamsStruct>& source) {

  // Shift elements up
  for (byte i = 4; i > 0; i--) {
    destination[i].index = destination[i - 1].index;
    destination[i].ccName = destination[i - 1].ccName;
    destination[i].ccNum = destination[i - 1].ccNum;
    destination[i].ccVal = destination[i - 1].ccVal;
    destination[i].obtainedVal = destination[i - 1].obtainedVal;
  }

  // Set the first element to the new source value
  destination[0].index = &source[indexValue].index;
  destination[0].ccName = source[indexValue].name;
  destination[0].ccNum = &source[indexValue].ccNum;
  destination[0].ccVal = &source[indexValue].ccVal;
  destination[0].obtainedVal = &source[indexValue].obtained;
}

void scrollDispCtrlrDown(byte indexValue, std::vector<CtrlrDispStruct>& destination, std::vector<DeviceParamsStruct>& source) {
  
  // Shift elements down
  for (byte i = 0; i < 4; i++) {
    destination[i].index = destination[i + 1].index;
    destination[i].ccName = destination[i + 1].ccName;
    destination[i].ccNum = destination[i + 1].ccNum;
    destination[i].ccVal = destination[i + 1].ccVal;
    destination[i].obtainedVal = destination[i + 1].obtainedVal;
  }
  
  // Set the last element to the new source value
  destination[4].index = &source[indexValue].index;
  destination[4].ccName = source[indexValue].name;
  destination[4].ccNum = &source[indexValue].ccNum;
  destination[4].ccVal = &source[indexValue].ccVal;
  destination[4].obtainedVal = &source[indexValue].obtained;
}


void scrollDispCtrlrUp(byte indexValue, std::vector<CtrlrDispStruct>& destination, std::vector<CtrlrStruct>& source) {

  // Shift elements up
  for (byte i = 4; i > 0; i--) {
    destination[i].index = destination[i - 1].index;
    destination[i].ccName = destination[i - 1].ccName;
    destination[i].ccNum = destination[i - 1].ccNum;
    destination[i].ccVal = destination[i - 1].ccVal;
    
  }

  // Set the first element to the new source value
  destination[0].index = &source[indexValue].index;
  destination[0].ccName = source[indexValue].ccName;
  destination[0].ccNum = &source[indexValue].ccNum;
  destination[0].ccVal = &source[indexValue].ccVal;
  
}

void scrollDispCtrlrDown(byte indexValue, std::vector<CtrlrDispStruct>& destination, std::vector<CtrlrStruct>& source) {
  
  // Shift elements down
  for (byte i = 0; i < 4; i++) {
    destination[i].index = destination[i + 1].index;
    destination[i].ccName = destination[i + 1].ccName;
    destination[i].ccNum = destination[i + 1].ccNum;
    destination[i].ccVal = destination[i + 1].ccVal;
  }
  
  // Set the last element to the new source value
  destination[4].index = &source[indexValue].index;
  destination[4].ccName = source[indexValue].ccName;
  destination[4].ccNum = &source[indexValue].ccNum;
  destination[4].ccVal = &source[indexValue].ccVal;
}




void drawChordEntryScreen(byte selectedArrayEntry) {
  
  
  if (chordResetPressed) {
    memset(tempChord, 255, sizeof(tempChord));
    memset(sequencerArray[selectedArrayEntry].seqStepNotes, 255, sizeof(sequencerArray[selectedArrayEntry].seqStepNotes));
    sequencerArray[selectedArrayEntry].numNotes = 0;
    chordResetPressed = false;
    DS = false;
  }
  
  bool entryEmpty;
  entryEmpty = true;

  // draw the instructions text
  display.setTextColor(SH110X_WHITE);
  display.setCursor(chordEntryGrid[8].x, chordEntryGrid[8].y);
  display.print("ENTER A CHORD");

  // draw a box around the displayed chord entry area
  display.drawRect(chordEntryGrid[9].x, chordEntryGrid[9].y, 128, 11, SH110X_WHITE);

  //check to see if there is any value already saved in the array for this selected position by checking first value
  
  if (sequencerArray[selectedArrayEntry].seqStepNotes[0] != 255) {
    entryEmpty = false;
  }


  //if theres data in this array entry, copy the remaining values from the selected array entry into the tempoChord array
  if (!entryEmpty) {
    for (byte i = 0; i < 8; i++) {
      tempChord[i] = sequencerArray[selectedArrayEntry].seqStepNotes[i];
    }
  }

  //print the values of the temp array into the chord entry box
  for (byte i = 0; i < 8; i++) {
    display.setCursor(chordEntryGrid[i].x, chordEntryGrid[i].y);
    if (tempChord[i] == 255) {
      display.print("--");
    }

    if (tempChord[i] != 255) {
      display.print(byteToNote(tempChord[i]));
      
    }

  }


  drawSmallCell(chordEntryGrid[10].x, chordEntryGrid[10].y, chordEntryGrid[11].x, chordEntryGrid[11].y, 63, 11, "RESET", false);
  drawSmallCell(chordEntryGrid[12].x, chordEntryGrid[12].y, chordEntryGrid[13].x, chordEntryGrid[13].y, 62, 11, "CANCEL", false);

  drawBigCell(chordEntryGrid[14].x, chordEntryGrid[14].y, "CONFIRM", false);


}


void drawFunctionSmily(Grid* gridArray, byte x, int index, bool selected){
    display.setTextColor(SH110X_WHITE);
  defineListGridColumn(gridArray, x, 10); // Example x coordinate
  display.setCursor(gridArray[index].x, gridArray[index].y);
  if(!selected){
  display.write(0x07);     
  }
  if(selection){
  display.write(0x08);       
  }
}
void drawFunctionSmily(Grid* gridArray, byte x, int* selectedButtonSelectedFunction, bool selected){
    display.setTextColor(SH110X_WHITE);
    const byte index = *selectedButtonSelectedFunction;
  defineListGridColumn(gridArray, x, 10); // Example x coordinate
  display.setCursor(gridArray[index].x, gridArray[index].y);
  if(!selected){
  display.write(0x07);     
  }
  if(selection){
  display.write(0x08);       
  }
}

void drawSelectedTriggerApp(Grid* gridArray, byte x, byte selectedTriggerSelectedFunction, bool selected){
    display.setTextColor(SH110X_WHITE);
  defineListGridColumn(gridArray, x, 10); // Example x coordinate
  display.setCursor(gridArray[selectedTriggerSelectedFunction].x, gridArray[selectedTriggerSelectedFunction].y);
  if(!selected){
  display.write(0x07);     
  }
  if(selection){
  display.write(0x08);       
  }
}




void displayMsg(const char *message) {
  //basic message displayer
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(3, 3);
  display.drawRect(0, 0, 128, 64, SH110X_WHITE);
  display.println(message);
  display.display();
}

void overwriteConfirm() {
  display.setCursor(0, HEADER_AREA_HEIGHT + 2);
  display.setTextColor(SH110X_WHITE);
  display.print("Overwrite: ");
   display.print(selectedDisplayedNames[selection]);
  display.println(" ?");
  display.println(" ");
  display.println("YES or NO");
}




void displayIntArrayContents(int* array, int size) {
  //for debugging
    Serial.println("Displaying Integer Array Contents:");
    for (int i = 0; i < size; i++) {
        Serial.print("Index ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(array[i]);
    }
}

void displayStringArrayContents(char** array, int size) {
  //for debugging
    Serial.println("Displaying String Array Contents:");
    for (int i = 0; i < size; i++) {
        Serial.print("Index ");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(array[i]);  // Assumes null-terminated strings
    }
}


void drawEncoderBar(byte encoderBarX, byte encoderBarY, int* selectedvalue, int encoderBarMin, int encoderBarMax) {

    encoderBarVisible = true;

    int encoderBarValue;

    byte encoderBarTextX;
    byte encoderBarTextY;

    // Map the selectedTempo value to a 0-100 range using global tempoMin and tempoMax


    encoderBarValue = (mapUL(*selectedvalue, encoderBarMin, encoderBarMax, 0, 100));  
    
    
    // Draw black background for tempo bar
    display.fillRect(encoderBarX, encoderBarY, TEMPOBAR_WIDTH, TEMPOBAR_HEIGHT, SH110X_BLACK);
    // Draw the white border for the "tempoBar" area
    display.drawRect(encoderBarX, encoderBarY, TEMPOBAR_WIDTH, TEMPOBAR_HEIGHT, SH110X_WHITE);

    // Determine where you want the tempo bar numeric value to be
    encoderBarTextX = encoderBarX + TEMPOBAR_WIDTH + 3;
    encoderBarTextY = encoderBarY;

    // Calculate the width of the white fill based on the mapped tempo value
    int fillWidth = map(encoderBarValue, 0, 100, 0, TEMPOBAR_WIDTH - 2);

    // Draw the white fill for the "tempoBar" area
    display.fillRect(encoderBarX + 1, encoderBarY + 1, fillWidth, TEMPOBAR_HEIGHT - 2, SH110X_WHITE);

    // Draw the black background for the tempo bar numeric area
    display.fillRect(encoderBarTextX, encoderBarTextY, 22, 8, SH110X_BLACK);

    // Set the text color to white for the tempo numeric
    display.setTextColor(SH110X_WHITE);

    // Display the mapped tempo value (0-100)
    display.setTextSize(1);
    display.setCursor(encoderBarTextX, encoderBarTextY); // Adjust the position based on your preference
    display.print(encoderBarValue);
}

void drawSwangBar(byte swangBarX, byte swangBarY, int8_t* swangValue, int8_t seqSwangMin, int8_t seqSwangMax) {

    swangBarVisible = true;
    byte swangBarWidth = 123;   // Total width of the bar including edges and middle
    byte swangBarMiddle = 61;   // Middle pixel of the bar (center point)
    byte swangBarHeight = 8;
    byte swangBarTextX;
    byte swangBarTextY;

   
    int tempSwangMin = seqSwangMin;
    int tempSwangMax = seqSwangMax;
    int tempSwangValue = *swangValue;
    int swangFillWidth;  // Dereference the pointer to get the value

    // Draw black background for swang bar
    display.fillRect(swangBarX, swangBarY, swangBarWidth, swangBarHeight, SH110X_BLACK);

    // Draw the white border for the swang bar area
    display.drawRect(swangBarX, swangBarY, swangBarWidth, swangBarHeight, SH110X_WHITE);

    // Draw middle line of swang bar
    display.drawLine(swangBarX + swangBarMiddle, swangBarY, swangBarX + swangBarMiddle, swangBarY + swangBarHeight-1, SH110X_WHITE);

    // Determine the text position
    swangBarTextX = (swangBarX + swangBarWidth) / 2;
    swangBarTextY = swangBarY + swangBarHeight + 2;

    // Map the swangValue (-50 to 50) to the fill width (-60 to 60)
    swangFillWidth = map(tempSwangValue, tempSwangMin, tempSwangMax, -60, 60);

    // Draw the white fill for the swang area, check if it's positive or negative
    if (swangFillWidth > 0) {
        // Fill to the right of the middle if positive
        display.fillRect(swangBarX + swangBarMiddle, swangBarY, swangFillWidth, swangBarHeight, SH110X_WHITE);
    } else if(swangFillWidth < 0){
        // Fill to the left of the middle if negative
       display.fillRect(swangBarX + swangBarMiddle + swangFillWidth, swangBarY, -swangFillWidth, swangBarHeight, SH110X_WHITE);
    } 

    // Draw the black background for the swang value text area
    display.fillRect(swangBarTextX, swangBarTextY, 30, 8, SH110X_BLACK);

    // Set the text color and display the swang value
    display.setTextColor(SH110X_WHITE);
    display.setTextSize(1);
    display.setCursor(swangBarTextX, swangBarTextY);
    
    display.print(*swangValue);  // Print the swang value
}


void drawTempoBar(byte tempoBarX, byte tempoBarY, unsigned long* selectedTempo) {
    tempoBarVisible = true;
    unsigned long tempoBarTempo;
    byte tempoBarTextX;
    byte tempoBarTextY;

    // Map the selectedTempo value to a 0-100 range using global tempoMin and tempoMax
    
    tempoBarTempo = 100 -(mapUL(*selectedTempo, tempoMin, tempoMax, 0, 100));
    
    // Draw black background for tempo bar
    display.fillRect(tempoBarX, tempoBarY, TEMPOBAR_WIDTH, TEMPOBAR_HEIGHT, SH110X_BLACK);
    // Draw the white border for the "tempoBar" area
    display.drawRect(tempoBarX, tempoBarY, TEMPOBAR_WIDTH, TEMPOBAR_HEIGHT, SH110X_WHITE);

    // Determine where you want the tempo bar numeric value to be
    tempoBarTextX = tempoBarX + TEMPOBAR_WIDTH + 3;
    tempoBarTextY = tempoBarY;

    // Calculate the width of the white fill based on the mapped tempo value
    int fillWidth = map(tempoBarTempo, 0, 100, 0, TEMPOBAR_WIDTH - 2);

    // Draw the white fill for the "tempoBar" area
    display.fillRect(tempoBarX + 1, tempoBarY + 1, fillWidth, TEMPOBAR_HEIGHT - 2, SH110X_WHITE);

    // Draw the black background for the tempo bar numeric area
    display.fillRect(tempoBarTextX, tempoBarTextY, 22, 8, SH110X_BLACK);

    // Set the text color to white for the tempo numeric
    display.setTextColor(SH110X_WHITE);

    // Display the mapped tempo value (0-100)
    display.setTextSize(1);
    display.setCursor(tempoBarTextX, tempoBarTextY); // Adjust the position based on your preference
    display.print(tempoBarTempo);
}

void drawSmallWhiteBox(byte x,byte y, char boxContent ){
// blink box for single characters
//since single characters are always 5x7 the box width and heigh will always be 7x9
byte boxWidth = 6;
byte boxHeight = 9;
display.fillRect(x-1 , y-1 , boxWidth + 2, boxHeight + 2, SH110X_WHITE);  
display.setTextColor(SH110X_BLACK); 
display.setCursor(x+1, y);
display.print(boxContent);

}
void drawSmallWhiteBox(byte x,byte y, const char* boxContent ){
// blink box for single characters
//since single characters are always 5x7 the box width and heigh will always be 7x9
byte boxWidth = 7;
byte boxHeight = 9;
display.fillRect(x-1 , y-1 , boxWidth + 2, boxHeight + 2, SH110X_WHITE);  
display.setTextColor(SH110X_BLACK); 
display.setCursor(x, y);
display.print(boxContent);

}
void drawBigCell(byte topLeftPixelx, byte topLeftPixely, const char* content, bool selected){
  //the gettextbounds doesnt work
int x = topLeftPixelx;
int y = topLeftPixely;
int16_t x1, y1;
uint16_t w, h;
display.fillRect(topLeftPixelx ,topLeftPixely,SCREENWIDTH,TABLECELLHEIGHT,SH110X_BLACK);
if(!selected){
  display.drawRect(topLeftPixelx ,topLeftPixely,SCREENWIDTH,TABLECELLHEIGHT,SH110X_WHITE);
}
if(selected){
  display.fillRect(topLeftPixelx ,topLeftPixely,SCREENWIDTH,TABLECELLHEIGHT,SH110X_WHITE);
}
if(!selected){
  display.setTextColor(SH110X_WHITE);   
}
if(selected){
  display.setTextColor(SH110X_BLACK);   
}
  display.getTextBounds(content, x, y, &x1, &y1, &w, &h);
  display.setCursor((SCREENWIDTH - w)/2, topLeftPixely + 2);
  display.print(content);
}

void drawBigCell(byte topLeftPixelx, byte topLeftPixely,byte number1, const char* words, byte number2, bool selected){
  //the gettextbounds doesnt work
int x = topLeftPixelx;
int y = topLeftPixely;
int16_t x1, y1;
uint16_t w, h;

    std::string number1Str = std::to_string(number1);
    std::string wordsStr = std::string(words);
    std::string number2Str = std::to_string(number2);

    std::string contentStr = number1Str + wordsStr + number2Str;

    // If you need to use a const char* again, you can do:
    const char* content = contentStr.c_str();
    
display.fillRect(topLeftPixelx ,topLeftPixely,SCREENWIDTH,TABLECELLHEIGHT,SH110X_BLACK);
if(!selected){
  display.drawRect(topLeftPixelx ,topLeftPixely,SCREENWIDTH,TABLECELLHEIGHT,SH110X_WHITE);
}
if(selected){
  display.fillRect(topLeftPixelx ,topLeftPixely,SCREENWIDTH,TABLECELLHEIGHT,SH110X_WHITE);
}
if(!selected){
  display.setTextColor(SH110X_WHITE);   
}
if(selected){
  display.setTextColor(SH110X_BLACK);   
}
  display.getTextBounds(content, x, y, &x1, &y1, &w, &h);
  display.setCursor((SCREENWIDTH - w)/2, topLeftPixely + 2);
  display.print(content);
}
void drawSmallCell(byte boxX, byte boxY,byte textX, byte textY, byte boxWidth, byte boxHeight,const char* words, bool boxSelected){
//first draw the box. 
        if(!boxSelected){
        display.fillRect(boxX, boxY, boxWidth, boxHeight, SH110X_BLACK);
        display.drawRect(boxX, boxY, boxWidth, boxHeight, SH110X_WHITE);
        display.setTextColor(SH110X_WHITE);
        }else{
        
        display.fillRect(boxX, boxY, boxWidth, boxHeight, SH110X_WHITE);
        display.setTextColor(SH110X_BLACK);          
        }
        display.setCursor(textX, textY);
        display.print(words);
}
void drawSmallCell(byte boxX, byte boxY,byte textX, byte textY, byte boxWidth, byte boxHeight,byte value, bool boxSelected){
//first draw the box. 
        if(!boxSelected){
        display.fillRect(boxX, boxY, boxWidth, boxHeight, SH110X_BLACK);
        display.drawRect(boxX, boxY, boxWidth, boxHeight, SH110X_WHITE);
        display.setTextColor(SH110X_WHITE);
        }else{
        
        display.fillRect(boxX, boxY, boxWidth, boxHeight, SH110X_WHITE);
        display.setTextColor(SH110X_BLACK);          
        }
        display.setCursor(textX, textY);
        display.print(value);
}
void drawWhiteBox(byte x,byte y, const char* boxContent ){
        int16_t x1, y1;
        uint16_t w, h;
display.getTextBounds(boxContent, x, y, &x1, &y1, &w, &h);
        display.fillRect(x1 - 1, y1 - 1, w + 1, h + 1, SH110X_WHITE);
        display.setCursor(x, y);
        display.setTextColor(SH110X_BLACK);
       display.println(boxContent);
}
void drawWhiteBox(byte x, byte y, byte value) {
    int16_t x1, y1;
    uint16_t w, h;
    char boxContent[4]; // Enough to hold up to 3 digits and a null terminator
    itoa(value, boxContent, 10); // Convert byte to string (base 10)
    display.getTextBounds(boxContent, x, y, &x1, &y1, &w, &h);
    display.fillRect(x1 - 1, y1 - 1, w + 1, h + 1, SH110X_WHITE);
    display.setCursor(x, y);
    display.setTextColor(SH110X_BLACK);
    display.print(boxContent); // Use print to display the byte as a string
}

void drawWhiteBox(byte x, byte y, unsigned long value) {
    int16_t x1, y1;
    uint16_t w, h;
    char boxContent[4]; // Enough to hold up to 3 digits and a null terminator
    itoa(value, boxContent, 10); // Convert byte to string (base 10)
    display.getTextBounds(boxContent, x, y, &x1, &y1, &w, &h);
    display.fillRect(x1 - 1, y1 - 1, w + 1, h + 1, SH110X_WHITE);
    display.setCursor(x, y);
    display.setTextColor(SH110X_BLACK);
    display.print(boxContent); // Use print to display the byte as a string
}

//overloaded drawWhitebox to accept weird characters fromt he adafruit gfx library
void drawWhiteBox(byte x, byte y, char specialChar) {
    int16_t x1, y1;
    uint16_t w, h;
    char boxContent[2] = { specialChar, '\0' }; // Create a string with the special character
    display.getTextBounds(boxContent, x, y, &x1, &y1, &w, &h);
    display.fillRect(x1 - 1, y1 - 1, w + 1, h + 1, SH110X_WHITE);
    display.setCursor(x, y);
    display.setTextColor(SH110X_BLACK);
    display.write(specialChar); // Use write to print the special character
}

void updateSelectedTempo(int step, unsigned long* selectedTempo) {
    // Calculate step value based on range using float for precision
    float stepValue = (float)(tempoMax - tempoMin) / 100.0;

    // Update the selectedTempo based on the step
    if (step > 0) {
        if (*selectedTempo > tempoMin) {
            *selectedTempo -= (unsigned long)(stepValue * step); // Decrease the selectedTempo to increase speed
            if (*selectedTempo < tempoMin) {
                *selectedTempo = tempoMin;
            }
        }
    } else if (step < 0) {
        if (*selectedTempo < tempoMax) {
            *selectedTempo += (unsigned long)(stepValue * (-step)); // Increase the selectedTempo to decrease speed
            if (*selectedTempo > tempoMax) {
                *selectedTempo = tempoMax;
            }
        }
    }
}

void updateEncoderBar(int step, int* selectedValue, int valueMin, int valueMax) {
  if (morphFileChosen && currentScreen == MORPH){
    // Calculate step value based on range using float for precision
    float stepValue = (float)(valueMax - valueMin) / 100.0;

    // Update the selectedTempo based on the step
    if (step > 0) {
        if (*selectedValue > valueMin) {
            *selectedValue -= (unsigned long)(stepValue * step); // Decrease the selectedTempo to increase speed
            if (*selectedValue < valueMin) {
                *selectedValue = valueMin;
            }
        }
    } else if (step < 0) {
        if (*selectedValue < valueMax) {
            *selectedValue += (unsigned long)(stepValue * (-step)); // Increase the selectedTempo to decrease speed
            if (*selectedValue > valueMax) {
                *selectedValue = valueMax;
            }
        }
    }
  }
}


void drawTable(byte topLeftPixelx, byte topLeftPixely, byte rows){
  //this function draws the table and changes the position of the table gridpoints based on where the table is drawn 
  //DRAW THE LINES OF THE TABLE
  for(byte i = 0; i < rows; i++){
  display.drawRect(topLeftPixelx ,topLeftPixely + (i * (TABLECELLHEIGHT - 1) ),SCREENWIDTH,TABLECELLHEIGHT,SH110X_WHITE);
  display.drawLine(SCREENWIDTH / 2,topLeftPixely + (i * (TABLECELLHEIGHT -1)),SCREENWIDTH / 2,topLeftPixely + (i *( TABLECELLHEIGHT - 1))+ TABLECELLHEIGHT - 1,SH110X_WHITE);
  //SET THE GRID VALUES FOR THE TABLE
  tableColumnA[i].x = topLeftPixelx + 2;
  tableColumnA[i].y = topLeftPixely + (i * (TABLECELLHEIGHT - 1) ) +2;
  tableColumnB[i].x = (SCREENWIDTH / 2) + 2;
  tableColumnB[i].y = topLeftPixely + (i * (TABLECELLHEIGHT -1)) + 2;
  }
  
}

//for this table, try overload so you can plug in different things. 
void drawTableContents(
const char* tableColumnA_0,const char* tableColumnB_0,
const char* tableColumnA_1,const char* tableColumnB_1,
const char* tableColumnA_2,const char* tableColumnB_2,
const char* tableColumnA_3,const char* tableColumnB_3,
const char* tableColumnA_4,const char* tableColumnB_4,
byte rows){
  display.setTextColor(SH110X_WHITE);
  if (rows > 0){
  display.setCursor(tableColumnA[0].x,tableColumnA[0].y);
  display.print(tableColumnA_0);
  display.setCursor(tableColumnB[0].x,tableColumnB[0].y);
  display.print(tableColumnB_0);
  }
  if (rows >= 2){
  display.setCursor(tableColumnA[1].x,tableColumnA[1].y);
  display.print(tableColumnA_1); 
  display.setCursor(tableColumnB[1].x,tableColumnB[1].y);
  display.print(tableColumnB_1);
  }
  if(rows >= 3){
  display.setCursor(tableColumnA[2].x,tableColumnA[2].y);
  display.print(tableColumnA_2);
  display.setCursor(tableColumnB[2].x,tableColumnB[2].y);
  display.print(tableColumnB_2);
  }
  if(rows >= 4){   
  display.setCursor(tableColumnA[3].x,tableColumnA[3].y);
  display.print(tableColumnA_3);
  display.setCursor(tableColumnB[3].x,tableColumnB[3].y);
  display.print(tableColumnB_3);
  }
  if (rows == 5){
  display.setCursor(tableColumnA[4].x,tableColumnA[4].y);
  display.print(tableColumnA_4);
  display.setCursor(tableColumnB[4].x,tableColumnB[4].y);
  display.print(tableColumnB_4);
  }


}

void scrollMonitorUp(byte selectedDeviceParam) {
  byte* tmpByteA;
  byte* tmpByteB;
  bool* tmpBool;

if(deviceParams[selectedDeviceParam].nrpn == false){
  tmpByteA = &deviceParams[selectedDeviceParam].ccNum;
  tmpByteB = &deviceParams[selectedDeviceParam].ccVal;
  
}
if(deviceParams[selectedDeviceParam].nrpn == true){
  tmpByteA = &deviceParams[selectedDeviceParam].msbVal;
  tmpByteB = &deviceParams[selectedDeviceParam].lsbVal;
  
}
tmpBool = &deviceParams[selectedDeviceParam].obtained;
strncpy(deviceMonitorNames[4],deviceMonitorNames[3],15);
strncpy(deviceMonitorNames[3],deviceMonitorNames[2],15);
strncpy(deviceMonitorNames[2],deviceMonitorNames[1],15);
strncpy(deviceMonitorNames[1],deviceMonitorNames[0],15);
strncpy(deviceMonitorNames[0],deviceParams[selectedDeviceParam].name,15);
deviceMonitorNames[0][14]='\0';

  deviceMonitorByteColumnA[4] = deviceMonitorByteColumnA[3];
  deviceMonitorByteColumnA[3] = deviceMonitorByteColumnA[2];
  deviceMonitorByteColumnA[2] = deviceMonitorByteColumnA[1];
  deviceMonitorByteColumnA[1] = deviceMonitorByteColumnA[0];
  deviceMonitorByteColumnA[0] = tmpByteA;

  
  deviceMonitorByteColumnB[4] = deviceMonitorByteColumnB[3];
  deviceMonitorByteColumnB[3] = deviceMonitorByteColumnB[2];
  deviceMonitorByteColumnB[2] = deviceMonitorByteColumnB[1];
  deviceMonitorByteColumnB[1] = deviceMonitorByteColumnB[0];
  deviceMonitorByteColumnB[0] = tmpByteB;
  
  deviceMonitorObtained[4] = deviceMonitorObtained[3];
  deviceMonitorObtained[3] = deviceMonitorObtained[2];
  deviceMonitorObtained[2] = deviceMonitorObtained[1];
  deviceMonitorObtained[1] = deviceMonitorObtained[0];
  deviceMonitorObtained[0] = tmpBool;


}



void scrollListUp(byte selection,byte dislayedArray[5], byte selectedArray[5]){
  byte tempArrayValue = selectedArray[selection];
  
  dislayedArray[4] = dislayedArray[3];
  dislayedArray[3] = dislayedArray[2];
  dislayedArray[2] = dislayedArray[1];
  dislayedArray[1] = dislayedArray[0];
  dislayedArray[0] = tempArrayValue;
  
}

void scrollListDown(byte selection,byte dislayedArray[5], byte selectedArray[5]){
  byte tempArrayValue = selectedArray[selection];
  
  dislayedArray[0] = dislayedArray[1];
  dislayedArray[1] = dislayedArray[2];
  dislayedArray[2] = dislayedArray[3];
  dislayedArray[3] = dislayedArray[4];
  dislayedArray[4] = tempArrayValue;
  
}

void scrollListUp(byte selection,byte dislayedArray[5], byte* const selectedArray[9],bool isPointer){
  byte tempArrayValue;
    if (isPointer) {
        tempArrayValue = *selectedArray[selection];  // Dereference the pointer to get the value
    } else {
        return;
    }  
  dislayedArray[4] = dislayedArray[3];
  dislayedArray[3] = dislayedArray[2];
  dislayedArray[2] = dislayedArray[1];
  dislayedArray[1] = dislayedArray[0];
  dislayedArray[0] = tempArrayValue;
  
}

void scrollListDown(byte selection,byte dislayedArray[5], byte* const selectedArray[9],bool isPointer){
  byte tempArrayValue;
    if (isPointer) {
        tempArrayValue = *selectedArray[selection];  // Dereference the pointer to get the value
    } else {
        return;
    }  
  dislayedArray[0] = dislayedArray[1];
  dislayedArray[1] = dislayedArray[2];
  dislayedArray[2] = dislayedArray[3];
  dislayedArray[3] = dislayedArray[4];
  dislayedArray[4] = tempArrayValue;
  
}

void scrollMonitorDown(byte selectedDeviceParam) {
byte* tmpByteA;
byte* tmpByteB;
bool* tmpBool;

strncpy(deviceMonitorNames[0],deviceMonitorNames[1],15);
strncpy(deviceMonitorNames[1],deviceMonitorNames[2],15);
strncpy(deviceMonitorNames[2],deviceMonitorNames[3],15);
strncpy(deviceMonitorNames[3],deviceMonitorNames[4],15);
strncpy(deviceMonitorNames[4],deviceParams[selectedDeviceParam].name,15);
deviceMonitorNames[4][14]='\0';

if(deviceParams[selectedDeviceParam].nrpn == false){
  tmpByteA = &deviceParams[selectedDeviceParam].ccNum;
  tmpByteB = &deviceParams[selectedDeviceParam].ccVal;
}

if(deviceParams[selectedDeviceParam].nrpn == true){
  tmpByteA = &deviceParams[selectedDeviceParam].msbVal;
  tmpByteB = &deviceParams[selectedDeviceParam].lsbVal;
}
tmpBool = &deviceParams[selectedDeviceParam].obtained;
  deviceMonitorByteColumnA[0] = deviceMonitorByteColumnA[1];
  deviceMonitorByteColumnA[1] = deviceMonitorByteColumnA[2];
  deviceMonitorByteColumnA[2] = deviceMonitorByteColumnA[3];
  deviceMonitorByteColumnA[3] = deviceMonitorByteColumnA[4];
  deviceMonitorByteColumnA[4] = tmpByteA;

  deviceMonitorByteColumnB[0] = deviceMonitorByteColumnB[1];
  deviceMonitorByteColumnB[1] = deviceMonitorByteColumnB[2];
  deviceMonitorByteColumnB[2] = deviceMonitorByteColumnB[3];
  deviceMonitorByteColumnB[3] = deviceMonitorByteColumnB[4];
  deviceMonitorByteColumnB[4] = tmpByteB;

  deviceMonitorObtained[0] = deviceMonitorObtained[1];
  deviceMonitorObtained[1] = deviceMonitorObtained[2];
  deviceMonitorObtained[2] = deviceMonitorObtained[3];
  deviceMonitorObtained[3] = deviceMonitorObtained[4];
  deviceMonitorObtained[4] = tmpBool;
}








void drawHeader() {

  // Draw the white background for the white header area
  display.fillRect(HEADER_AREA_X , HEADER_AREA_Y , HEADER_AREA_WIDTH , HEADER_AREA_HEIGHT - 1, SH110X_WHITE);
  // Set the text color to black


  // Get the width and height of the text to be displayed
  int16_t x, y;
  uint16_t w, h;
  display.getTextBounds(currentScreenName, 0, 0, &x, &y, &w, &h);
  int textX = HEADER_AREA_X + (HEADER_AREA_WIDTH -  w) / 2;
  int textY = HEADER_AREA_Y + (HEADER_AREA_HEIGHT - h) / 2;

  // print the screen name in the header in black
  display.setCursor(textX, textY);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK);
  display.print(currentScreenName);

}


void displayFileList() {
    switch (currentScreen) {
        case PATCHES:
            displayedListCount = patchFileCount;
            memcpy(displayedListEntries, displayedPatchNames, sizeof(displayedPatchNames));
            break;
        case TRIGGERPATCHES:
          displayedListCount = patchFileCount;
          memcpy(displayedListEntries, displayedPatchNames, sizeof(displayedPatchNames));
            break;
        case SEQUENCES:
            displayedListCount = sequenceFileCount;
            memcpy(displayedListEntries, displayedSequenceNames, sizeof(displayedSequenceNames));
            break;

        case REMAPLIST:
            displayedListCount = reMapFileCount;
            memcpy(displayedListEntries, displayedReMapFileNames, sizeof(displayedReMapFileNames));
            break;

        case TRACKERLIST:
            displayedListCount = trackerFileCount;
            memcpy(displayedListEntries, displayedTrackerFileNames, sizeof(displayedTrackerFileNames));
            break;
        case USERBUTTONCHOOSEPATCH:
            displayedListCount = patchFileCount;
            memcpy(displayedListEntries, displayedPatchNames, sizeof(displayedPatchNames));
            break;
        case MORPHPATCHES:
            displayedListCount = patchFileCount;
            memcpy(displayedListEntries, displayedPatchNames, sizeof(displayedPatchNames));
            break;
        case USERBUTTONCHOOSESESEQUENCE:
            displayedListCount = sequenceFileCount;
            memcpy(displayedListEntries, displayedSequenceNames, sizeof(displayedSequenceNames));
            break;
        case TRIGGERCHOOSESEQUENCE:
            displayedListCount = sequenceFileCount;
            memcpy(displayedListEntries, displayedSequenceNames, sizeof(displayedSequenceNames));
            break;

    }

    
    display.setTextColor(SH110X_WHITE);
    
    if (displayedListCount == 0) {
        display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
        display.println("No Device Files (yet)");
    } else {
        for (byte i = 0; i<5;i++){
          display.setCursor(listGridColumnA[i].x,listGridColumnA[i].y);
          display.print(displayedListEntries[i]);
 }
    }
}








void drawColumn(Grid* gridArray, byte x, byte* valueArray[5]){
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<5;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  byte* selectePointer = valueArray[i]; 
  byte tmpByte = *selectePointer; 
  display.print(tmpByte);
 }   
}

void drawColumn(Grid* gridArray, byte x,const byte* valueArray){
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<5;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  display.print(valueArray[i]);
 }   
}

void drawColumn(Grid* gridArray, byte x,bool* boolArray[5]){
//used for drawing 5 empty boxes 
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<5;i++){
  
  if (*boolArray[i] == true){
    display.setCursor(gridArray[i].x,gridArray[i].y);
    display.print("ON");  
  }else{
    display.setCursor(gridArray[i].x,gridArray[i].y);
    display.print("OFF");      
  }
 }   
}


void drawColumn(Grid* gridArray, byte x,byte value){
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<5;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  display.print(value);
 }   
}

void drawDeviceMonitor(){
  if(deviceParams[selectedDeviceParam].nrpn == false){
    strcpy(currentScreenName, "NAME       CC VAL CH");
  }
  if(deviceParams[selectedDeviceParam].nrpn == true){
    strcpy(currentScreenName, "NAME      MSB LSB CH");  
  }
      drawHeader();
      display.setTextColor(SH110X_WHITE);
      drawColumn(listGridColumnA,0,deviceMonitorNames,5);
      drawColumn(listGridColumnB,70,deviceMonitorByteColumnA);
      drawColumn(listGridColumnC,90,deviceMonitorByteColumnB);
      drawColumn(listGridColumnD,113,displayedIncomingMidiChannel); 
      defineListGridColumn(listGridColumnE, 120,10);
      display.setTextColor(SH110X_WHITE);
      for(byte i = 0; i < 5; i++){
        if(*deviceMonitorObtained[i] == true){
        display.setCursor(listGridColumnD[i].x-5,listGridColumnD[i].y);
        display.write(0x07);
        }
      }
}
void drawColumn(Grid* gridArray, byte x,char charArray[5]){
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<5;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  display.print(charArray[i]);
 }   
}


void drawColumn(Grid* gridArray, byte x,char charArray[5], byte linesToDraw){
      display.setTextColor(SH110X_WHITE);
defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<linesToDraw;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  display.print(charArray[i]);
 }   
}

void drawColumn(Grid* gridArray, byte x, byte* byteArray[5], byte linesToDraw){
  display.setTextColor(SH110X_WHITE);
  defineListGridColumn(gridArray, x, 10); // Example x coordinate
 for (byte i = 0; i<linesToDraw;i++){
  display.setCursor(gridArray[i].x,gridArray[i].y);
  display.print( *byteArray[i]);
 }  
}


void fileNameEntry(){
  display.setCursor(0,HEADER_AREA_HEIGHT + 2);
  display.setTextColor(SH110X_WHITE);
  display.println("ENTER NEW FILE NAME");

  for (byte i = 0; i < MAX_FILE_NAME_LENGTH; i++){
  display.setCursor(newFileNameGrid[i].x,newFileNameGrid[i].y);
  display.print(newFileNameArray[i]);
  display.setCursor(newFileNameGrid[i].x,newFileNameGrid[i].y+3);
  display.print("_");
}
display.setCursor(0,42);
display.println("YES = CONFIRM NAME");  
display.setCursor(0,52);
display.println("NO = CANCEL");  
}

void updateDisplayNamesBasedOnScreen(char displayedNames[][21]) {
    switch(currentScreen) {
        case PATCHES:
            memcpy(displayedPatchNames, displayedNames, sizeof(displayedPatchNames));
            break;
        case SEQUENCES:
            memcpy(displayedSequenceNames, displayedNames, sizeof(displayedSequenceNames));
            break;
        case REMAPLIST:
            memcpy(displayedReMapFileNames, displayedNames, sizeof(displayedReMapFileNames));
            break;
        case TRACKERLIST:
            memcpy(displayedTrackerFileNames, displayedNames, sizeof(displayedTrackerFileNames));
            break;
    }
}





void deleteConfirm() {
  display.setCursor(0, HEADER_AREA_HEIGHT + 2);
  display.setTextColor(SH110X_WHITE);
  display.print("Delete: ");
  display.print(selectedDisplayedNames[selection]);
  display.println(" ?");
  display.println(" ");
  display.println("YES or NO");
}




void drawWaveform() {
  int lastY; // Declare lastY here, outside of the switch statement
    // Calculate the center line of the wave box
    int centerY = WAVEBOX_Y + WAVEBOXHEIGHT / 2;

    // Calculate the maximum amplitude (half of the wave box height minus 2 pixels for the top and bottom margins)
    int maxAmplitude = (WAVEBOXHEIGHT - 2) / 2;

    // Minimum amplitude corresponds to half of the 5 pixels (total height) so that the full height is 5 pixels
    int minAmplitude = 5 / 2;

    // Map the depth to the amplitude range (from minAmplitude to maxAmplitude)
    int amplitude = map(lfoDepth, 0, 100, minAmplitude, maxAmplitude);

    // Map the frequency to control the number of waveforms
    int frequency = map(lfoFreq, 0, 100, 1, 10);
  switch (lfoWaveForm) {
case TRIANGLE:


    lastY = centerY;
    for (int x = 0; x < WAVEBOXWIDTH; x++) {
        // Create a linear ramp (0 to 1) that repeats based on the frequency
        float ramp = (float)((x * frequency) % WAVEBOXWIDTH) / WAVEBOXWIDTH;

        // Triangle wave: Convert the ramp to a triangle waveform oscillating between -1 and 1
        float triangle = 1.0 - 4.0 * abs(ramp - 0.5);

        // Scale and center the triangle wave within the wave box
        int y = centerY - int(triangle * amplitude);
        
        display.drawLine(x - 1 + WAVEBOX_X, lastY, x + WAVEBOX_X, y, SH110X_WHITE);
        lastY = y;
    }
    break;
case SINE:
    // Initialize lastY for the sine wave
    lastY = centerY - int(sin(0) * amplitude);

    for (int x = 0; x < WAVEBOXWIDTH; x++) {
        float angle = 2.0 * PI * frequency * x / WAVEBOXWIDTH;
        int y = centerY - int(sin(angle) * amplitude);
        
        display.drawLine(x - 1 + WAVEBOX_X, lastY, x + WAVEBOX_X, y, SH110X_WHITE);
        lastY = y;
    }
    break;


case SQUARE:
    // Initialize lastY for the square wave
    lastY = centerY;

    for (int x = 0; x < WAVEBOXWIDTH; x++) {
        // Determine the high or low state based on the frequency
        // Multiply the frequency by 2 to double the number of cycles
        bool isHigh = ((x * frequency * 2 / WAVEBOXWIDTH) % 2) == 0;
        
        // Calculate y based on the amplitude and whether the state is high or low
        int y = isHigh ? (centerY - amplitude) : (centerY + amplitude);

        display.drawLine(x - 1 + WAVEBOX_X, lastY, x + WAVEBOX_X, y, SH110X_WHITE);
        lastY = y;
    }
    break;


  }
}

void drawLfoTaskBar() {

  //plop lables and values on screen based on lfoGridPoints. 
  display.drawLine(LFOTASKBAR_X, LFOTASKBAR_Y-1, SCREENWIDTH, LFOTASKBAR_Y-1, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  //print the lables for parameters
  for (byte i = 5; i < 9; i++){
  display.setCursor(lfoGridPoint[i].x,lfoGridPoint[i].y);
  display.print(lfoTaskBar[i-5]);
  }
  //print the current values of the parameters
  display.setCursor(lfoGridPoint[1].x, lfoGridPoint[1].y);
  display.print(lfoAppStatus ? "ON" : "OFF");
  display.setCursor(lfoGridPoint[2].x, lfoGridPoint[2].y);
  display.print(selectedLfoCcNum);
  display.setCursor(lfoGridPoint[3].x, lfoGridPoint[3].y);
  display.print(lfoFreq);
  display.setCursor(lfoGridPoint[4].x, lfoGridPoint[4].y);
  display.print(lfoDepth);
 }
 
 void drawSeqNotes(){
  display.setTextColor(SH110X_WHITE);
  for (byte i = 0; i < seqLength; i++) {

    
    if (sequencerArray[i].numNotes == 2){
      display.setCursor(seqGrid[i].x + 4, seqGrid[i].y + 4);
      display.write(0x13);
    }else if(sequencerArray[i].seqStepNotes[0] == 255){
      display.setCursor(seqGrid[i].x + 2, seqGrid[i].y + 4);
        display.print("--");
     }else if(sequencerArray[i].seqStepNotes[0] != 255){
      display.setCursor(seqGrid[i].x + 2, seqGrid[i].y + 4);
        display.print(byteToNote(sequencerArray[i].seqStepNotes[0]));
     }
    }
}


void drawSeqSubMenu(){
  
  //draw the box
  display.fillRect(seqSubMenuGrid[11].x,seqSubMenuGrid[11].y,SUBMENUWIDTH,SUBMENUHEIGHT,SH110X_BLACK);
  display.drawRect(seqSubMenuGrid[11].x,seqSubMenuGrid[11].y,SUBMENUWIDTH,SUBMENUHEIGHT,SH110X_WHITE);
  display.fillRect(seqSubMenuGrid[11].x,seqSubMenuGrid[11].y,SUBMENUWIDTH,SUBMENUHEADERHEIGHT,SH110X_WHITE);
  display.drawLine(seqSubMenuGrid[12].x,seqSubMenuGrid[12].y, seqSubMenuGrid[12].x, SUBMENUHEIGHT+1, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_BLACK);
  //print words for the header
for (byte i = 0; i < 2;i++){
display.setCursor(seqSubMenuGrid[i].x,seqSubMenuGrid[i].y);
display.print(seqSubMenuLables[i]);
}
display.setTextColor(SH110X_WHITE);
for (byte i = 2; i < 8; i++){
display.setCursor(seqSubMenuGrid[i].x,seqSubMenuGrid[i].y);
display.print(seqSubMenuLables[i]);  
}

display.setCursor(seqSubMenuGrid[8].x,seqSubMenuGrid[8].y);
  display.print(seqRingout ? "Y" : "N");
display.setCursor(seqSubMenuGrid[9].x,seqSubMenuGrid[9].y);
  display.print(seqTie ? "Y" : "N");
display.setCursor(seqSubMenuGrid[10].x,seqSubMenuGrid[10].y);
  display.print(seqVel);


display.setCursor(seqSubMenuGrid[13+ seqRecordMode].x ,seqSubMenuGrid[13+ seqRecordMode].y);
display.write(0x07);

drawBigCell(seqSubMenuGrid[16].x, seqSubMenuGrid[16].y, "SAVE/LOAD", false);



}

void drawSeqLength() {
  if (seqLength <= 8) {
    display.drawRect(0, 0, 16*seqLength, SEQ_BOX_HEIGHT, SH110X_WHITE);
    return;
  }
  if (seqLength == 16) {
    display.drawRect(0, 0, SCREENWIDTH, SEQ_BOX_HEIGHT*2, SH110X_WHITE);
    return;
  }
  if (seqLength == 24) {
    display.drawRect(0, 0, SCREENWIDTH, SEQ_BOX_HEIGHT*3, SH110X_WHITE);
    return;
  }
  if (seqLength == 32) {
    display.drawRect(0, 0, SCREENWIDTH, SCREENHEIGHT, SH110X_WHITE);
    return;
  }
  byte boxHeight = (seqLength / 8 + 1) * SEQ_BOX_HEIGHT;
  byte bottomLineY = boxHeight - SEQ_BOX_HEIGHT;
  byte rightLineX = seqGrid[seqLength].x - 2;

  display.drawLine(0, 0, SCREENWIDTH, 0, SH110X_WHITE); // Top horizontal line
  display.drawLine(0, 0, 0, boxHeight, SH110X_WHITE); // Left vertical line

  if (seqLength > 8) {
    display.drawLine(0, boxHeight, rightLineX, boxHeight, SH110X_WHITE); // Bottom left horizontal line
    display.drawLine(rightLineX, boxHeight, rightLineX, bottomLineY, SH110X_WHITE); // Middle vertical line
    display.drawLine(rightLineX, bottomLineY, SCREENWIDTH, bottomLineY, SH110X_WHITE); // Bottom right horizontal line
    display.drawLine(SCREENWIDTH - 1, 0, SCREENWIDTH - 1, bottomLineY, SH110X_WHITE); // Right vertical line
  }
}

void scrollDisplayedFileListDown(const std::vector<CharArray>& fileList, char displayedNames[][21], int selectedFile) {
    // Shift the displayed names up to make room for the new bottom entry
    for (int i = 0; i < 4; ++i) {
        strcpy(displayedNames[i], displayedNames[i + 1]);
    }

    // Add the new entry at the bottom based on the selectedFile index
    if (selectedFile >= 0 && selectedFile < fileList.size()) {
        strncpy(displayedNames[4], fileList[selectedFile].data(), 20);
        displayedNames[4][20] = '\0';  // Ensure null termination

        // If the names have extensions like '.txt', this will remove it
        char* dotPosition = strchr(displayedNames[4], '.');
        if (dotPosition != nullptr) {
            *dotPosition = '\0';  // Truncate the string at the position of the dot
        }
    } else {
        // If selectedFile is out of range, provide a fallback message
        strcpy(displayedNames[4], "Invalid selection");
    }

    // Optionally, update the displayed names based on the current screen
    updateDisplayNamesBasedOnScreen(displayedNames);
}

void scrollDisplayedFileListUp(const std::vector<CharArray>& fileList, char displayedNames[][21], int selectedFile) {
    // Shift the displayed names down to make room for the new top entry
    for (int i = 4; i > 0; --i) {
        strcpy(displayedNames[i], displayedNames[i - 1]);
    }

    // Add the new entry at the top based on the selectedFile index
    if (selectedFile >= 0 && selectedFile < fileList.size()) {
        strncpy(displayedNames[0], fileList[selectedFile].data(), 20);
        displayedNames[0][20] = '\0';  // Ensure null termination

        // If the names have extensions like '.txt', this will remove it
        char* dotPosition = strchr(displayedNames[0], '.');
        if (dotPosition != nullptr) {
            *dotPosition = '\0';  // Truncate the string at the position of the dot
        }
    } else {
        // If selectedFile is out of range, provide a fallback message
        strcpy(displayedNames[0], "Invalid selection");
    }

    // Optionally, update the displayed names based on the current screen
    updateDisplayNamesBasedOnScreen(displayedNames);
}
