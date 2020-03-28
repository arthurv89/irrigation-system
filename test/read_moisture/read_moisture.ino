// Channels
const int moisture_sensor_in = A0;
const int loop_delay_time = 300;
const int p1 = D5;
const int p2 = D6;

int moisture_value;


void setup() {
  Serial.begin(115200);       // use the serial port to print the number
}

void loop() {
  Serial.println();
  Serial.println();
  moisture_value = get_moisture_value(moisture_sensor_in, p1, p2);
//  Serial.println(moisture_value);

  delay(loop_delay_time);
}


int get_moisture_value(int moisture_sensor_in, int pin1, int pin2) {
  int value = read_moisture_value(moisture_sensor_in, pin1, pin2);
  read_moisture_value(moisture_sensor_in, pin2, pin1); // Reverse polarity and throw the result away

  return value;
}

int read_moisture_value(int moisture_sensor_in, int ground_pin, int voltage_pin) {
    pinMode(moisture_sensor_in, INPUT);
    pinMode(ground_pin, OUTPUT);
    _digitalWrite(ground_pin, LOW);
    pinMode(voltage_pin, OUTPUT);
    _digitalWrite(voltage_pin, HIGH);
    delay(1000);

    int value = analogRead(moisture_sensor_in);
    Serial.println(value);

    pinMode(ground_pin, INPUT);
    pinMode(voltage_pin, INPUT);

    return value;
}
