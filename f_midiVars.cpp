#include "f_midiVars.h"
//midi variables
byte midiOutChannel = 2; // define midi out channel
byte noteReceived = 255; // the midi note received
byte octaveShift = 2; // determines output note played when one is received
const char* harmonyNoteName; // used to display harmony note letter
const char* harmonyOctave; // used to display harmony note octave
const char* noteName; // used to display received note letter
const char* octave; // used to display received note octave
//monitor
bool justReceivedNrpn = false;
bool justReceivedCc = true;
bool justReceivedNote = false;
byte displayedIncomingMidiChannel = 0;

const char* byteToNote(byte midiNote) {
    // Define the note names and octaves
    const char* noteTable[128][2] = {
        {"C", "-"}, {"c", "-"}, {"D", "-"}, {"d", "-"}, {"E", "-"}, {"F", "-"}, {"f", "-"}, {"G", "-"}, {"g", "-"}, {"A", "-"}, {"a", "-"}, {"B", "-"},
        {"C", "0"}, {"c", "0"}, {"D", "0"}, {"d", "0"}, {"E", "0"}, {"F", "0"}, {"f", "0"}, {"G", "0"}, {"g", "0"}, {"A", "0"}, {"a", "0"}, {"B", "0"},
        {"C", "1"}, {"c", "1"}, {"D", "1"}, {"d", "1"}, {"E", "1"}, {"F", "1"}, {"f", "1"}, {"G", "1"}, {"g", "1"}, {"A", "1"}, {"a", "1"}, {"B", "1"},
        {"C", "2"}, {"c", "2"}, {"D", "2"}, {"d", "2"}, {"E", "2"}, {"F", "2"}, {"f", "2"}, {"G", "2"}, {"g", "2"}, {"A", "2"}, {"a", "2"}, {"B", "2"},
        {"C", "3"}, {"c", "3"}, {"D", "3"}, {"d", "3"}, {"E", "3"}, {"F", "3"}, {"f", "3"}, {"G", "3"}, {"g", "3"}, {"A", "3"}, {"a", "3"}, {"B", "3"},
        {"C", "4"}, {"c", "4"}, {"D", "4"}, {"d", "4"}, {"E", "4"}, {"F", "4"}, {"f", "4"}, {"G", "4"}, {"g", "4"}, {"A", "4"}, {"a", "4"}, {"B", "4"},
        {"C", "5"}, {"c", "5"}, {"D", "5"}, {"d", "5"}, {"E", "5"}, {"F", "5"}, {"f", "5"}, {"G", "5"}, {"g", "5"}, {"A", "5"}, {"a", "5"}, {"B", "5"},
        {"C", "6"}, {"c", "6"}, {"D", "6"}, {"d", "6"}, {"E", "6"}, {"F", "6"}, {"f", "6"}, {"G", "6"}, {"g", "6"}, {"A", "6"}, {"a", "6"}, {"B", "6"},
        {"C", "7"}, {"c", "7"}, {"D", "7"}, {"d", "7"}, {"E", "7"}, {"F", "7"}, {"f", "7"}, {"G", "7"}, {"g", "7"}, {"A", "7"}, {"a", "7"}, {"B", "7"},
        {"C", "8"}, {"c", "8"}, {"D", "8"}, {"d", "8"}, {"E", "8"}, {"F", "8"}, {"f", "8"}, {"G", "8"}
    };

    // Retrieve the note name and octave from the noteTable
    const char* noteName = noteTable[midiNote][0];
    const char* octave = noteTable[midiNote][1];

    // Combine the note name and octave into a single string
    static char note[3];
    note[0] = noteName[0];
    note[1] = octave[0];
    note[2] = '\0';

    return note;
}
