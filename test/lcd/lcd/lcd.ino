/**
 * How to install:
 * SDA -> D2
 * SDL -> D1
 *
 * Make sure the libraries LiquidCrystal_IC2.h/cpp are in the project.
 * If not, add them through Sketch -> Add File
 */

#include "LiquidCrystal_I2C.h"
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
}

int i = 1;
void loop() { // do nothing here
  //Use predefined PINS consts
  Wire.begin(D2, D1);

  lcd.begin();

  Serial.println("Loop " + String(i%10));
  lcd.home();
  lcd.print("Hello, NodeMCU " + String(i%10));
  i++;
  delay(1000);
}
