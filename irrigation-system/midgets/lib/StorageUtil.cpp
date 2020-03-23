#include <Arduino.h>
#include "PermStorageUtil.h"
#include "EEPROMUtil.h"

PermStorageUtil* permStorageUtil;
EEPROMUtil* eepromUtil;

void initializeStorage(bool resetStorage) {
  permStorageUtil->initializeStorage(resetStorage);
  eepromUtil->initializeStorage(resetStorage);
}

String deviceId;
String getDeviceId() {
  if(!deviceId) {
    deviceId = permStorageUtil->getDeviceId();
  }
  return deviceId;
}

int cycle;
int getCycle() {
  if(!cycle) {
    cycle = eepromUtil->getCycle();
  }
  return cycle;
}

void setCycle(int _cycle) {
  cycle = _cycle;
  eepromUtil->setCycle(_cycle);
}

String ssid;
String psk;
void setWifiCredentials(String _ssid, String _psk) {
  if(_ssid != ssid || _psk != psk) {
    ssid = _ssid;
    psk = _psk;
    permStorageUtil->setWifiCredentials(_ssid, _psk);
  }
}

String getWifiSsid() {
  return ssid;
}

String getWifiPsk() {
  return psk;
}
