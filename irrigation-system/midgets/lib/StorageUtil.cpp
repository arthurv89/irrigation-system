#include <Arduino.h>
#include "StorageUtil.h"
#include "PermStorageUtil.h"
#include "EEPROMUtil.h"
#include "Utils.h"

PermStorageUtil permStorageUtil;
EEPROMUtil eepromUtil;

int deviceIdLength = 20;

StaticJsonDocument<200> data;

void initializeStorage(bool resetStorage) {
  eepromUtil.initializeEEPROMStorage(resetStorage);
  permStorageUtil.initializePermStorage(resetStorage);

  readEEPROMValues();
  if(!data["deviceId"]) {
    // EEPROM not initialised yet.
    readPermStorageValues();
  }

  if(!data["deviceId"]) {
    // Perm storage also nevery initialised.
    // Initialise now.
    data["deviceId"] = createDeviceId();
    data["cycle"] = 0;
    updateEEPROM();
    updatePerm();
  }
}

void readPermStorageValues() {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, permStorageUtil.read_json());
  if(!error) {
    data["deviceId"] = doc["deviceId"];
    data["wifi_ssid"] = doc["wifi_ssid"];
    data["wifi_psk"] = doc["wifi_psk"];
  }
}

void readEEPROMValues() {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, eepromUtil.read_json());
  if(!error) {
    data["deviceId"] = doc["deviceId"];
    data["cycle"] = doc["cycle"];
    data["wifi_ssid"] = doc["wifi_ssid"];
    data["wifi_psk"] = doc["wifi_psk"];
  }
  Serial.println(_serializeJson(data));
}

void updateEEPROM() {
  StaticJsonDocument<200> doc;
  doc["deviceId"] = data["deviceId"];
  doc["cycle"] = data["cycle"];
  doc["wifi_ssid"] = data["wifi_ssid"];
  doc["wifi_psk"] = data["wifi_psk"];

  eepromUtil.write_json(doc);
}

void updatePerm() {
  StaticJsonDocument<200> doc;
  doc["deviceId"] = data["deviceId"];
  doc["wifi_ssid"] = data["wifi_ssid"];
  doc["wifi_psk"] = data["wifi_psk"];

  permStorageUtil.write_json(doc);
}


String createDeviceId() {
  // Create random deviceId
  String deviceIdPostfix = "";
  for (int i=0; i < deviceIdLength; i++) {
    deviceIdPostfix += char(('0' + random(0, 10)));
  }
  return deviceIdPostfix;
}



String getDeviceId() {
  return data["deviceId"];
}

int getCycle() {
  return data["cycle"];
}

void setCycle(int cycle) {
  data["cycle"] = cycle;
  updateEEPROM();
}

String getWifiSsid() {
  return data["wifi_ssid"];
}

String getWifiPsk() {
  return data["wifi_psk"];
}

void setWifiCredentials(String ssid, String psk) {
  if(data["wifi_ssid"] != ssid || data["wifi_psk"] != psk) {
    data["wifi_ssid"] = ssid;
    data["wifi_psk"] = psk;
    updateEEPROM();
    updatePerm();
  }
}
