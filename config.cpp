#include "config.h"

long currentMillis = 0;
char UniqueIDString[(UniqueIDsize * 2) + 1];

CompassConfig config; 

void CompassConfig::begin() {
  byte index = 0;
  for (size_t i = UniqueIDsize - 2; i < UniqueIDsize; i++)
  {
    UniqueIDString[index++] = "0123456789ABCDEF"[UniqueID[i] >> 4];
    UniqueIDString[index++] = "0123456789ABCDEF"[UniqueID[i]  & 0x0F];
  }
  UniqueIDString[index++] = 0; // Null Terminator

  Log.noticeln("[CFG] -   Compass Config   [Ready]");
}
