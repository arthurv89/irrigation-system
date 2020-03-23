#include <Arduino.h>
#include "PermStorageUtil.h"
#include "FS.h"
#include "Utils.h"

const bool isDebug = false;

const String path = "/data.txt";
const int deviceIdLength = 20;

void PermStorageUtil::initializeStorage(bool resetPermStorage) {
  SPIFFS.begin();

  json = read_json();

  if (resetPermStorage) {
    StaticJsonDocument<200> doc;
    doc["deviceId"] = createDeviceId();
    write_json(doc);
  }
}

StaticJsonDocument<200> PermStorageUtil::read_json() {
  String contents = "";

  File f = SPIFFS.open(path, "r");
  while (f.available()){
    contents += f.readStringUntil('\n');
  }
  f.close();

  return _deserializeJson(contents);
}

void PermStorageUtil::write_json(StaticJsonDocument<200> doc) {
  String contents = _serializeJson(doc);

  File f = SPIFFS.open(path, "w");
  f.print(contents);
  f.close();
  json = doc;
}

String PermStorageUtil::getDeviceId() {
  return json["deviceId"];
}

String PermStorageUtil::getWifiSsid() {
  return json["wifi-ssid"];
}

String PermStorageUtil::getWifiPsk() {
  return json["wifi-psk"];
}

void PermStorageUtil::setWifiCredentials(String ssid, String psk) {
  json["wifi-ssid"] = ssid;
  json["wifi-psk"] = psk;
  write_json(json);
}


String PermStorageUtil::createDeviceId() {
  // Create random deviceId
  String deviceIdPostfix = "";
  for (int i=0; i < deviceIdLength; i++) {
    deviceIdPostfix += char(('0' + random(0, 10)));
  }
  return deviceIdPostfix;
}
