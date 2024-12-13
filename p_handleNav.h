#ifndef P_HANDLENAV_H
#define P_HANDLENAV_H
#include "b_definitions.h"
#include "c_appVariables.h"
#include "e_encoderAndButtonVars.h"
#include "d_selectionVariables.h"
#include "c_appVariables.h"
#include "h_deviceVars.h"
#include "i_initializers.h"
#include "j_displayFunctions.h"
#include "r_fileFunctions.h"
#include "w_morph.h"
#include "n_arpeggiator.h"
extern bool noteEdit;

void finishNoteEdit();
void triggerFunctionsNavCommands();
void handleXYNavigation(byte &x, byte &y, byte xMax, byte yMax);
template <size_t N,size_t M>
void genericListNav(int* selectedListIndex,char (&displayedArray)[5][N], char (*selectedArray)[M] ){
  //encNav(encState);
  switch(buttonPressed){
    case upBtnPressed:{
      if (displayIndex > displayIndexMin){
        displayIndex--;
        (*selectedListIndex)--;
        DS = false;
        return;
      }
      if(displayIndex == displayIndexMin && *selectedListIndex > 0){
        (*selectedListIndex)--;
        scrollListUp(selectedListIndex, displayedArray, selectedArray);
        DS = false;
        return;
      }
    }
    break;
    case downBtnPressed:{
      if (displayIndex < displayIndexMax && displayIndex < selectedListIndexMax){
        displayIndex++;
        (*selectedListIndex)++;
        DS = false;
        return;
      }
      if(displayIndex == displayIndexMax && *selectedListIndex < selectedListIndexMax){
        (*selectedListIndex)++;
        scrollListDown(selectedListIndex, displayedArray, selectedArray);
        DS = false;
        return;
      }      
    }
    break;

  }
}



template <size_t N,size_t M,size_t O>
//used for scrolling lists that have two entries 
void genericListsNav(int* selectedListIndex,char (&displayedArray)[5][N], char (*selectedArray)[M],char (&displayedArray2)[5][N], char (*selectedArray2)[O] ){
  //encNav(encState);
  switch(buttonPressed){
    case upBtnPressed:{
      if (displayIndex > displayIndexMin){
        displayIndex--;
        (*selectedListIndex)--;
        DS = false;
        return;
      }
      if(displayIndex == displayIndexMin && *selectedListIndex > 0){
        (*selectedListIndex)--;
        scrollListUp(selectedListIndex, displayedArray, selectedArray);
        scrollListUp(selectedListIndex, displayedArray2, selectedArray2);
        DS = false;
        return;
      }
    }
    break;
    case downBtnPressed:{
      if (displayIndex < displayIndexMax && displayIndex < selectedListIndexMax){
        displayIndex++;
        (*selectedListIndex)++;
        DS = false;
        return;
      }
      if(displayIndex == displayIndexMax && *selectedListIndex < selectedListIndexMax){
        (*selectedListIndex)++;
        scrollListDown(selectedListIndex, displayedArray, selectedArray);
        scrollListDown(selectedListIndex, displayedArray2, selectedArray2);
        DS = false;
        return;
      }      
    }
    break;

  }
}
void handleNavForPatchSelection();
void handleNavForTriggerFunctions();
void handleNavForTriggers();
void handleNavForFileListLimited();
void loadAppValues();
void screenSwitch();
void gotoMainMenu();
void chooseDevice();
void encNav(byte encState);
void userBtnSelectNavCommands(int* selectedListIndex);
void userButtonFunctionsNavCommands();
void userBtnChooseAppNavCommands();
void handleNavigationForOverWrite();
void handleNavigationForLfo();
void handleNavigationForMonitor();
void handleNavigationForMidiOutChannel();
void handleNavigationForApp();
void handleNavigationForSaveNewFile();
void handleNavigationForDelete();
void handleNavForController();
void menuNavCommands(int* selectedListIndex);
void handleNavigationForFileList();
void deviceSelectNavCommands();
void handleNavigationForRemap();
void handleNavForAddControllerParam();
void handleNavigationForSequencer();
void triggerFunctionChooseAppNavCommands();
void handleNavigationForMorph();
void handleNavForSeqSwang();
void extraArpeggiatorNavOptions();
void handleNavigation();
void handleNavForClockSource();
void handleNavForSystemInfo();
#endif //P_HANDLENAV_H
