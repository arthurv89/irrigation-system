#include "Utils.h"

String to_str(long l) {
  char mystr[40];
  sprintf(mystr, "%u", l);
  return mystr;
}


DynamicJsonDocument deserializeJson(String json) {
  DynamicJsonDocument doc(1024);
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
