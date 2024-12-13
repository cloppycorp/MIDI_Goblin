#ifndef S_HANDLEVALEDITS_H
#define S_HANDLEVALEDITS_H

#include "b_definitions.h"
#include "c_appVariables.h"
#include "e_encoderAndButtonVars.h"
#include "d_selectionVariables.h"
#include "c_appVariables.h"
#include "h_deviceVars.h"
#include "i_initializers.h"
#include "j_displayFunctions.h"
#include "r_fileFunctions.h"
#include "k_checkAppStatuses.h"
#include "p_handleNav.h"
#include "q_vectors.h"
#include "v_addcontrollerparam.h"
void handleValueEditsForTriggers();

extern ValueEditConfig midiThruConfig[4];
extern ValueEditConfig autoChordConfig[4];
extern ValueEditConfig arpNoteConfig[4];
extern ValueEditConfig arpSettingsConfig[5];
extern ValueEditConfig arpeggiatorConfig[4]; 
extern ValueEditConfig2 lfoConfig[5]; 
extern ValueEditConfig2 midiOutChannelsConfig[11];
void handleValueEditsForTriggerFunctions();
void handleValueEditsForReMap();
void handleValueEditsForDelete();
void handleValueEditsForSaveNewFile();
void handleValueEditsForSequencer();
void finishNoteEdit();
void handleValueEditsForMidiOutChannels();
void handleValueEditsForController();
void handleValueEditsForLfo();
void handleValueEditsForApp(ValueEditConfig* valueEditConfig, int configSize);
void handleValueEdits();
void handleValueEditsForSeqChordEntry();
void handleValueEditsForAddControllerParam();
void handleValueEditsForControllerOptions();
void handleValueEditsForSeqSwang();
void handleValueEditsForClockSource();
#endif // S_HANDLEVALEDITS_H
