/**
 * Instructions:
 * See image int his folder to understand how to put the pins.
 */
#include "PermStorageUtil.h"
#include "PermStorage.h"

String version = "1.0";

int pin = D2;

int buttonPressed;

int delay_ms = 100;

String str = "";

void setup() {
  Serial.begin(115200);
  PermStorage.begin(512);

//  Serial.println("");
//  Serial.println("==== STARTING DEVICE =====");
//  Serial.println("");
  inializePermStorage(false);

  pinMode(pin, INPUT);
  buttonPressed = digitalRead(pin);

  Serial.println("");
  Serial.println("");
  Serial.println("");
  Serial.println("======= Finished Setup =======");

  run();
}

void run() {

  str += version + " " + "PIN " + String(pin) + " = " + buttonPressed + ", cycle=" + getCycle();

  if(buttonPressed == 1) {
    handle_button_pressed();
  } else if(getCycle() >= 5) {
    setCycle(0);
    do_big_calculation();
  } else {
    no_button_press();
  }
}

void do_big_calculation() {
  Serial.println("--------------------> Submit data over wifi");
  sleep(100);
}

void handle_button_pressed() {
  // Reboot and handle it once it's rebooted.
  Serial.println("Setup wifi");
  sleep(100);
}

void no_button_press() {
  // Just wait for the next cycle to see if a button is pressed
  sleep(delay_ms * 1000);
}

void sleep(int us) {
  Serial.println(str);
  setCycle(getCycle() + 1);
  ESP.deepSleep(us, WAKE_RF_DEFAULT);
}

void loop() {
//    run();
//    delay(delay_ms);
}
