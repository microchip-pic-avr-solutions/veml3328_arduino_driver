#include "veml3328.h"
#include <Wire.h>

#define WIRE Wire
#define SerialDebug Serial3 // Cellular Mini
// #define DEBUG

#define I2C_ADDRESS_DEFAULT 0x10
#define POINTER_CONFIG 0x00     // VEML3328 configuration register
#define POINTER_CLEAR 0x04      // Red channel pointer
#define POINTER_RED 0x05        // Blue channel pointer
#define POINTER_GREEN 0x06      // Green channel pointer
#define POINTER_BLUE 0x07       // Blue channel pointer
#define POINTER_IR 0x08         // Infrared (IR) channel pointer
#define POINTER_DEVICE_ID 0x0C  // Device ID = 0x28

/* Global variables */
static uint8_t I2C_ADDRESS;

/* Forward function delcarations */
void regWrite(uint8_t reg_ptr, uint16_t data);
uint16_t regRead(uint8_t reg_ptr);

/* Singleton instance. Used by rest of library */
VEMLClass Veml3328 = VEMLClass::instance();

static uint8_t initalize(void) {
	WIRE.swap(2);
	WIRE.begin();

	if (regRead(POINTER_CONFIG) != (uint16_t) ((1 << 15) | (1 << 0))) {
#ifdef DEBUG
		SerialDebug.println("Error: sensor config not at standard value");
#endif
		return 1;	
	}
	
	/* Wake up device */
	return Veml3328.wake();
}

uint8_t VEMLClass::begin(void) {
	I2C_ADDRESS = I2C_ADDRESS_DEFAULT;
	return initalize();
}

uint8_t VEMLClass::begin(uint8_t address) {
	I2C_ADDRESS = address;
	return initalize();	
}

uint8_t VEMLClass::wake(void) {
	regWrite(POINTER_CONFIG, ((0 << 15) & (0 << 0))); // Set shutdown bits SD1/SD0
	
	/* Check shutdown bits */
	uint16_t reg = regRead(POINTER_CONFIG);
	if ((reg & (1 << 15)) && (reg & (1 << 0))) {
#ifdef DEBUG
        SerialDebug.println("Error: shutdown bits set");
#endif
		return 1;
	}
	
	return 0;
}

uint8_t VEMLClass::shutdown(void) {
	/* Set shutdown bits SD1/SD0 */
	regWrite(POINTER_CONFIG, ((1 << 15) | (1 << 0)));

	/* Check shutdown bits */
	uint16_t reg = regRead(POINTER_CONFIG);
	if (!((reg & (1 << 15)) && (reg & (1 << 0)))) {
#ifdef DEBUG
        SerialDebug.println("Error: shutdown bits not set");
#endif
		return 1;
	}

	return 0;
}

int16_t VEMLClass::getRed(void) {
    return regRead(POINTER_RED);
}

int16_t VEMLClass::getGreen(void) {
    return regRead(POINTER_GREEN);
}

int16_t VEMLClass::getBlue(void) {
    return regRead(POINTER_BLUE);
}

int16_t VEMLClass::getIR(void) {
    return regRead(POINTER_IR);
}

int16_t VEMLClass::getClear(void) {
    return regRead(POINTER_CLEAR);
}

uint16_t VEMLClass::deviceID(void) {
	return (regRead(POINTER_DEVICE_ID) & 0xFF); // LSB data
}

uint8_t VEMLClass::rbShutdown(void) {
	regWrite(POINTER_CONFIG, (1 << 14)); 
	
	/* Check shutdown bit */
	uint16_t reg = regRead(POINTER_CONFIG);
	if (!(reg & (1 << 14))) {
#ifdef DEBUG
        SerialDebug.println("Error: shutdown bit not set");
#endif
		return 1;
	}
	
	return 0;
}

uint8_t VEMLClass::rbWakeup(void) {
	regWrite(POINTER_CONFIG, (0 << 14)); 
	
	/* Check shutdown bit */
	uint16_t reg = regRead(POINTER_CONFIG);
	if (reg & (1 << 14)) {
#ifdef DEBUG
        SerialDebug.println("Error: shutdown bit set");
#endif
		return 1;
	}
	
	return 0;
}

uint8_t VEMLClass::setDG(DG_t val) {
	regWrite(POINTER_CONFIG, val);

	/* Check user val */
	uint16_t reg = regRead(POINTER_CONFIG);
	if (!(reg & val)) {
#ifdef DEBUG
        SerialDebug.println("Error: DG not set");
#endif
		return 1;
	}
	
	return 0;
}

uint8_t VEMLClass::setGain(gain_t val) {
	regWrite(POINTER_CONFIG, val);

	/* Check user val */
	uint16_t reg = regRead(POINTER_CONFIG);
	if (!(reg & val)) {
#ifdef DEBUG
        SerialDebug.println("Error: gain not set");
#endif
		return 1;
	}
	
	return 0;
}

uint8_t VEMLClass::setSensitivity(bool high_low_sens) {
	/* Variables */
	uint16_t reg;

	if (high_low_sens) {
		regWrite(POINTER_CONFIG, (1 << 6));
		
		reg = regRead(POINTER_CONFIG);
		if (reg & (0 << 6)) {
	#ifdef DEBUG
			SerialDebug.println("Error: gain not set");
	#endif
			return 1;
		}
	} else {
		regWrite(POINTER_CONFIG, (0 << 6));
		
		reg = regRead(POINTER_CONFIG);
		if (reg & (1 << 6)) {
	#ifdef DEBUG
			SerialDebug.println("Error: gain not set");
	#endif
			return 1;
		}
	}
	return 0;
}

uint8_t VEMLClass::setIntTime(int_time_t time) {
	regWrite(POINTER_CONFIG, time);

	/* Check user val */
	uint16_t reg = regRead(POINTER_CONFIG);
	if (!(reg & time)) {
#ifdef DEBUG
        SerialDebug.println("Error: gain not set");
#endif
		return 1;
	}
	
	return 0;
}

/**
 * @brief 16-bit write procedure
 *
 * @param reg_ptr Register pointer
 * @param data 16-bit data
 */
void regWrite(uint8_t reg_ptr, uint16_t data) {
	/* Start transaction */
    WIRE.beginTransmission(I2C_ADDRESS);

	WIRE.write(reg_ptr);   // Register pointer
	WIRE.write(data & 0xFF); // LSB data
	WIRE.write(data >> 8); // MSB data

	WIRE.endTransmission(true);
}

/**
 * @brief 16-bit read procedure
 *
 * @param reg_ptr Register pointer
 * @return uint16_t Returned data
 */
uint16_t regRead(uint8_t reg_ptr) {
	/* Variables */
	unsigned char rx_data[2] = {0};

	/* Start transaction, send command code */
	WIRE.beginTransmission(I2C_ADDRESS);
    WIRE.write(reg_ptr);

	/* Repeated start, request 2 bytes */
	WIRE.endTransmission(false);
	WIRE.requestFrom((uint8_t) I2C_ADDRESS, (size_t) 2);

	/* Read available data */
	int i = 0;
	if (WIRE.available()) {
		while (i < 2)
		{
			rx_data[i] = WIRE.read();
			i++;
		}
	}

	return (rx_data[1] << 8) | rx_data[0];
}
