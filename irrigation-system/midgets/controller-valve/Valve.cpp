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

LiquidCrystal_I2C* _lcd;

void setLcdText(String text) {
  Serial.println("Printing to LCD");
  Wire.begin(D2, D1);

  _lcd->begin();
  _lcd->home();
  _lcd->print(text);
  delay(1000);
}

void handle(IRunner* iRunner) {
  _lcd = iRunner->getLcd();

  setLcdText("Hello, Arthur!");

  StaticJsonDocument<200> instructions = get_instructions();
  setLcdText("Disconnecting Wifi");
  disconnectWifi();

  setLcdText("Disconnecting Wifi");
  execute_instructions(instructions, iRunner);

  setLcdText("Connecting to Wifi");
  connect_wifi();
}


StaticJsonDocument<200> get_instructions() {
  fetch_settings();
  StaticJsonDocument<200> settings = get_settings();
  String ip = settings["controller_addr"]["ip"];
  int port = settings["controller_addr"]["port"];
  String controller_addr = ip + ":" + port;
  String url = "http://" + controller_addr + "/api/valveInstructions?deviceId=" + getDeviceId();

  String valveInstructionsJson = do_get_request(url);

  StaticJsonDocument<200> instructions = _deserializeJson(valveInstructionsJson)["response"];

  String str;
  serializeJson(instructions, str);
  Serial.println(str);

  return instructions;
}

void execute_instructions(StaticJsonDocument<200> instructions, IRunner* iRunner){
  Serial.println("Instructions");
  JsonArray open = instructions["open"];
  for (JsonVariant value : open) {
    JsonObject obj = value.as<JsonObject>();
    int valve = obj["valve"];
    int pin = iRunner->getValvePin(valve);
    int period_ms = obj["time"];
    handleInstruction(valve, pin, period_ms);
  }
}

void handleInstruction(int valve, int pin, int period_ms) {
  Serial.print("Valve " + String(valve));
  Serial.print(" Pin " + String(pin));
  Serial.println(" On");
  setLcdText("V " + String(valve) + " P " + String(pin) + " ON");

  setPin(pin, HIGH);
  Serial.println("Delay " + String(period_ms) + " ms");
  setLcdText("Delay " + String(period_ms) + " ms");
  delay(period_ms);

  setPin(pin, LOW);
  setLcdText("V " + String(valve) + " P " + String(pin) + " OFF");
  Serial.println(" Off");
  Serial.println();
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
