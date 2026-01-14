#ifndef BUZZER_MANAGER_H
#define BUZZER_MANAGER_H

#include "config.h"

// Notes frequencies
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_C5  523 
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_G5  784
#define NOTE_A5  880
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_E6  1319
#define NOTE_G6  1568
#define REST     0


namespace BuzzerConfig {
    constexpr int PIN = 8;
    constexpr float PAUSE_COEFFICIENT = 1.30;
}

enum class MelodyType {
    NONE,
    START,
    SEARCH,
    FOUND,
    FAIL
};

struct Note {
    int frequency;
    int duration; // 4 = quarter, 8 = eighth, etc.
};

class BuzzerManager {
  public:
    BuzzerManager();
    void begin();
    void update(unsigned long currentMillis);
    void play(MelodyType melody, bool shouldLoop = false);
    void stop();
private:
    MelodyType _currentMelody;
    int _noteIndex;
    int _totalNotes;
    unsigned long _nextNoteTime;
    bool _isPlaying;
    bool _isLooping;

    void _getNextNote();
};

extern BuzzerManager bzr;

#endif // BUZZER_MANAGER_H