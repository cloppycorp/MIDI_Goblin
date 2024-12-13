#include "w_morph.h"
void handleMorph(int morphPosition){
if (morphPosition != previousMorphPosition){
  calcMorph(numEntries, morphPosition);

  for (byte i = 0; i < numEntries; i++ ){
    if(deviceParams[i].morphCalc != 255){
      MIDI.sendControlChange(deviceParams[i].ccNum, deviceParams[i].morphCalc, morphOutChannel);
    }
  }

  previousMorphPosition = morphPosition;
  morphed = true;
}
}




void calcMorph(byte numEntries, int morphPosition) {
  byte morphPos = (byte)morphPosition;

    for (byte i = 0; i < numEntries; i++) {
        
        // Perform linear interpolation
        if(deviceParams[i].nrpn == false && deviceParams[i].morphFinal != 255 && deviceParams[i].obtained == true){
          deviceParams[i].morphCalc = deviceParams[i].ccVal + ((deviceParams[i].morphFinal - deviceParams[i].ccVal) * morphPos/100);
        }
    
  }
}

void resetMorph(){
  for(byte i = 0; i < numEntries; i++){
    if(deviceParams[i].nrpn == false && deviceParams[i].ccVal != deviceParams[i].morphCalc)
    delayMicroseconds(10);
    MIDI.sendControlChange(deviceParams[i].ccNum,deviceParams[i].ccVal,morphOutChannel);
  }
}



