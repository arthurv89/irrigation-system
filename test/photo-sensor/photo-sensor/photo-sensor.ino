/**
 * See circuit here: https://www.arduino.cc/en/tutorial/AnalogInput
 * 
 */

int sensorPin = A0;    // select the input pin for the potentiometer

void setup() {
  Serial.begin(115200);
  Serial.println();
 
}

void loop() {
  int sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  delay(1000);
}
