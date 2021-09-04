#include "Utils.h"

String to_str(long l) {
  char mystr[40];
  sprintf(mystr, "%u", l);
  return mystr;
}


DynamicJsonDocument _deserializeJson(String json) {
  DynamicJsonDocument doc(4096);
  DeserializationError error = deserializeJson(doc, json);
  if (error) {
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println("Error during deserialisation");
    Serial.println(error.c_str());

    Serial.println();
    Serial.println("JSON:");
    Serial.println(json);

    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();
    Serial.println();

  }

  return doc;
}

String _serializeJson(StaticJsonDocument<2048> doc) {
  String str;
  serializeJson(doc, str);
  return str;
}

StaticJsonDocument<2048> _emptyJsonObject() {
  StaticJsonDocument<2048> doc;
  JsonObject obj = doc.createNestedObject("_");
  // JsonObject& object = doc.createObject();
  return doc;
}
