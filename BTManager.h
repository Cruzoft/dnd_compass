#ifndef BT_MANAGER_H
#define BT_MANAGER_H

#include "config.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "MagicNeedle.h"
#include "CompassLights.h"
#include "BuzzerManager.h"
#include "StatusGem.h"

// Use constexpr for memory efficiency
namespace BTConfig {
    constexpr int INTERVAL = 300;
    constexpr int PIN_TX = 4;
    constexpr int PIN_RX = 7;
    constexpr int FRAME_TIMEOUT = 20;
    constexpr uint32_t BAUD_RATE = 9600;
    constexpr char NAME[] = "Eye-Of-Aldric";
    constexpr char PIN_CODE[] = "1977";
    constexpr size_t BUFFER_SIZE = 32; // Maximum expected command length
}

class BTManager {
public:
    BTManager();
    void begin();
    void update(unsigned long currentMillis);

private:
    SoftwareSerial _btSerial;
    unsigned long _previousMillis;
    char _buffer[BTConfig::BUFFER_SIZE];
    size_t _bufferIdx;
    unsigned long _lastByteTime; // To track inter-byte timeouts

    void _handleCommand(char* cmd);
};

// Global instance to be used in main .ino
extern BTManager btc;

#endif