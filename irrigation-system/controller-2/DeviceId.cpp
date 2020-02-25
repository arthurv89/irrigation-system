#include "DeviceId.h"
#include <EEPROM.h>

const int deviceIdLength = 20;
const char prefix[] = "IRSYS-";
const boolean resetDeviceId = false;

String getDeviceId() {
  char deviceId[deviceIdLength+1];
  memset(deviceId, 0, sizeof deviceId);
  for (int i = 0; i < deviceIdLength; i++) {
    deviceId[i] = EEPROM.read(i);
  }
  deviceId[deviceIdLength] = '\0';
//  String deviceId = "IRSYS-66320771887196";
  return deviceId;
}

String getPrefix(String s) {
  return s.substring(0, strlen(prefix));
}

/**
   Only sets it if we don't have a deviceId yet!
*/
void setDeviceId() {
  String deviceId = getDeviceId();
  Serial.print("DeviceId: ");
  Serial.println(deviceId);
  if (resetDeviceId || !getPrefix(deviceId).equals(prefix)) {
    Serial.println("Writing back deviceId to EEPROM");

    // Create random deviceId and write it back to EEPROM
    for (int i = 0; i < strlen(prefix); i++) {
      EEPROM.write(i, prefix[i]);
      delay(100);
    }
    for (int i = strlen(prefix); i < deviceIdLength; i++) {
      char x = '0' + random(0, 10);
      EEPROM.write(i, x);
      delay(100);
    }
    EEPROM.commit();
    for (int i = 0; i < deviceIdLength; i++) {
      char y = (char) EEPROM.read(i);
      Serial.print(y);
    }
    Serial.println("");
  } else {
    Serial.println("DeviceId already set");
  }
}
