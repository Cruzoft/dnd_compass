
#ifndef BT_CONNECT_H
#define BT_CONNECT_H

#include <SoftwareSerial.h>

const int BTC_INTERVAL = 300;
const int BTC_PIN_TX = 5;
const int BTC_PIN_RX = 4;
const String BTC_NAME = "Eye-Of-Aldric";
const String BTC_CODE_PIN = "1977";
const byte BTC_STANDBY = 0;

static unsigned long previousBTConnectMillis = 0;

SoftwareSerial btCon(BTC_PIN_RX, BTC_PIN_TX); // RX, TX

// Initialize
void setupBTConnect() {
    btCon.begin(9600);
    btCon.print(String("AT+NAME") + BTC_NAME + String("\r\n"));
    btCon.print(String("AT+PIN") + BTC_CODE_PIN + String("\r\n"));
    btCon.print(String("AT+ROLE0\r\n")); // Sets the BT role to slave
    btCon.print(String("AT+RESET\r\n")); // Resets the module to make sure chagnes are applied
    Log.noticeln("[BTC] -   Bluetooth        [Ready]");
}

void updateBTCState() {

  if (currentMillis - previousBTConnectMillis >= BTC_INTERVAL) {
        // its time for another move
    previousBTConnectMillis += BTC_INTERVAL;

    String btData = "";
    while (btCon.available()) {
      delay(10);
      char btValue = btCon.read();

      btData += btValue;
    }
    //Serial.println(btData)
    // (btData == "12345678901234567890"){
    if (btData == "CMD_STOP"){
        Log.noticeln("[BTC] - STOP");
        stopNeedle();
    } else if (btData == "CMD_SCAN"){
      Log.noticeln("[BTC] - SCAN");
      startNeedleScanning();
    } else if (btData == "CMD_LOST"){
      Log.noticeln("[BTC] - LOST ");
    } else if (btData.startsWith("CMD_MOVE_TO_")){
      btData.replace("CMD_MOVE_TO_", "");
      int position = btData.toInt();
      moveNeedleToPossition(position);
      Log.noticeln("[BTC] - MOVE TO %d", position);
    } 
  }
}

#endif // BT_CONNECT_H