#include "Arduino.h"
#include "TemperatureSensor.h"
#include"DHTLib.h"

// const int temperature_sensor_in = A0;

int get_temperature_value() {
    // pinMode(in_pin, INPUT);

    uint8_t * temp, uint8_t * hum;
    dhtlib_read11(temp, hum);
    Serial.println(temp);
    Serial.println(hum);

    if (result == E_DHTLIB_OK)
      sprintf (buf, "T:% 02d CH:% 02d %%", tb, hb);
    else
      sprintf (buf, "Sensor error:% 02d", result);

    return value;
}
