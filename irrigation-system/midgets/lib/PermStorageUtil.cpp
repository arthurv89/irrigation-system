#include <Arduino.h>
#include "PermStorageUtil.h"
#include "FS.h"
#include "Utils.h"

const bool isDebug = false;

const String path = "/data.txt";
const int deviceIdLength = 20;

void PermStorageUtil::initializePermStorage(bool resetPermStorage) {
  SPIFFS.begin();
}

String PermStorageUtil::read_json() {
  String contents = "";

  if (SPIFFS.exists(path)) {
    File f = SPIFFS.open(path, "r");
    while (f.available()){
      contents += f.readStringUntil('\n');
    }
    f.close();

    Serial.println("Read SPIFFS contents");
    Serial.println(contents);

    return contents;
  } else {
    Serial.println("No file yet. Pretend we just read an empty JSON file.");
    return "";
  }
}

void PermStorageUtil::write_json(StaticJsonDocument<200> doc) {
  String contents = _serializeJson(doc);

  Serial.println("Writing JSON file");
  Serial.println(contents);


  File f = SPIFFS.open(path, "w");
  f.print(contents);
  f.close();
}
