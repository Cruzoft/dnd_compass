#ifndef MAGIC_NEEDLE_H
#define MAGIC_NEEDLE_H

#include "config.h"
#include <Servo.h>

namespace NeedleConfig {
    constexpr int PIN = 3;
    constexpr int INTERVAL = 80;
    constexpr int POS_MULTIPLIER = 20; 
    constexpr int MAX_DEGREES = 170;
    constexpr int MIN_DEGREES = 10;
    constexpr int DEFAULT_SPEED = 3;
    constexpr int START_POS = 90;
}

enum class NeedleState {
    STANDBY,
    MOVE,
    SCAN
};

class MagicNeedle {
public:
    MagicNeedle();
    void begin();
    void update(unsigned long currentMillis);
    
    void startScanning();
    void moveToPosition(int position);
    void moveToCenter();
    void stop();

private:
    Servo _servo;
    NeedleState _state;
    unsigned long _previousMillis;
    
    int _currentPos;
    int _targetPos;
    int _speed;

    void _processMove();
    void _processScan();
    void _writeToServo(int angle);
    void _attachIfNeeded();
};

extern MagicNeedle needle;

#endif // MAGIC_NEEDLE_H