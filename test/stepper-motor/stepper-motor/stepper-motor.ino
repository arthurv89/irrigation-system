#include <Stepper.h>

const int stepsPerRevolution = 200;

Stepper myStepper = Stepper(stepsPerRevolution, D1, D2, D3, D4);

void setup() {
  Serial.begin(115200);
  myStepper.setSpeed(80);
}

void loop() {
  myStepper.step(stepsPerRevolution);
}
