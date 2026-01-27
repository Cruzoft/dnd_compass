#include "StatusGem.h"

StatusGem gem; // Global instance

// Define the "Arcane" Palettes
const GemColor PALETTE_STANDBY[] = {{200,200,200}, {50,50,100}, {20,80,40}, {100,100,100}}; // White, Gray, Light Blue, Light Green
const GemColor PALETTE_SEARCH[]  = {{220,150,0}, {200,50,0}};                             // Yellow, Orange
const GemColor PALETTE_SUCCESS[] = {{0,255,100}, {0,100,255}};                            // Green, Blue
const GemColor PALETTE_FAIL[] = {{255,0,0}, {255,60,0}};                               // Red, Deep Orange

StatusGem::StatusGem() : _state(GemState::STANDBY), _transitionStartTime(0), _paletteIndex(0) {
    _current = {0,0,0};
}

void StatusGem::begin() {
    pinMode(StatusGemConfig::PIN_R, OUTPUT);
    pinMode(StatusGemConfig::PIN_G, OUTPUT);
    pinMode(StatusGemConfig::PIN_B, OUTPUT);
    setState(GemState::STANDBY);
    Log.noticeln("[GEM] -   Status Gem       [Ready]");
}

void StatusGem::setState(GemState newState) {
    if (_state == newState && newState != GemState::STANDBY) return;
    _state = newState;
    _paletteIndex = 0;
    _transitionDuration = 1000; // Start with a 1s transition
    _pickNextTargetColor();
}

void StatusGem::_pickNextTargetColor() {
    _start = _current;
    _transitionStartTime = millis();
    
    switch(_state) {
        case GemState::STANDBY:
            _target = PALETTE_STANDBY[_paletteIndex];
            _paletteIndex = (_paletteIndex + 1) % 4;
            _transitionDuration = 3000; // Slow breathing
            break;
        case GemState::SEARCHING:
            _target = PALETTE_SEARCH[_paletteIndex];
            _paletteIndex = (_paletteIndex + 1) % 2;
            _transitionDuration = 2500;  // Fast pulsing
            break;
        case GemState::SUCCESS:
            _target = PALETTE_SUCCESS[_paletteIndex];
            _paletteIndex = (_paletteIndex + 1) % 2;
            _transitionDuration = 2500;
            break;
        case GemState::FAIL:
            _target = PALETTE_FAIL[_paletteIndex];
            _paletteIndex = (_paletteIndex + 1) % 2;
            _transitionDuration = 1500; // Urgent flicker
            break;
    }
}

void StatusGem::update(unsigned long currentMillis) {
    float progress = (float)(currentMillis - _transitionStartTime) / _transitionDuration;

    if (progress >= 1.0f) {
        _current = _target;
        _pickNextTargetColor(); // Loop to next color in palette
    } else {
        // LERP formula
        _current.r = _start.r + (_target.r - _start.r) * progress;
        _current.g = _start.g + (_target.g - _start.g) * progress;
        _current.b = _start.b + (_target.b - _start.b) * progress;
    }

    _applyCurrentColor();
}

void StatusGem::_applyCurrentColor() {
    analogWrite(StatusGemConfig::PIN_R, _current.r);
    analogWrite(StatusGemConfig::PIN_G, _current.g);
    analogWrite(StatusGemConfig::PIN_B, _current.b);
}