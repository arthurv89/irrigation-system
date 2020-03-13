#include"DHTLib.h"

void setup()
{
  Serial.begin(115200);
  dhtlib_init ()
}

void loop()
{
  uint8_t * temp, uint8_t * hum;
  dhtlib_read11(temp, hum);
  Serial.println(temp);
  Serial.println(hum);

  if (result == E_DHTLIB_OK) 
    sprintf (buf, "T:% 02d CH:% 02d %%", tb, hb); 
  else 
    sprintf (buf, "Sensor error:% 02d", result); 
}
