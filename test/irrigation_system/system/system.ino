// Channels
const int moisture_sensor_in = 3;
const int relay_out = 7;

const int moisture_threshold = 450;
const int solenoid_open_time = 5000;
const int loop_delay_time = 5000;

int moisture_value;


void setup() {
  Serial.begin(9600);       // use the serial port to print the number
  pinMode(moisture_sensor_in, INPUT);
  pinMode(relay_out, OUTPUT);
}

void loop() {
  moisture_value = analogRead(moisture_sensor_in);
  Serial.println(moisture_value);

  if(moisture_value > moisture_threshold) {
    // It's wet: Open the Solenoid to give water.
      _digitalWrite(relay_out, HIGH);
      delay(solenoid_open_time);
      _digitalWrite(relay_out, LOW);
  }
  
  delay(loop_delay_time);
}
