#include "v_addcontrollerparam.h"




void drawAddControllerParam(){
  
      drawTable(addCcParamGrid[0].x,addCcParamGrid[0].y,1);
      strcpy(tableColumnA_0, "CC NUMBER");
      sprintf(tableColumnB_0, "%d", tempCcNum);
      drawTableContents(
    tableColumnA_0,tableColumnB_0,
    tableColumnA_1,tableColumnB_1,
    tableColumnA_2,tableColumnB_2,
    tableColumnA_3,tableColumnB_3,
    tableColumnA_4,tableColumnB_4,        
     1);
     
  display.setCursor(2,25);
  display.print("NAME:");
 //draw some lines under where the text will go
 for (byte i = 1; i < 15; i++){
  display.setCursor(addCcParamGrid[i].x,addCcParamGrid[i].y+2);
  display.print('_');
 }

 //draw tempCcName (which is initialized as blank spaces
 
 for (byte i = 0; i <14;i++){
  if(tempCcNameAlphaIndex[i]!=255){
  tempCcName[i] = alphabet[tempCcNameAlphaIndex[i]];
  display.setCursor(addCcParamGrid[i+1].x,addCcParamGrid[i+1].y);
  display.print(tempCcName[i]);
 }
 }
 
    


  
  drawBigCell(addCcParamGrid[15].x, addCcParamGrid[15].y, "CANCEL", false);  
  drawBigCell(addCcParamGrid[16].x, addCcParamGrid[16].y, "CONFIRM", false);
}
