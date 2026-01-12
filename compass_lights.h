
#ifndef COMPASS_LIGHTS_H
#define COMPASS_LIGHTS_H

static const int CLT_PIN_N = 6;
static const int CLT_PIN_NE = 7;
static const int CLT_PIN_E = 8;
static const int CLT_PIN_SE = 9;
static const int CLT_PIN_S = 10;
static const int CLT_PIN_SW = 11;
static const int CLT_PIN_W = 12;
static const int CLT_PIN_NW = 13;

// Initialize
void setupCompassLights() {
  pinMode(CLT_PIN_N, OUTPUT);
  pinMode(CLT_PIN_NE, OUTPUT);
  pinMode(CLT_PIN_E, OUTPUT);
  pinMode(CLT_PIN_SE, OUTPUT);
  pinMode(CLT_PIN_S, OUTPUT);
  pinMode(CLT_PIN_SW, OUTPUT);
  pinMode(CLT_PIN_W, OUTPUT);
  pinMode(CLT_PIN_NW, OUTPUT);
  
  Log.noticeln("[CLT] -   Compass Lights   [Not Ready]");
}

void updateCompassLightsState() {
}

#endif // COMPASS_LIGHTS_H