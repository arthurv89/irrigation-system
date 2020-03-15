#ifndef DHTLIB_H
#define DHTLIB_H
/**
 * Defines the sensor API return codes
 */
enum dht_status {
	E_DHTLIB_OK = 0, //!< Read operation on DHT11 succesfull
	E_DHTLIB_TIMEOUT_ERROR, //!< Timeout error occured on DHT11 comunication
	E_DHTLIB_CHKSUM_ERROR, //!< Checksum verification error
};
/*-------------------------------------------------------------*/
/*		Function prototypes				*/
/*-------------------------------------------------------------*/

/**
 * @brief Prepares the DHT11 communications channel
 *
 * Prepares the communication with the DHT11 sensor, initializes IO ports and
 * any other required peripherals.
 */
void dhtlib_init();

/**
 * @brief Reads the current temperature and humidity from DHT11 sensor
 *
 * Reads the current temperature and humidity, returns an enumerated value
 * defining if the comunication with the sensor was succesfull. This function
 * places the temperature and humidity readings on the provided buffers.
 * 
 * @param pxTemperature Pointer to place the temperature reading
 * @param pxHumidity Pointer to place the humidity reading
 *
 * @return An enumerated value indictating the communication and device status
 */
enum dht_status dhtlib_read11(uint8_t * temp, uint8_t * hum);

/**
 * @brief Reads the current temperature and humidity from DHT22 sensor
 *
 * Reads the current temperature and humidity, returns an enumerated value
 * defining if the comunication with the sensor was succesfull. This function
 * puts the temperature and humidity readings on the provided buffers.
 *
 * @param pxTemperature Pointer to place the temperature reading
 * @param pxHumidity Pointer to place the humidity reading
 *
 * @return An enumerated value indictating the communication and device status
 */
enum dht_status dhtlib_read22(uint16_t * temp, uint16_t * hum);

/**
 * @brief Reads the current temperature and humidity from DHT22 sensor
 *
 * Reads the current temperature and humidity, returns an enumerated value
 * defining if the comunication with the sensor was succesfull. This function
 * puts the temperature and humidity readings on the provided buffers.
 *
 * @param temp Pointer to float to store the temperature reading
 * @param hum Pointer to float to store the humidity reading
 *
 * @return An enumerated value indictating the communication and device status
 */
enum dht_status dhtlib_float22(float * temp, float * hum);

#endif
// End of Header file
