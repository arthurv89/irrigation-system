#include "Arduino.h"
#include <ArduinoJson.h>

String to_str(long l);


DynamicJsonDocument _deserializeJson(String json);
String _serializeJson(StaticJsonDocument<200> doc);
StaticJsonDocument<200> emptyJsonObject();
