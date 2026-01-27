/*
  # DnD - Compass
  An arcane magical compass to guide you through your adventure.
*/

#include "config.h"   // Centralized logging & constants
#include <avr/wdt.h>  // Watchdog timer for AVR boards

#include "MagicNeedle.h"
#include "BuzzerManager.h"
#include "StatusGem.h"
#include "CompassLights.h"
#include "BTManager.h"

const char *version = "v0.6.0";

void setup() {
  // High-speed Serial for non-blocking logs
  Serial.begin(115200);
  // Initializing logging
  Log.begin(LOG_LEVEL_VERBOSE, &Serial);
  delay(500);
  Serial.println(version);
  // Hardware Initialization
  Log.noticeln("--- SYSTEM BOOT ---");
  Log.noticeln("[SYS] - # Initiating The Eye of Aldrin");
  config.begin();
  btc.begin();
  bzr.begin();
  gem.begin();
  compass.begin();
  needle.begin();

  // Enable Watchdog Timer (Set to 2 seconds)
  wdt_enable(WDTO_2S); // If loop() takes longer than 2s to complete, the system resets.

  Log.noticeln("[SYS] - # UID: %s", UniqueIDString);
  Log.noticeln("[SYS] - # Ready to Roll (Setup done)");
}

void loop() {
  // Reset the Watchdog timer at the start of every loop
  wdt_reset();

  currentMillis = millis();   // capture the latest value of millis()
                              //   this is equivalent to noting the time from a clock
                              //   use the same time for all LED flashes to keep them synchronized
  
  // State Machines
  btc.update(currentMillis);
  bzr.update(currentMillis);
  gem.update(currentMillis);
  compass.update(currentMillis);
  needle.update(currentMillis);
}
