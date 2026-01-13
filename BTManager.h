#ifndef BT_MANAGER_H
#define BT_MANAGER_H

#include "config.h"
#include <Arduino.h>
#include <SoftwareSerial.h>
#include "MagicNeedle.h"

// Use constexpr for memory efficiency
namespace BTConfig {
    constexpr int INTERVAL = 300;
    constexpr int PIN_TX = 5;
    constexpr int PIN_RX = 4;
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

    void _handleCommand(char* cmd);
    void _readSerial();
};

// Global instance to be used in main .ino
extern BTManager btc;

#endif