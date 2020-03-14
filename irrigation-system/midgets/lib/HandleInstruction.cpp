#include "HandleInstruction.h"

#include <ArduinoJson.h>

void handleDigitalWriteInstruction(uint8_t pinNo, int iVal) {
  int val = iVal == 1 ? HIGH : LOW;
  Serial.print("Set pin ");
  Serial.print(pinNo);
  Serial.print(" to ");
  Serial.println(val);
  pinMode(pinNo, OUTPUT);
  digitalWrite(pinNo, val);
}

void handleDelayInstruction(int ms) {
  Serial.print("Delay ");
  Serial.println(ms);
  delay(ms);
}


void handle(DynamicJsonDocument& jsonObj) {
  JsonArray instructions = jsonObj["instructions"].as<JsonArray>();
  for(auto instruction : instructions) {
    String funcName = instruction["function"].as<String>();
    JsonObject params = instruction["params"];
    handleInstruction(funcName, params);
  }
}


void handleInstruction(String funcName, JsonObject params) {
  Serial.println();
  Serial.println("Instruction: " + funcName);
  if(funcName == "dw") {
    handleDigitalWriteInstruction(
      params["pin"],
      params["value"]);
  }
  if(funcName == "d") {
    handleDelayInstruction(
      params["ms"]);
  }
}
