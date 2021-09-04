#include "Arduino.h"
#include <ArduinoJson.h>

String to_str(long l);


DynamicJsonDocument _deserializeJson(String json);
String _serializeJson(StaticJsonDocument<2048> doc);
StaticJsonDocument<2048> _emptyJsonObject();
