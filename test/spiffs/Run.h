#include <Arduino.h>
#include <ArduinoJson.h>

void _setup();

String ser(StaticJsonDocument<2048> json);
StaticJsonDocument<2048> copy_json(StaticJsonDocument<2048> json);
StaticJsonDocument<2048> create_json();

String read_file(String path);
void write_file(String path, String contents);
void copy_value(String key, StaticJsonDocument<2048>& from_json, StaticJsonDocument<2048>& to_json);
