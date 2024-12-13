#include "m_lfo.h"
bool lfoAppStatusCheck = false;
bool lfoStarted = false;

unsigned long calculateIncrementTime() {
    // Adjust the formula based on the waveform
    switch (lfoWaveForm) {
        case SQUARE:
            // Longer interval for square wave as it changes less frequently
            return 2000 / max(lfoFreq, (byte)1); // Example, adjust as needed

        case SINE:
        case TRIANGLE:
            // More frequent updates for continuous waveforms
            return 1000 / max(lfoFreq, (byte)1);

        // Other waveforms...
    }
}
void calculateLfoValueDepth(int &minValue, int &maxValue) {
    if (lfoDepth == 0) {
        // If depth is 0, set a narrow range around the midpoint
        minValue = 60;
        maxValue = 68;
    } else {
        // Calculate the range based on lfoDepth
        int range = map(lfoDepth, 0, 100, 0, 127);
        minValue = 64 - range / 2; // Adjust to center around the midpoint (64)
        maxValue = 64 + range / 2;
    }

    // Ensure values are within MIDI range
    minValue = constrain(minValue, 0, 127);
    maxValue = constrain(maxValue, 0, 127);
}

void startLfo(){

if (lfoAppStatus && !lfoStarted){
   //yoink the selected device cc num, value and index and hold them in memory (this way
  //when you change the selected cc num, you can plug the old values back in that were being oscillated 
  lfoEndTime = millis();
  tempDeviceCcNum = selectedLfoCcNum;
  tempDeviceLfoIndex = deviceLfoIndex;
  if (patchLoaded){
tempDeviceCcVal = deviceCcValues[deviceLfoIndex];
lfoOscVal = deviceCcValues[deviceLfoIndex];
  }
  if (!patchLoaded){
tempDeviceCcVal = panelCcValues[deviceLfoIndex];    
lfoOscVal = deviceCcValues[deviceLfoIndex];
  }
  
  
  lfoAppStatusCheck = false;
  lfoStarted = true;
}
  
}

void playLfo() {
  if (lfoStarted){
    unsigned long currentTime = millis();
    
    if (lfoAppStatus && currentTime >= lfoEndTime) {
        int minValue, maxValue;
        calculateLfoValueDepth(minValue, maxValue);

        if (lfoWaveForm == SQUARE) {
            // For square wave, only send MIDI message on state change
            if (lfoUpDown != previousLfoUpDown) {
                lfoOscVal = lfoUpDown ? maxValue : minValue;
                MIDI.sendControlChange(tempDeviceCcNum , lfoOscVal, lfoOutChannel);
                panelCcValues[tempDeviceLfoIndex] = lfoOscVal;
                previousLfoUpDown = lfoUpDown;
            }

            // Toggle lfoUpDown state at the end of each interval
            if (currentTime >= lfoEndTime) {
                lfoUpDown = !lfoUpDown;
            }
        } else {
            // Logic for other waveforms (sine, triangle, etc.)
            if (lfoUpDown) {
                lfoOscVal++;
                if (lfoOscVal >= maxValue) {
                    lfoOscVal = maxValue;
                    lfoUpDown = false; // Start decrementing
                }
            } else {
                lfoOscVal--;
                if (lfoOscVal <= minValue) {
                    lfoOscVal = minValue;
                    lfoUpDown = true; // Start incrementing
                }
            }

            lfoOscVal = constrain(lfoOscVal, minValue, maxValue); // Ensure value stays within range
            MIDI.sendControlChange(tempDeviceCcNum , lfoOscVal, lfoOutChannel);
            panelCcValues[tempDeviceLfoIndex] = lfoOscVal;
        }

        // Calculate the next end time
        lfoIncrementTime = calculateIncrementTime();
        lfoEndTime = currentTime + lfoIncrementTime;
    }
  }
}

void stopLfo(){
  if (!lfoAppStatus){
    MIDI.sendControlChange(tempDeviceCcNum,tempDeviceCcVal,lfoOutChannel);
    panelCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
    deviceCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
//    if(patchLoaded){
//      deviceCcValues[tempDeviceLfoIndex] = tempDeviceCcVal;
//    }
     
     lfoAppStatusCheck = true;
     lfoStarted = false;
  }
}
