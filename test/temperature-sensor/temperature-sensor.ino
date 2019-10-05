// Loop variables
long long previousMillis = 0;
const int interval = 300000; // Should be 900000 (15 minutes)

// Channels
const int sensor_in = D0;


/*********************************************
                     Setup
 *********************************************/

void setup() {
  Serial.begin(115200);
  
  Serial.println();
  Serial.println("[Setup] Starting up");
}

void setup_temperature_sensor() {
  pinMode(sensor_in, INPUT);
}





void perform_action() {
  Serial.println();
  Serial.println("==========");
  setup_temperature_sensor();
  int temperature_value = get_temperature_value();
  Serial.println(temperature_value);
}

int get_temperature_value() {
  int temperature_value = digitalRead(sensor_in);
  Serial.print("Temperature value: ");
  Serial.println(temperature_value);
  return temperature_value;
}


void loop() {
  perform_action();
  delay(1000);
}
