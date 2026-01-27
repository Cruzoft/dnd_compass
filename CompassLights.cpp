#include "CompassLights.h"

CompassLights compass; // Global instance
CompassLights::CompassLights() : 
    _state(CompassState::STANDBY), 
    _targetDirection(0), 
    _currentStep(0), 
    _lastAnimMillis(0) 
{}

void CompassLights::begin() {
    for (int i = 0; i < CompassLightsConfig::COUNT; i++) {
        pinMode(CompassLightsConfig::PINS[i], OUTPUT);
    }
    _allOff();
    Log.noticeln("[CLT] -   Compass Ligths   [Ready]");
}

void CompassLights::setState(CompassState newState) {
    _state = newState;
    _allOff();
}
int CompassLights::_getIndexFromCardinal(const char* cardinal) {
    if (strcmp(cardinal, "N") == 0)  return 0;
    if (strcmp(cardinal, "NE") == 0) return 1;
    if (strcmp(cardinal, "E") == 0)  return 2;
    if (strcmp(cardinal, "SE") == 0) return 3;
    if (strcmp(cardinal, "S") == 0)  return 4;
    if (strcmp(cardinal, "SW") == 0) return 5;
    if (strcmp(cardinal, "W") == 0)  return 6;
    if (strcmp(cardinal, "NW") == 0) return 7;
    return 0; // Por defecto al Norte si no se reconoce
}

void CompassLights::setTargetByCardinal(const char* cardinal) {
    _targetDirection = _getIndexFromCardinal(cardinal);
}

void CompassLights::setTargetDirection(int index) {
    _targetDirection = index % CompassLightsConfig::COUNT;
}

void CompassLights::update(unsigned long currentMillis) {
    _updateAnimation(currentMillis);
}

void CompassLights::_updateAnimation(unsigned long currentMillis) {
    switch (_state) {
        case CompassState::SEARCHING:
            // Animación de rotación (Chaser)
            if (currentMillis - _lastAnimMillis > 100) { // Velocidad de giro
                _lastAnimMillis = currentMillis;
                _allOff();
                digitalWrite(CompassLightsConfig::PINS[_currentStep], HIGH);
                _currentStep = (_currentStep + 1) % CompassLightsConfig::COUNT;
            }
            break;

        case CompassState::SUCCESS:
            // Encender solo la luz que apunta al objetivo
            _allOff();
            digitalWrite(CompassLightsConfig::PINS[_targetDirection], HIGH);
            break;

        case CompassState::FAIL:
            // Parpadeo de todas las luces (opcional)
            if ((currentMillis / 250) % 2) {
                for(int i=0; i<8; i++) digitalWrite(CompassLightsConfig::PINS[i], HIGH);
            } else {
                _allOff();
            }
            break;

        case CompassState::STANDBY:
        default:
            _allOff();
            break;
    }
}

void CompassLights::_allOff() {
    for (int i = 0; i < CompassLightsConfig::COUNT; i++) {
        digitalWrite(CompassLightsConfig::PINS[i], LOW);
    }
}