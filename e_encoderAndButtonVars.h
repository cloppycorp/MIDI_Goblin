#ifndef E_ENCODERANDBUTTONVARS_H
#define E_ENCODERANDBUTTONVARS_H
#include <Arduino.h>
#include "b_definitions.h"

extern unsigned long lastBtnPressTime;
extern unsigned long btndebounce;
//encoder variables
extern bool encStates[4];
extern bool previousEncStatesA[4];
extern bool currentEncStates[2];
// Define the patterns
extern const bool pattern1[4];
extern const bool prevPattern1[4] ;

extern const bool pattern2[4];
extern const bool prevPattern2[4];
extern byte encValue; // a generic counter
extern byte previousEncValue; //used to detect changes to encoder
extern byte encIncrementA; //change encVal when pins are true,true
extern byte encIncrementB; //change encVal when pins are false,false
extern byte encMaximum;
extern byte encMinimum;

//button variables
extern byte buttonPressed; // is set after mux is read
extern byte previousBtnPressed; //is set after buttons are reset or released
extern bool buttonHeld; //flag for when button is held down after being pressed
extern bool noButtonPressed;
extern char pressedButtonName[10]; //name of button that is displayed - gets updated with btnNames array when buttons are pressed
extern bool previousBtnStates[16]; //array for storing button states
extern bool currentBtnStates[16]; //array for storing button states
extern int frameCounter;//counts the number of frames drawn - helps identify when unnessesary frames are drawn.
extern bool navBtnPressed;
// an array of button names - corresponds with "i" in checkButtons()
extern char btnNames[BTN_NAMES][MAX_BTN_NAME_LENGTH];

//enumeration of the buttonPressed variable
enum Button {
  leftBtnPressed,
  upBtnPressed,
  downBtnPressed,
  rightBtnPressed,
  savenewBtnPressed,
  deleteBtnPressed,
  backBtnPressed,
  overwriteBtnPressed,
  sendBtnPressed,
  yesBtnPressed,
  menuBtnPressed,
  noBtnPressed,
  user1BtnPressed,
  user2BtnPressed,
  user3BtnPressed,
  encoderBtnPressed,
  noBtnsPressed = 255
};
void updateEncoderStates();

void checkEncPattern();



void readMUX();
void checkBtnStates();

void handleBtns();

#endif // E_ENCODERANDBUTTONVARS_H
