/*
 * 16
 * 5
 * 4
 * 0
 * 2
 * 14
 * 12
 * 13
 * 15
 * 3
 *
 */

//const int ledPins[3] = {5, 4, 0};
//const int ledPins[3] = {14, 12, 13};
const int ledPins[] = {D1};

const int builtin_on_value = LOW;
const int builtin_off_value = HIGH;

const long interval = 0;
const long blink_period = 5000;
const int times = 1;

void setup() {
 Serial.begin(115200);
}

void loop() {
  handle();
  delay(interval);
}

void handle() {
  for(int p=0; p<sizeof(ledPins); p++) {
    int pin = ledPins[p];
    pinMode(pin, OUTPUT);
    Serial.print(pin);
    for(int i=0; i<times; i++) {
      setPin(pin, HIGH);
      Serial.print(" On");
      delay(blink_period);

      setPin(pin, LOW);
      Serial.print(" Off");
      delay(blink_period);
    }
    Serial.println();
    pinMode(pin, INPUT);
  }
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
