/*
  Melodía: "Buscando Destino"
  Duración aproximada: 5 segundos (incluyendo pausas)
  Diseñado para: Buzzer pasivo en pin 8
*/

#ifndef MELODIES_H
#define MELODIES_H

// Definición de frecuencias de las notas
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

static const int BZR_PIN = 2;

const int BZR_INTERVAL = 100;
const int BZR_MELODY_START = 0;
const int BZR_MELODY_SEARCH = 1;
const int BZR_MELODY_FOUND = 2;
const int BZR_MELODY_FAIL = 3;
const byte BZR_STATE_STANDBY = 0;
const byte BZR_STATE_PLAYING = 1;

static unsigned long previousMelodyMillis = 0;

// Notas de la melodía
int melodies[][16] = {
  { 
    NOTE_C4, NOTE_E4, NOTE_G4, NOTE_C5, 
    NOTE_D5, NOTE_E5, NOTE_G5, NOTE_C6
  },
  { 
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6, 
    NOTE_A5, NOTE_G5, NOTE_E5, NOTE_G5,
    NOTE_C5, NOTE_E5, NOTE_G5, NOTE_C6,
    NOTE_B5, NOTE_G5, NOTE_E5, NOTE_D5
  },
  {
    NOTE_G5, NOTE_C6, NOTE_E6, NOTE_G6,
    NOTE_G6
  },
  {
    NOTE_DS4, NOTE_D4, NOTE_CS4, NOTE_C4
  }
};

// Duración de las notas: 4 = negra, 8 = corchea
int noteDurations[][16] = {
  { 
    8, 8, 8, 8,
    8, 8, 4, 2 
  },
  { 
    8, 8, 8, 4, 
    8, 8, 8, 4,
    8, 8, 8, 4,
    8, 8, 8, 4
  },
  {
    16, 16, 16, 8,
    2
  },
  {
    4, 4, 4, 1
  }
};

int melodiesDelay[] = {
  100,
  500,
  5000,
  5000
};

int melodiesDuration[] = {
  8,
  16,
  5,
  4
};


// Initialize
void setupMelodyBuzzer() {
  for (int note = 0; note < melodiesDuration[BZR_MELODY_START]; note++) {

    // Calculamos la duración de la nota (1000ms / tipo de nota)
    int noteDuration = 1000 / noteDurations[BZR_MELODY_START][note];
    tone(BZR_PIN, melodies[BZR_MELODY_START][note], noteDuration);

    // Para distinguir las notas, agregamos un pequeño retraso
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }
  Log.noticeln("[BZR] -   Melody Buzzer    [Ready]");
  // Pausa de 1 segundo antes de reiniciar (para completar el ciclo de 5s)
  delay(melodiesDelay[BZR_MELODY_START]);

}

void _play( int melody, int repetition = 1 ){
  for (int note = 0; note < melodiesDuration[melody]; note++) {

    // Calculamos la duración de la nota (1000ms / tipo de nota)
    int noteDuration = 1000 / noteDurations[melody][note];
    tone(BZR_PIN, melodies[melody][note], noteDuration);

    // Para distinguir las notas, agregamos un pequeño retraso
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
  }

  // Pausa de 1 segundo antes de reiniciar (para completar el ciclo de 5s)
  delay(melodiesDelay[melody]);
}

void updateMelodyState() {
  if (currentMillis - previousNeedleMillis < NEEDLE_INTERVAL)
    return;
  
  // its time for another move
  previousNeedleMillis += NEEDLE_INTERVAL;

  switch( needleState )
  {
    case BZR_STATE_STANDBY:
      break;
    case BZR_STATE_PLAYING:
      Log.traceln("[BZR] - Playing a melody.");
      break;
    default:
      break;
  }
}

#endif // MELODIES_H