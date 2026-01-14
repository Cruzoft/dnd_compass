#ifndef STATUS_GEM_H
#define STATUS_GEM_H

#include "config.h"

namespace StatusGemConfig {
    constexpr int PIN_R = 3;
    constexpr int PIN_G = 5;
    constexpr int PIN_B = 6;
}
enum class GemState {
    STANDBY,
    SEARCHING,
    SUCCESS,
    FAIL
};

struct GemColor {
    uint8_t r, g, b;
};

class StatusGem {
public:
    StatusGem();
    void begin();
    void update(unsigned long currentMillis);
    void setState(GemState newState);

private:
    void _updateAnimation(unsigned long currentMillis);
    void _applyCurrentColor();
    void _pickNextTargetColor();

    GemState _state;
    GemColor _current; // The color actually being displayed right now
    GemColor _start;   // The color we started the transition from
    GemColor _target;  // The color we are moving towards

    unsigned long _transitionStartTime;
    uint16_t _transitionDuration; // How long the current glide should take
    uint8_t _paletteIndex;
};

extern StatusGem gem;

#endif // STATUS_GEM_H