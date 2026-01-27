
#ifndef CONFIG_H
#define CONFIG_H

// UNCOMMENT this line to disable logging everywhere
#define DISABLE_LOGGING 

#include "ArduinoLog.h"
#include <ArduinoUniqueID.h>

extern long currentMillis;
extern char UniqueIDString[];


class CompassConfig {
public:
  void begin();
};

extern CompassConfig config;
#endif // CONFIG_H