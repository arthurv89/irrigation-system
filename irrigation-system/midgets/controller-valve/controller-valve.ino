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

const int pins = 3;
const int pinNos[pins] = {D1, D2, D3};

const int builtin_on_value = LOW;
const int builtin_off_value = HIGH;

const long interval = 0;
const long blink_period = 5000;
const int times = 1;

void setup() {
  Serial.begin(115200);
  Serial.println();

  for(int p=0; p<pins; p++) {
    int pin = pinNos[p];
    pinMode(pin, OUTPUT);
  }
}

void loop() {

  handle();
  delay(interval);
}

void handle() {
  for(int p=0; p<pins; p++) {
    Serial.println();
    int pin = pinNos[p];
    Serial.print("Index " + String(p));
    Serial.print(" Pin " + String(pin));
    for(int i=0; i<times; i++) {
      setPin(pin, HIGH);
      Serial.print(" On");
      delay(blink_period);

      setPin(pin, LOW);
      Serial.print(" Off");
      delay(blink_period);
    }
  }
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
