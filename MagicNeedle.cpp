#include "MagicNeedle.h"

MagicNeedle needle; // Global instance

MagicNeedle::MagicNeedle() 
    : _state(NeedleState::STANDBY), 
      _previousMillis(0),
      _currentPos(NeedleConfig::START_POS),
      _targetPos(NeedleConfig::START_POS),
      _speed(NeedleConfig::DEFAULT_SPEED) {}

void MagicNeedle::begin() {
    // Initial hardware shake: attach, move, then detach to save power
    _servo.attach(NeedleConfig::PIN);
    _servo.write(_currentPos);
    
    // In production firmware, we avoid delay() even in setup if we can,
    // but a small startup delay is acceptable for hardware stabilization.
    delay(500); 
    _servo.detach();
    
    Log.noticeln("[MDL] -   Magic Needle     [Ready]");
}

void MagicNeedle::update(unsigned long currentMillis) {
    if (currentMillis - _previousMillis < NeedleConfig::INTERVAL) {
        return;
    }
    _previousMillis = currentMillis;

    switch (_state) {
        case NeedleState::MOVE:
            _processMove();
            break;
        case NeedleState::SCAN:
            _processScan();
            break;
        case NeedleState::STANDBY:
        default:
            break;
    }
}

void MagicNeedle::startScanning() {
    Log.noticeln("[MDL] - Starting Scan");
    _speed = abs(NeedleConfig::DEFAULT_SPEED);
    _state = NeedleState::SCAN;
    _attachIfNeeded();
}

void MagicNeedle::moveToPosition(int position) {
    _targetPos = position * NeedleConfig::POS_MULTIPLIER;
    
    // Constrain target within physical limits
    _targetPos = constrain(_targetPos, NeedleConfig::MIN_DEGREES, NeedleConfig::MAX_DEGREES);
    
    _state = NeedleState::MOVE;
    _attachIfNeeded();
    Log.noticeln("[MDL] - Moving to Position: %d (Angle: %d)", position, _targetPos);
}

void MagicNeedle::moveToCenter() {
    moveToPosition(90 / NeedleConfig::POS_MULTIPLIER);
}

void MagicNeedle::stop() {
    _state = NeedleState::STANDBY;
    _servo.detach(); // Disable PWM to stop jitter and save power
    Log.noticeln("[MDL] - Needle Stopped and Detached");
}

void MagicNeedle::_processMove() {
    int distance = _targetPos - _currentPos;
    
    // Check if we arrived
    if (abs(distance) <= abs(_speed)) {
        _currentPos = _targetPos;
        _writeToServo(_currentPos);
        stop(); // Arrived at destination
        return;
    }

    // Move step-wise
    if (distance > 0) _currentPos += _speed;
    else _currentPos -= _speed;

    _writeToServo(_currentPos);
}

void MagicNeedle::_processScan() {
    _currentPos += _speed;

    if (_currentPos >= NeedleConfig::MAX_DEGREES || _currentPos <= NeedleConfig::MIN_DEGREES) {
        _speed = -_speed; // Reverse direction
        _currentPos += _speed; // Immediate correction
    }

    _writeToServo(_currentPos);
}

void MagicNeedle::_writeToServo(int angle) {
    int safeAngle = constrain(angle, NeedleConfig::MIN_DEGREES, NeedleConfig::MAX_DEGREES);
    _servo.write(safeAngle);
}

void MagicNeedle::_attachIfNeeded() {
    if (!_servo.attached()) {
        _servo.attach(NeedleConfig::PIN);
    }
}