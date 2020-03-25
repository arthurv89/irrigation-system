#include "Run.h"
#include <FS.h>   // Include the SPIFFS library
#include <ArduinoJson.h>

String path = "/wifi.txt";

void _setup() {
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');

  SPIFFS.begin();

  StaticJsonDocument<200> json = create_json();
  String str = ser(json);
  write_file(path, str);
  String contents = read_file(path);
  Serial.println("contents");
  Serial.println(contents);

  StaticJsonDocument<200> new_json = copy_json(json);
  String new_str = ser(new_json);
  write_file(path, new_str);
  String new_contents = read_file(path);
  Serial.println("new_contents");
  Serial.println(new_contents );
}

String ser(StaticJsonDocument<200> json) {
  String str;
  serializeJson(json, str);
  return str;
}

void copy_value(String key, StaticJsonDocument<200>& from_json, StaticJsonDocument<200>& to_json) {
  // Serial.println("Copy " + key + "?");

  if(from_json.containsKey(key)) {
    // Serial.println("contains key " + key);
    to_json[key] = from_json[key];
  }
}

StaticJsonDocument<200> copy_json(StaticJsonDocument<200> json) {
  Serial.println("");
  Serial.println("Prev JSON");
  Serial.println(ser(json));

  StaticJsonDocument<200> new_json;

  Serial.println("");
  Serial.println("New JSON");
  copy_value("x", json, new_json);
  Serial.println(ser(new_json));

  copy_value("y", json, new_json);
  Serial.println(ser(new_json));

  copy_value("z", json, new_json);
  Serial.println(ser(new_json));
  Serial.println("");


//  Serial.println("z=" + z);
//  Serial.println("NULL " + to_str(z == NULL));
//  Serial.println("'NULL' " + to_str(z == "null"));
//  Serial.println();

//  Serial.println("json[z]=...");
//  Serial.println("NULL " + to_str(json["z"] == NULL));
//  Serial.println("'NULL' " + to_str(json["z"] == "null"));
//  Serial.println();

  return new_json;
}

String to_str(boolean b) {
  if(b) {
    return "TRUE";
  } else {
    return "FALSE";
  }
}

StaticJsonDocument<200> create_json() {
  StaticJsonDocument<200> json;
  String x = "a";
  int y = 1;
//  String z;
  json["x"] = x;
  json["y"] = y;
  return json;
}

String read_file(String path) {
  Serial.println("Reading file");
  String contents = "";
  File f = SPIFFS.open(path, "r");
  while (f.available()){
    contents += f.readStringUntil('\n');
  }
  f.close();
  return contents;
}

void write_file(String path, String contents) {
  Serial.println("Writing file");
  File f = SPIFFS.open(path, "w");
  int bytesWritten = f.print(contents);
  f.close();
}
