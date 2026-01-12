
#ifndef STATUS_GEM_H
#define STATUS_GEM_H

static const int GEM_PIN_R = A0;
static const int GEM_PIN_G = A1;
static const int GEM_PIN_B = A2;

setColor(0, 255, 255); // Cian

const int colors[][3] = {
  {0, 255, 255} // Cian
  {255, 150, 0}, // Yellow/Orange
}

// Initialize
void setupStatusGem() {
  pinMode(GEM_PIN_R, OUTPUT);
  pinMode(GEM_PIN_G, OUTPUT);
  pinMode(GEM_PIN_B, OUTPUT);

  Log.noticeln("[GEM] -   Status Gem       [Not Ready]");
}

void _setColor(int r, int g, int b) {
  analogWrite(GEM_PIN_R, r);
  analogWrite(GEM_PIN_R, g);
  analogWrite(GEM_PIN_B, b);
}

void updateStatusGemState() {
}

#endif // STATUS_GEM_H