#include "u_controller.h"

void drawControllerOptions(){
display.setTextColor(SH110X_WHITE);
display.setCursor(listGridColumnA[0].x, listGridColumnA[0].y);
display.print("USE DEVICE: ");
display.setCursor(listGridColumnB[0].x, listGridColumnB[0].y);
display.print(ctrlrUseDeviceParams ? "YES" : "NO");
display.setCursor(listGridColumnA[1].x, listGridColumnA[1].y);
display.print("MIDI CHANNEL: ");
display.setCursor(listGridColumnB[1].x, listGridColumnB[1].y);
display.print(controllerOutChannel);
}

void drawControllerList(){
display.setTextColor(SH110X_WHITE);


if (controllerParamsCount > 0){

for(byte i = 0; i < 5; i++){
  if(ctrlrPtrs[i].index != nullptr && *ctrlrPtrs[i].index != 255){
    display.setCursor(listGridColumnA[i].x, listGridColumnA[i].y);
    display.print(ctrlrPtrs[i].ccName);
    display.setCursor(listGridColumnB[i].x, listGridColumnB[i].y);
    display.print(*ctrlrPtrs[i].ccNum);
    display.setCursor(listGridColumnC[i].x, listGridColumnC[i].y);
    display.print(*ctrlrPtrs[i].ccVal);
    if(ctrlrUseDeviceParams && *ctrlrPtrs[i].obtainedVal == true ){
      display.setCursor(listGridColumnD[i].x-28, listGridColumnD[i].y);
      display.write(0x07);
    }
    
  }
  if(ctrlrPtrs[i].index != nullptr && *ctrlrPtrs[i].index == 255){
    drawBigCell(listGridColumnA[i].x, listGridColumnA[i].y, "+", false);
  }
}

}
}





  
