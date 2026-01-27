#include "BTManager.h"

BTManager btc; // Instance definition

BTManager::BTManager() 
    : _btSerial(BTConfig::PIN_TX, BTConfig::PIN_RX), // Some boards, like mine, has this pins inverted. If BT is not working for you try inverting the pins.
      _previousMillis(0), 
      _bufferIdx(0) {
    memset(_buffer, 0, BTConfig::BUFFER_SIZE);
}

void BTManager::begin() {
    _btSerial.begin(BTConfig::BAUD_RATE);
    _btSerial.listen();
    
    // HM-10 Setup Logic
    _btSerial.print("AT+NAME");
    _btSerial.print(BTConfig::NAME);
    _btSerial.print("\r\n");
    
    _btSerial.print("AT+PIN");
    _btSerial.print(BTConfig::PIN_CODE);
    _btSerial.print("\r\n");
    
    _btSerial.print("AT+ROLE0\r\n"); 
    _btSerial.print("AT+RESET\r\n"); 

    Log.noticeln("[BTC] -   Bluetooth        [Ready]");
}

void BTManager::update(unsigned long currentMillis) {
    while (_btSerial.available() > 0) {
        char c = _btSerial.read();
        //Serial.print(c);
        _lastByteTime = currentMillis; 

        if (c == '\n' || c == '\r') {
            if (_bufferIdx > 0) {
                _buffer[_bufferIdx] = '\0';
                _handleCommand(_buffer);
                _bufferIdx = 0;
            }
        } else if (_bufferIdx < BTConfig::BUFFER_SIZE - 1) {
            _buffer[_bufferIdx++] = c;
        }
    }

    // 2. TIMEOUT CHECK (Only process if no new data arrived for 50ms)
    if (_bufferIdx > 0 && (currentMillis - _lastByteTime > BTConfig::FRAME_TIMEOUT)) {
        Log.traceln("[BTC] - Timeout Check");
        _buffer[_bufferIdx] = '\0';
        _handleCommand(_buffer); // This will finally trigger!
        _bufferIdx = 0;
    }
}

void BTManager::_handleCommand(char* cmd) {
    if (strcmp(cmd, "CMD_STOP") == 0) {
        Log.noticeln("[BTC] - STOP");
        gem.setState(GemState::STANDBY);
        bzr.play(MelodyType::NONE);
        needle.stop();
    } 
    else if (strcmp(cmd, "CMD_SCAN") == 0) {
        Log.noticeln("[BTC] - SCAN");
        needle.startScanning();
        gem.setState(GemState::SEARCHING);
        bzr.play(MelodyType::SEARCH, true); // Play the search melody indefinitely
    } 
    else if (strcmp(cmd, "CMD_LOST") == 0) {
        needle.moveToCenter();
        gem.setState(GemState::FAIL);
        bzr.play(MelodyType::FAIL);
        Log.noticeln("[BTC] - LOST");
    } 
    else if (strncmp(cmd, "CMD_MOVE_TO_", 12) == 0) {
        // Extract integer from position 12 onwards
        int position = atoi(&cmd[12]); // This is the Servo target position
        // Extract chars from position 15 onwards
        char* cardinal = &cmd[15]; // This is the Compass target direction
        needle.moveToPosition(position);
        compass.setState(CompassState::SUCCESS);
        compass.setTargetByCardinal(cardinal);
        gem.setState(GemState::SUCCESS);
        bzr.play(MelodyType::FOUND);
        Log.noticeln("[BTC] - MOVE TO %d FACING %s", position, cardinal);
    }
}