#ifndef U_CONTROLLER_H
#define U_CONTROLLER_H
#include "b_definitions.h"
#include "j_displayFunctions.h"
#include "c_appVariables.h"
#include "i_initializers.h"

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h> //when creating default instance we need to use "Adafruit_SH1106G"


void drawControllerOptions();
void drawControllerList();



#endif // U_CONTROLLER_H
