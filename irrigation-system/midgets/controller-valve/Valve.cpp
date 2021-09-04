#include "Valve.h"

#include "Arduino.h"
#include "Request.h"
#include "Utils.h"
#include "Internet.h"
#include "Midget.h"
#include "Pin.h"
#include "StorageUtil.h"
#include "LiquidCrystal_I2C.h"
#include <Wire.h>

const int builtin_on_value = LOW;
const int builtin_off_value = HIGH;

const long interval = 0;
const long blink_period = 5000;

IRunner* _iRunner;

void setLcdText(String line1, String line2) {
  Serial.println("Printing to LCD: | " + line1 + " | " + line2 + " | ");
  Wire.begin(D2, D1);

  LiquidCrystal_I2C* lcd = _iRunner->getLcd();
  lcd->begin();
  lcd->home();
  lcd->print(line1);
  lcd->setCursor(0,1) ;
  lcd->print(line2);
}

void setLcdText(String line1) {
  setLcdText(line1, "");
}

void handle(IRunner* iRunner) {
  _iRunner = iRunner;

  StaticJsonDocument<2048> response = get_response();
  // setLcdText("Disconnecting Wifi");
  // disconnectWifi();

  execute_instructions(response);

  // setLcdText("Connecting Wifi");
  // connect_wifi();
}


StaticJsonDocument<2048> get_response() {
  fetch_settings();
  StaticJsonDocument<2048> settings = get_settings();
  String ip = settings["controller_addr"]["ip"];
  int port = settings["controller_addr"]["port"];
  String controller_addr = ip + ":" + port;
  String url = "http://" + controller_addr + "/api/valveInstructions?deviceId=" + getDeviceId();

  String valveInstructionsJson = do_get_request(url);
  Serial.println("valveInstructionsJson");
  Serial.println(valveInstructionsJson);

  StaticJsonDocument<2048> response = _deserializeJson(valveInstructionsJson)["response"];

  String deserSerJson = _serializeJson(response);
  Serial.println("deserSerJson");
  Serial.println(deserSerJson);

  return response;
}

void execute_instructions(StaticJsonDocument<2048> response){
  JsonArray instructions = response.as<JsonArray>();;
  setLcdText("Executing " + String(instructions.size()), "instructions");
  for (JsonArray openInstruction : instructions) {
    handleInstruction(openInstruction);
  }
  setLcdText("Finished exec", String(instructions.size()) + " instructions");
  Serial.println("Done executing instructions");
}

void handleInstruction(JsonArray instruction) {
  int valve = instruction[0];
  float period_seconds = instruction[1];
  int period_ms = period_seconds * 1000;
  int _pin = _iRunner->getValvePin(valve);

  setLcdText("V" + String(valve) + " P" + String(_pin) + " ON", String(period_ms) + " ms)");

  setPin(_pin, HIGH);
  delay(period_ms);

  setPin(_pin, LOW);
  setLcdText("V" + String(valve) + " P" + String(_pin), " OFF");
}

void setPin(int pin, int value) {
  _digitalWrite(pin, value);

  int builtin_value = -1;
  if(value == HIGH) {
    builtin_value = builtin_on_value;
  } else {
    builtin_value = builtin_off_value;
  }
  _digitalWrite(LED_BUILTIN, builtin_value);
}
