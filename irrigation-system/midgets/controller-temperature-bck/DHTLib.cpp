#include <stdint.h>
#include"Arduino.h"
#include"DHTLib.h"
/**
 * Buffer to store data from sensors
 */
unsigned char bits[5];

/*-------------------------------------------------------------*/
/*		Function prototypes	(this file only)	*/
/*-------------------------------------------------------------*/
static void dhtlib_start();


#ifndef DHTLIBPORT_H
#define DHTLIBPORT_H

#if defined(PLIB_PIC16)

#include <xc.h>
#include <stdint.h>
// Include project specific header file
#include "Config.h"
typedef uint8_t PORTBASE;
/** This macro configures the output pin for the DHT11 as an output */
/** This macro configures the input pin for the DHT22 as an output */
/** This macro sets the DHT11 to a logic high state */
/** This macro sets the DHT11 to a logic low state */
/** This macro reads the current state of the DHT11 interface pin */
/** This macro disables global interrupts while communicating with the DHT11 */
#define dhtlib_disint()   di()
/** This macro enables global interrupts after communication with the DHT11 */

#include <xc.h>
#include <stdint.h>
// Include project specific header file
#include "Config.h"
typedef uint8_t PORTBASE;
/** This macro configures the output pin for the DHT11 as an output */
/** This macro configures the input pin for the DHT22 as an output */
#define dhtlib_setin()    CONFIG_DHTLIB_TRIS = 1
/** This macro sets the DHT11 to a logic high state */
#define dhtlib_outhi()    {CONFIG_DHTLIB_OUTPIN = 1; CONFIG_DHTLIB_OUTPIN = 1;}
/** This macro sets the DHT11 to a logic low state */
/** This macro reads the current state of the DHT11 interface pin */
#define CONFIG_DHTLIB_PORTPIN()  CONFIG_DHTLIB_PORTPIN
/** This macro disables global interrupts while communicating with the DHT11 */
#define dhtlib_disint()   di()
/** This macro enables global interrupts after communication with the DHT11 */

#endif

#endif
// End of Header file


int CONFIG_DHTLIB_TRIS = 1;
int CONFIG_DHTLIB_PORTPIN = -1;
int CONFIG_DHTLIB_OUTPIN = -1;
void dhtlib_outlow() {
  CONFIG_DHTLIB_OUTPIN = 0; CONFIG_DHTLIB_OUTPIN = 0;
}
void dhtlib_outhi()    {CONFIG_DHTLIB_OUTPIN = 1; CONFIG_DHTLIB_OUTPIN = 1;}


/*-------------------------------------------------------------*/
/*		API Functions Implementation			*/
/*-------------------------------------------------------------*/
void dhtlib_init()
{
	// Initialization just sets DHT11 pin as input
	CONFIG_DHTLIB_TRIS = 1;
}

static enum dht_status dhtlib_read()
{
	unsigned char i = 0;
	unsigned char aindex = 0;
	unsigned char bcount = 7;

	// This is a variable used as timeout counter, type of this variable should
	// be chosen according to the processor speed.
	// When this variable overflows, timeout is detected
	uint8_t tocounter = 0;

	// Clear all bits on data reception buffer
	for (i = 0; i < 5; i++) bits[i] = 0;

	// Disable interrupts to keep timing accurate, especially on slow procesors

	// Generate MCU start signal
	dhtlib_start();

	// Wait for response from DHT11 max 80 uS low
	tocounter = 1;
	while (!CONFIG_DHTLIB_PORTPIN) {
		if (!tocounter++)
			goto timeout;
	}
	// Wait for response from DHT11 max 80 uS high
	tocounter = 1;
	while (CONFIG_DHTLIB_PORTPIN) {
		if (!tocounter++)
			goto timeout;
	}
	// Begin data reception, 40 bits to be received
	for (i = 0; i < 40; i++) {
		tocounter = 1;
		while (!CONFIG_DHTLIB_PORTPIN) {
			if (!tocounter++)
				goto timeout;
		}
		// If after 50 uS the pin is low we're on the start of another bit
		delayMicroseconds(40);
		if (!CONFIG_DHTLIB_PORTPIN) {
			if (bcount == 0) {
				bcount = 7;
				aindex++;
			} else {
				bcount--;
			}
			continue;
		}
		// If pin is high after 50 us we're receiving logic high
		tocounter = 1;
		while (CONFIG_DHTLIB_PORTPIN) {
			if (!tocounter++)
				goto timeout;
		}
		// Set the bit and shift left
		bits[aindex] |= (1 << bcount);
		if (bcount == 0) {
			bcount = 7;
			aindex++;
		} else {
			bcount--;
		}
	}
	// Exit for normal operation
	
	return E_DHTLIB_OK;
	// Exit when timeout occurs
timeout:
	return E_DHTLIB_TIMEOUT_ERROR;
}

enum dht_status dhtlib_read11(uint8_t * temp, uint8_t * hum)
{
	// Read operation
	enum dht_status s = dhtlib_read();
	if (s != E_DHTLIB_OK)
		return s;
	// Checksum comprobation
	unsigned char chksum = bits[0] + bits[1] + bits[2] + bits[3];
	if (chksum != bits[4])
		return E_DHTLIB_CHKSUM_ERROR;
	// Copy results
	* hum = bits[0];
	* temp = bits[2];
	// Return Ok code
	return E_DHTLIB_OK;
}

enum dht_status dhtlib_read22(uint16_t * temp, uint16_t * hum)
{
	// Read operation
	enum dht_status s = dhtlib_read();
	if (s != E_DHTLIB_OK)
		return s;
	// Checksum comprobation
	unsigned char chksum = bits[0] + bits[1] + bits[2] + bits[3];
	if (chksum != bits[4])
		return E_DHTLIB_CHKSUM_ERROR;
	// Copy results
	* hum = ((bits[0] << 8) + bits[1]) & 0x7FFF;
	* temp = (bits[2] << 8) + bits[3];
	if (* temp & 0x8000) {
		* temp = -((* temp) & 0x7FFF);
	}
	// Return Ok code
	return E_DHTLIB_OK;

}

enum dht_status dhtlib_float22(float * temp, float * hum)
{
	// Read operation
	enum dht_status s = dhtlib_read();
	if (s != E_DHTLIB_OK)
		return s;
	// Checksum comprobation
	unsigned char chksum = bits[0] + bits[1] + bits[2] + bits[3];
	if (chksum != bits[4])
		return E_DHTLIB_CHKSUM_ERROR;
	// Copy results
	* hum = 0.1 * (((bits[0] << 8) + bits[1]) & 0x7FFF);
	if (bits[2] & 0x80) {
		*temp = -0.1 * ((bits[2] << 8) + bits[3]);
	} else {
		*temp = 0.1 * ((bits[2] << 8) + bits[3]);
	}
	// Return Ok code
	return E_DHTLIB_OK;
}

/**
 * @brief Generates start signal on the bus
 *
 * This function generates the signal needed to start the comunication with the
 * tamperature and humidity sensor.
 */
static void dhtlib_start()
{
	CONFIG_DHTLIB_TRIS = 0;
	dhtlib_outlow(); // Pull bus to low state

	delay(20);

	dhtlib_outhi();
	CONFIG_DHTLIB_TRIS = 1;

	delayMicroseconds(60);
}
