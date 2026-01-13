#include "BTManager.h"

BTManager btc; // Instance definition

BTManager::BTManager() 
    : _btSerial(BTConfig::PIN_RX, BTConfig::PIN_TX), 
      _previousMillis(0), 
      _bufferIdx(0) {
    memset(_buffer, 0, BTConfig::BUFFER_SIZE);
}

void BTManager::begin() {
    _btSerial.begin(BTConfig::BAUD_RATE);
    
    // HM-10 Setup Logic
    _btSerial.print("AT+NAME");
    _btSerial.print(BTConfig::NAME);
    _btSerial.print("\r\n");
    
    _btSerial.print("AT+PIN");
    _btSerial.print(BTConfig::PIN_CODE);
    _btSerial.print("\r\n");
    
    _btSerial.print("AT+ROLE0\r\n"); 
    _btSerial.print("AT+RESET\r\n"); 

    Log.noticeln("BTC] -   Bluetooth        [Ready]");
}

void BTManager::update(unsigned long currentMillis) {
    // Non-blocking timing
    if (currentMillis - _previousMillis >= BTConfig::INTERVAL) {
        _previousMillis = currentMillis;
        _readSerial();
    }
}

void BTManager::_readSerial() {
    while (_btSerial.available() > 0) {
        char c = _btSerial.read();

        // Check for end of line or buffer overflow
        if (c == '\n' || c == '\r' || _bufferIdx >= BTConfig::BUFFER_SIZE - 1) {
            if (_bufferIdx > 0) {
                _buffer[_bufferIdx] = '\0'; // Null terminate
                _handleCommand(_buffer);
                _bufferIdx = 0; // Reset buffer
            }
        } else {
            _buffer[_bufferIdx++] = c;
        }
    }
}

void BTManager::_handleCommand(char* cmd) {
    if (strcmp(cmd, "CMD_STOP") == 0) {
        Log.noticeln("[BTC] - STOP");
        needle.stop();
    } 
    else if (strcmp(cmd, "CMD_SCAN") == 0) {
        Log.noticeln("[BTC] - SCAN");
        needle.startScanning();
    } 
    else if (strcmp(cmd, "CMD_LOST") == 0) {
        needle.moveToCenter();
        Log.noticeln("[BTC] - LOST");
    } 
    else if (strncmp(cmd, "CMD_MOVE_TO_", 12) == 0) {
        // Extract integer from position 12 onwards
        int position = atoi(cmd + 12); 
        needle.moveToPosition(position);
        Log.noticeln("[BTC] - MOVE TO %d", position);
    }
}