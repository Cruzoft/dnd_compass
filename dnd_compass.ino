/*
  # DnD - Compass
  An arcane magical compass to guide you through your adventure.
*/

#define DISABLE_LOGGING // Uncomment this line to prevent logging library to be compiled

#include <EEPROM.h>

#include "ArduinoLog.h"

#include "config.h"
#include "magic_needle.h"
#include "bt_connect.h"

const char *version = "v0.2.2";

void setup() {
  // write a 0 to all 512 bytes of the EEPROM
  //for (int i = 0; i < 512; i++)
  //  EEPROM.write(i, 0);
    
  Serial.begin(9600);
  while(!Serial && !Serial.available()){}
  // logging
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  Serial.println(version);
  // put your setup code here, to run once:
  Log.noticeln("--- BOOT ---");
  Log.noticeln("[DnD] - # Initiating The Eye of Aldrin");
  setupBTConnect();
  setupNeedle();
  Log.noticeln("[DnD] - # Ready to Roll (Setup done)");
}

void loop() {
  currentMillis = millis();   // capture the latest value of millis()
                              //   this is equivalent to noting the time from a clock
                              //   use the same time for all LED flashes to keep them synchronized
  updateBTCState();
  updateMagicNeedleState();
}
