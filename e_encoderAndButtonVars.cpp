#include "e_encoderAndButtonVars.h"
unsigned long lastBtnPressTime = 0;
unsigned long btndebounce = 100;
//encoder variables
bool encStates[4] = {false, false, false, false};
bool previousEncStatesA[4] = {false, false, false, false};
bool currentEncStates[2] = {false, false};
// Define the patterns
const bool pattern1[4] = {0, 1, 0, 0};
const bool prevPattern1[4] = {0, 0, 1, 0};

const bool pattern2[4] = {0, 0, 0, 1};
const bool prevPattern2[4] = {0, 1, 1, 1};
byte encValue = 0; // a generic counter
byte previousEncValue = 0; //used to detect changes to encoder
byte encIncrementA = 1; //change encVal when pins are true,true
byte encIncrementB = 0; //change encVal when pins are false,false
byte encMaximum = 255;
byte encMinimum = 0;

//button variables
byte buttonPressed = 255; // is set after mux is read
byte previousBtnPressed = 255; //is set after buttons are reset or released
bool buttonHeld = false; //flag for when button is held down after being pressed
bool noButtonPressed = true;
char pressedButtonName[10] = {"---------"}; //name of button that is displayed - gets updated with btnNames array when buttons are pressed
bool previousBtnStates[16] {false}; //array for storing button states
bool currentBtnStates[16] {false}; //array for storing button states
int frameCounter = 0;//counts the number of frames drawn - helps identify when unnessesary frames are drawn.

bool navBtnPressed = false;
// an array of button names - corresponds with "i" in checkButtons()
char btnNames[BTN_NAMES][MAX_BTN_NAME_LENGTH] = {
  "left", //0
  "up", //1
  "down", //2
  "right", //3
  "save new", //4
  "delete", //5
  "back", //6
  "overwrite", //7
  "send", //8
  "yes", //9
  "menu", //10
  "no", //11
  "user 1", //12
  "user 2", //13
  "user 3", //14
  "encoder", //15
};

void updateEncoderStates() {
  // Use memcpy to copy encStates to previousEncStatesA
  memcpy(previousEncStatesA, encStates, sizeof(encStates));

  encStates[2] = encStates[0];
  encStates[3] = encStates[1];
  encStates[0] = currentEncStates[0];
  encStates[1] = currentEncStates[1];
}

void checkEncPattern() {
  if (memcmp(encStates, pattern1, sizeof(pattern1)) == 0 && memcmp(previousEncStatesA, prevPattern1, sizeof(prevPattern1)) == 0) {
    if (encValue + encIncrementA > 255) {
      encValue = 0;
      encState = increased;
    } else if ( encValue + encIncrementA <= 255) {
      encValue += encIncrementA;
      encState = increased;
    }
    return;
  }

  if (memcmp(encStates, pattern2, sizeof(pattern2)) == 0 && memcmp(previousEncStatesA, prevPattern2, sizeof(prevPattern2)) == 0) {
    if (encValue - encIncrementA < 0) {
      encValue = 255;
      encState = decreased;
    } else if (encValue - encIncrementA >= 0) {
      encValue -= encIncrementA;
      encState = decreased;
    }
    return;
  }
}

void readMUX() {
  //read all the pins of the MUX and set the states of the values in the buttonStates array
  for (int i = 0; i < 16; i++) {
    // Set the multiplexer select lines
    digitalWrite(MUX_S0, bitRead(i, 0));
    digitalWrite(MUX_S1, bitRead(i, 1));
    digitalWrite(MUX_S2, bitRead(i, 2));
    digitalWrite(MUX_S3, bitRead(i, 3));
    // Add a small delay to allow the multiplexer to stabilize
    delayMicroseconds(5);
    // Read the multiplexer signal pin
    int buttonState = digitalRead(MUX_SIG);
    // Check if the button is pressed (assuming active-HIGH)
    //if a button press is detected, flag it in buttonStates array and exit the loop.
    //if no button presses are detected - entire buttonStates array will be false.
    if (buttonState == HIGH) {
      currentBtnStates[i] = true;
    }

    if (buttonState == LOW) {
      currentBtnStates[i] = false;
    }
  }

}

void checkBtnStates() {
buttonPressed = 255;
for (byte i = 0; i<16;i++){
  if(currentBtnStates[i] == true && previousBtnStates[i] == false){
    lastBtnPressTime = millis();
    buttonPressed = i;
    return;
  }
}
}

void handleBtns() {
  readMUX(); //read all the mux pins
  checkBtnStates(); //set the buttonStates array values and determine but is being held

}
