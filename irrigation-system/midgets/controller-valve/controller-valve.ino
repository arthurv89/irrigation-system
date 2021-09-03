/**
 * How to install:
 * Assuming relay is with the blue side on top, and you're facing the pins.
 *
 * [XXX, Common, +]
 * [__________]
 * |          |
 * | BLUE BOX |
 * [__________]
 *  |    |   |
 * [D1, 3V, GND]:
 *
 */

using namespace std;

#include "Midget.h"
#include "IRunner.h"
#include "Valve.h"

#include "LiquidCrystal_I2C.h"
#include <Wire.h>

const int pins = 3;
const int valvePins[pins] = {D5, D6, D7};

LiquidCrystal_I2C lcd(0x27, 16, 2);


class Runner: public IRunner
{
public:
  bool setup_wifi() {
    return false;
  }

  String getType() {
    return "valve";
  }

  int getButtonPin() {
    return D1;
  }

  String getWifiName(String deviceId) {
    return "IRSYS-V-" + deviceId;
  }

  void add_sensor_values(JsonObject &doc) {
    handle(this);
  }

  int getValvePin(int valve) {
    return valvePins[valve];
  }

  LiquidCrystal_I2C getLcd() {
    return lcd;
  }
};


void setup() {
  IRunner* runner = new Runner;
  setupMidget(runner);


  Wire.begin(D2, D1);
  lcd.begin();


  for(int p=0; p<pins; p++) {
    int pin = valvePins[p];
    pinMode(pin, OUTPUT);
  }
}

void loop() {
    loopMidget();
}
