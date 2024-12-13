#include "x_midiClock.h"

bool midiClockStarted = false;
unsigned long midiClock0 = 0;
unsigned long midiClock24 = 0;
unsigned long bpm = 0;            // Stores the averaged BPM
unsigned long previousBpm = 0;
unsigned int clockCount = 0;      // Counter to track which clock message we're on (0-23)
unsigned long midiClockBeatLength = 0;
unsigned long bpmValues[4] = {0}; // Array to hold previous BPM values
int bpmIndex = 0;   
unsigned long arpBpm = 0;
unsigned long arpNoteBpm = 0;
unsigned long seqBpm = 0;
byte clockSource = 0; 
byte clockSourceMin = 0;
byte clockSourceMax = 1;

unsigned long bpmMin = 10;
unsigned long bpmMax = 330;
char clockSourceNames[2][7] = {
  "GOBLIN",
  "MIDI"
};

void drawSystemInfoScreen(){
  defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
  display.setTextColor(SH110X_WHITE);
  display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
  display.print("System Version:");
  display.println(systemVersion);
}
void drawClockSourceScreen(){
    defineListGridColumn(listGridColumnA, 1, 10); // Example x coordinate
    defineListGridColumn(listGridColumnB, 90, 10); // Example x coordinate
    display.setTextColor(SH110X_WHITE);
    display.setCursor(listGridColumnA[0].x,listGridColumnA[0].y);
    display.print("Source:");
    display.setCursor(listGridColumnB[0].x,listGridColumnB[0].y);
    display.print(clockSourceNames[clockSource]);

    switch(clockSource){
      case 0://goblin
      display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
      display.print("ARP  (MS): ");
      display.setCursor(listGridColumnB[1].x,listGridColumnB[1].y);
      arpBpm = 60000/arpeggiatorTempo;
      display.print(arpBpm);

      display.setCursor(listGridColumnA[2].x,listGridColumnA[2].y);
      display.print("ARP-NOTE (MS): ");
      display.setCursor(listGridColumnB[2].x,listGridColumnB[2].y);
      arpNoteBpm = 60000/arpNoteTempo;
      display.print(arpNoteBpm);

      display.setCursor(listGridColumnA[3].x,listGridColumnA[3].y);
      display.print("SEQUENCER (MS): ");
      display.setCursor(listGridColumnB[3].x,listGridColumnB[3].y);
      seqBpm = 60000/sequencerTempo;
      display.print(seqBpm);
      break;

      case 1://midiclock
      display.setCursor(listGridColumnA[1].x,listGridColumnA[1].y);
      display.print("BMP: ");
      display.setCursor(listGridColumnB[1].x,listGridColumnB[1].y);
      display.print(bpm);
      break;
    }

}

unsigned long bpmToMilliseconds(unsigned long bpm) {
  return 60000 / bpm;
}

void calculateMidiClockBpm(unsigned long midiClock0, unsigned long midiClock24) {
  
  if(bpmIndex <= 3){
  bpmValues[bpmIndex] = midiClock24 - midiClock0;
  bpmIndex++;
  }
  if(bpmIndex > 3){
    bpmValues[0] = bpmValues[1];
    bpmValues[1] = bpmValues[2];
    bpmValues[2] = bpmValues[3];
    bpmValues[3] = midiClock24 - midiClock0;


    float averageBeatLength = (bpmValues[0] + bpmValues[1] + bpmValues[2] + bpmValues[3])/4;
    midiClockBeatLength = averageBeatLength;
    bpm = 60000/midiClockBeatLength;
    if(bpm != previousBpm && currentScreen == CLOCKSOURCE){
      DS = false;
    }
    if (!midiClockStarted){
      midiClockStarted = true;
    }
  }




}

void handleMidiClock(unsigned long timeReceived) {
  switch(clockCount){
    case 0:
      midiClock0 = timeReceived;
    break;
    case 24:
      midiClock24 = timeReceived;
      calculateMidiClockBpm(midiClock0, midiClock24);
    break;
  }

  clockCount++;
  if (clockCount > 24){
    clockCount = 0;
  }

}