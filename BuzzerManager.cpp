#include "BuzzerManager.h"

BuzzerManager bzr;

// Define Melodies using the Note struct
const Note MELODY_START[]  = {
  {NOTE_C4,8}, {NOTE_E4,8}, {NOTE_G4,8}, {NOTE_C5,8}, 
  {NOTE_D5,8}, {NOTE_E5,8}, {NOTE_G5,4}, {NOTE_C6,2}};
const Note MELODY_SEARCH[] = {
  {NOTE_C5,8}, {NOTE_E5,8}, {NOTE_G5,8}, {NOTE_C6,4}, 
  {NOTE_A5,8}, {NOTE_G5,8}, {NOTE_E5,8}, {NOTE_G5,4}};
const Note MELODY_FOUND[]  = {
  {NOTE_G5,16}, {NOTE_C6,16}, {NOTE_E6,16}, {NOTE_G6,8}, 
  {NOTE_G6,2}};
const Note MELODY_FAIL[]   = {
  {NOTE_DS4,4}, {NOTE_D4,4}, {NOTE_CS4,4}, {NOTE_C4,1}};

BuzzerManager::BuzzerManager() 
    : _currentMelody(MelodyType::NONE), _noteIndex(0), _nextNoteTime(0), _isPlaying(false) {}

void BuzzerManager::begin() {
  pinMode(BuzzerConfig::PIN, OUTPUT);
  Log.noticeln("[BZR] -   Melody Buzzer    [Ready]");
  play(MelodyType::START); // Play start sound on boot
}

void BuzzerManager::play(MelodyType melody, bool shouldLoop = false) {
    _currentMelody = melody;
    _isLooping = shouldLoop;
    _noteIndex = 0;
    _isPlaying = true;
    _nextNoteTime = 0; // Trigger immediately on next update

    switch(melody) {
        case MelodyType::START:
          _totalNotes = 8;
          break;
        case MelodyType::SEARCH:
          _totalNotes = 8;
          break;
        case MelodyType::FOUND:
          _totalNotes = 5;
          break;
        case MelodyType::FAIL:
          _totalNotes = 4;
          break;
        case MelodyType::NONE:
        default: 
          _isPlaying = false;
          break;
    }
}

void BuzzerManager::stop() {
    noTone(BuzzerConfig::PIN);
    _isPlaying = false;
}

void BuzzerManager::update(unsigned long currentMillis) {
    if (!_isPlaying || currentMillis < _nextNoteTime) return;

    // Get the current note based on the melody
    Note currentNote;
    switch(_currentMelody) {
        case MelodyType::START:
          currentNote = MELODY_START[_noteIndex];
          break;
        case MelodyType::SEARCH:
          currentNote = MELODY_SEARCH[_noteIndex];
          break;
        case MelodyType::FOUND:
          currentNote = MELODY_FOUND[_noteIndex];
          break;
        case MelodyType::FAIL:
          currentNote = MELODY_FAIL[_noteIndex];
          break;
        default:
          return;
    }

    // Calculate duration in ms
    int noteDurationMs = 1000 / currentNote.duration;
    
    if (currentNote.frequency != REST) {
        tone(BuzzerConfig::PIN, currentNote.frequency, noteDurationMs);
    }

    // Set time for next note (duration + the pause)
    _nextNoteTime = currentMillis + (noteDurationMs * BuzzerConfig::PAUSE_COEFFICIENT);
    
    _noteIndex++;
    if (_noteIndex >= _totalNotes) {
        if (_isLooping) {
            _noteIndex = 0; // Reset to the start of the melody
            
            // Optional: Add a longer pause between loops (e.g., 500ms)
            _nextNoteTime += 500; 
        } else {
            _isPlaying = false; // Stop if not looping
        }
    }
}