
#ifndef MAGIC_NEEDLE_H
#define MAGIC_NEEDLE_H

#include <Arduino.h> // Necessary for Arduino functions in this file
#include <Servo.h> // Include Servo library

const int NEEDLE_INTERVAL = 80;
const int NEEDLE_POSITIONS = 20; // Options: 1, 2, 4, 5, 8, 10, 16, 20
static int NEEDLE_PIN = 3;
const byte NEEDLE_STANDBY = 0;
const byte NEEDLE_MOVE = 1;
const byte NEEDLE_SCAN = 2;

static Servo needleServo;
static byte needleState = NEEDLE_STANDBY;
static unsigned long previousNeedleMillis = 0;
static int needlePosition = 90;
static int needleTargetPosition = 90;
static int needleSpeed = 3;
static int servoMaxDegrees = 170;
static int servoMinDegrees = 10;


// Initialize
void setupNeedle() {
    needleServo.attach(NEEDLE_PIN);
    needleServo.write(needlePosition); // Initial angle
    delay(1000);
    needleServo.detach();
    Log.noticeln("[MDL] -   Magic Needle     [Ready]");
}

void startNeedleScanning() {
  needleState = NEEDLE_SCAN;
  needleServo.attach(NEEDLE_PIN);
}

void moveNeedleToPossition(int position) {
  needleSpeed = abs(needleSpeed);
  needleState = NEEDLE_MOVE;
  needleTargetPosition = position * NEEDLE_POSITIONS;
  needleServo.attach(NEEDLE_PIN);
}

void moveNeedleToCenter() {
  needleSpeed = abs(needleSpeed);
  needleState = NEEDLE_MOVE;
  needleTargetPosition = 90;
  needleServo.attach(NEEDLE_PIN);
}

void stopNeedle() {
  needleSpeed = abs(needleSpeed);
  needleState = NEEDLE_STANDBY;
  needleServo.detach();
  Log.traceln("[MDL] - Needle stoped.");
}

void _writeServo(int newPosition){
  if (newPosition > servoMaxDegrees)
    newPosition = servoMaxDegrees;
  else if (newPosition < servoMinDegrees)
    newPosition = servoMinDegrees;
  needleServo.write(newPosition);
  delay(10);
}

void _scan() {
  needlePosition = needlePosition + needleSpeed; // needleSpeed might be negative

  if ((needlePosition >= servoMaxDegrees) || (needlePosition <= servoMinDegrees))  {
        // if the servo is at either extreme change the sign of the degrees to make it move the other way
    needleSpeed = - needleSpeed; // reverse direction
        // and update the position to ensure it is within range
    needlePosition = needlePosition + needleSpeed; 
  }
      // make the servo move to the next position
  _writeServo(needlePosition);
      // and record the time when the move happened
}

void _move() {
  if ( abs(needlePosition - needleTargetPosition) <= 1 ){
    stopNeedle();
    return;
  }
  int distance = needleTargetPosition - needlePosition;
  Log.traceln("[MDL] - Needle Distance: %d.", abs(distance));
  if ( abs(distance) >= needleSpeed ) {
    if (distance > 0) {
      needlePosition = needlePosition + needleSpeed; 
    } else {
      needlePosition = needlePosition - needleSpeed; 
    }
  } else {
    Log.traceln("[MDL] - Needle moving at custom speed.");
    if (distance > 0) {
      needlePosition = needlePosition - distance; 
    } else {
      needlePosition = needlePosition + distance; 
    }
  }
  // make the servo move to the next position
  _writeServo(needlePosition);
}


void updateMagicNeedleState() {
  if (currentMillis - previousNeedleMillis < NEEDLE_INTERVAL)
    return;
  
  // its time for another move
  previousNeedleMillis += NEEDLE_INTERVAL;

  switch( needleState )
  {
    case NEEDLE_STANDBY:
      break;
    case NEEDLE_MOVE:
      Log.traceln("[MDL] - Moving needle from %d to %d at %d.", needlePosition, needleTargetPosition, needleSpeed);
      _move();
      break;
    case NEEDLE_SCAN:
      Log.traceln("[MDL] - Scanning on %d at %d.", needlePosition, needleSpeed);
      _scan();
      break;
    default:
      break;
  }
}

#endif // MAGIC_NEEDLE_H