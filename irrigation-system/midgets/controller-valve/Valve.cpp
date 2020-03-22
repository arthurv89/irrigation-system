#include "Valve.h"

#include "Arduino.h"
#include "Request.h"
#include "Utils.h"
#include "Internet.h"
#include "Midget.h"

const int builtin_on_value = LOW;
const int builtin_off_value = HIGH;

const long interval = 0;
const long blink_period = 5000;


void handle(IRunner* iRunner) {
  connect_wifi("CasaBatata", "nopassword");

  fetch_settings();
  StaticJsonDocument<200> settings = get_settings();
  String ip = settings["controller_addr"]["ip"];
  int port = settings["controller_addr"]["port"];
  String controller_addr = ip + ":" + port;
  String url = "http://" + controller_addr + "/api/valveInstructions";

  String valveInstructionsJson = do_get_request(url);

  StaticJsonDocument<200> instructions = deserializeJson(valveInstructionsJson)["response"];

  String str;
  serializeJson(instructions, str);
  Serial.println(str);

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

    setPin(pin, HIGH);
    Serial.print(" On");
    delay(period_ms);

    setPin(pin, LOW);
    Serial.print(" Off");
    Serial.println();
}

void setPin(int pin, int value) {
  digitalWrite(pin, value);

  int builtin_value = -1;
  if(value == HIGH) {
    builtin_value = builtin_on_value;
  } else {
    builtin_value = builtin_off_value;
  }
  digitalWrite(LED_BUILTIN, builtin_value);
}
