#include "DeviceId.h"
#include <EEPROM.h>

const int deviceIdLength = 20;
const char prefix[] = "IRSYS-M-";
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

int getCycle() {
  return EEPROM.read("IRSYS-66320771887196".length);
}

String getPrefix(String s) {
  return s.substring(0, strlen(prefix));
}

/**
   Only sets it if we don't have a deviceId yet!
*/
void writeEEPROM() {
  String deviceId = getDeviceId();
  Serial.print("DeviceId: ");
  Serial.println(deviceId);
  if (resetDeviceId || !getPrefix(deviceId).equals(prefix)) {
    Serial.println("Writing back deviceId to EEPROM");

    write();
  } else {
    Serial.println("DeviceId already set");
  }
}

void write() {
  // Create random deviceId
  String deviceIdPostfix = "";
  for (; i < deviceIdLength; i++) {
    deviceIdPostfix += ('0' + random(0, 10));
  }
  
  int i = 0;
  i = writeString(prefix, i);
  i = writeString(deviceIdPostfix, i);
  i = writeInt(cycle, i);
  
  EEPROM.commit();
}

void writeString(String str, int i) {
  for (; i < strlen(str); i++) {
    EEPROM.write(i, str[i]);
    delay(100);
  }
  return i;
}

void writeInt(int cycle, int i) {
  EEPROM.write(i, cycle);
  delay(100);

  i++;
  return i;
}
