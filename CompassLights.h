
#ifndef COMPASS_LIGHTS_H
#define COMPASS_LIGHTS_H

#include "config.h"

namespace CompassLightsConfig {
    // Defining the pins this ways makes easier to create animations
    const int PINS[] = {9, 13, 10, A3, 11, A4, 12, A5}; 
    constexpr int COUNT = 8;
}

enum class CompassState {
    STANDBY,
    SEARCHING,
    SUCCESS,
    FAIL
};

class CompassLights {
public:
    CompassLights();
    void begin();
    void update(unsigned long currentMillis);
    void setState(CompassState newState);
    void setTargetDirection(int index); // 0 to 7
    void setTargetByCardinal(const char* cardinal);

private:
    void _updateAnimation(unsigned long currentMillis);
    void _allOff();
    int _getIndexFromCardinal(const char* cardinal);
    
    CompassState _state;
    int _targetDirection; 
    int _currentStep;
    unsigned long _lastAnimMillis;
};

extern CompassLights compass;


#endif // COMPASS_LIGHTS_H