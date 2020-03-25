#include <Arduino.h>
#include <ArduinoJson.h>

void _setup();

String ser(StaticJsonDocument<200> json);
StaticJsonDocument<200> copy_json(StaticJsonDocument<200> json);
StaticJsonDocument<200> create_json();

String read_file(String path);
void write_file(String path, String contents);
void copy_value(String key, StaticJsonDocument<200>& from_json, StaticJsonDocument<200>& to_json);
