#include "veml3328.h"
#include <Wire.h>

#define SerialDebug Serial3 // Cellular Mini
#define DEBUG

// #define DEBUG// #ifdef __AVR_AVR128DB48__ // MINI
// #define WIRE Wire1
// #define WIRE_MUX 2
// #else
// #ifdef __AVR_AVR128DB64__ // Non-Mini
// #define WIRE Wire1
// #define WIRE_MUX 2
// #else
// // #error "INCOMPATIBLE_DEVICE_SELECTED"
// #endif
// #endif

#define I2C_ADDRESS_DEFAULT 0x10
#define POINTER_CONFIG 0x00     // VEML3328 configuration register
#define POINTER_CLEAR 0x04      // Red channel pointer
#define POINTER_RED 0x05        // Blue channel pointer
#define POINTER_GREEN 0x06      // Green channel pointer
#define POINTER_BLUE 0x07       // Blue channel pointer
#define POINTER_IR 0x08         // Infrared (IR) channel pointer
#define POINTER_DEVICE_ID 0x0C  // Device ID = 0x28

/* Global variables */
uint8_t I2C_ADDRESS;

/* Singleton instance. Used by rest of library */
VEMLClass Veml3328 = VEMLClass::instance();

static uint8_t initalize(void) {
	WIRE.swap(2);
	WIRE.begin();

	if (Veml3328.regRead(POINTER_CONFIG) != ((1 << 15) & (1 << 0))) {
#ifdef DEBUG
		SerialDebug.println("Error: sensor config not at standard value");
#endif
		return 1;	
	}
    return 0;
}

/**
 * @brief Initialize VEML3328 library (no arguments)
 * 
 * @return int 0 if successful, 1 if failed
 */
uint8_t VEMLClass::begin(void) {
	I2C_ADDRESS = I2C_ADDRESS_DEFAULT;
	return initalize();
}

/**
 * @brief Initialize VEML3328 library with custom I2C address
 *
 * @param address Custom I2C address
 * @return int 0 if successful, -1 if failed
 */
uint8_t VEMLClass::begin(uint8_t address) {
	I2C_ADDRESS = address;
	return initalize();	
}

/**
 * @brief Wake up VEML3328 (low power mode)
 *
 * @return int 0 if successful, -1 if failed
 */
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

/**
 * @brief Shutdown VEML3328 (low power mode)
 *
 * @return int 0 if successful, 1 if failed
 */
uint8_t VEMLClass::shutdown(void) {
	/* Set shutdown bits SD1/SD0 */
	regWrite(POINTER_CONFIG, ((1 << 15) & (1 << 0)));

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

/**
 * @brief Get RED channel value
 * 
 * @return int16_t Channel value
 */
int16_t VEMLClass::getRed(void) {
    uint16_t reg = regRead(POINTER_RED);
	return reg;
}

/**
 * @brief Get GREEN channel value
 * 
 * @return int16_t Channel value
 */
int16_t VEMLClass::getGreen(void) {
    uint16_t reg = regRead(POINTER_GREEN);
	return reg;
}

/**
 * @brief Get BLUE channel value
 * 
 * @return int16_t Channel value
 */
int16_t VEMLClass::getBlue(void) {
    uint16_t reg = regRead(POINTER_BLUE);
	return reg;
}

/**
 * @brief Get INFRARED(IR) channel value
 * 
 * @return int16_t Channel value
 */
int16_t VEMLClass::getIR(void) {
    uint16_t reg = regRead(POINTER_IR);
	return reg;
}

/**
 * @brief Get CLEAR channel value
 * 
 * @return int16_t Channel value
 */
int16_t VEMLClass::getClear(void) {
    uint16_t reg = regRead(POINTER_CLEAR);
	return reg;
}

/**
 * @brief Return device ID from sensor
 * 
 * @return uint8_t device ID byte
 */
uint16_t VEMLClass::deviceID(void) {
    uint16_t reg = regRead(POINTER_DEVICE_ID);
    return (reg & 0xFF); // LSB data
}

/**
 * @brief Shutdown R and B channel on device
 * 
 * @return int 0 if successful, 1 if failed
 */
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

/**
 * @brief Wake up R and B channel on device
 * 
 * @return int 0 if successful, 1 if failed
 */
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

/**
 * @brief Set DG value of device
 * 
 * @param val DG value enum member
 * @return int 0 if successful, 1 if failed
 */
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

/**
 * @brief Set Gain value of device
 * 
 * @param val Gain value enum member
 * @return int 0 if successful, 1 if failed
 */
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

/**
 * @brief Set sensitivity of device
 * 
 * @param high_low_sens High sensitivity (false, default), low sensitivity (true) (1/3)
 * @return int 0 if successful, 1 if failed
 */
uint8_t VEMLClass::setSensitivity(bool high_low_sens) {
	/* Variables */
	uint16_t reg;

	switch (high_low_sens)
	{
	case false:
		regWrite(POINTER_CONFIG, (0 << 6));
		
		reg = regRead(POINTER_CONFIG);
		if (reg & (1 << 6)) {
	#ifdef DEBUG
			SerialDebug.println("Error: gain not set");
	#endif
			return 1;
		}
		break;
	
	case true:
		regWrite(POINTER_CONFIG, (1 << 6));
		
		reg = regRead(POINTER_CONFIG);
		if (reg & (0 << 6)) {
	#ifdef DEBUG
			SerialDebug.println("Error: gain not set");
	#endif
			return 1;
		}
		break;
	}
	
	return 0;
}

/**
 * @brief Set integration time of device
 * 
 * @param time Integration time enum member
 * @return int 0 if successful, 1 if failed
 */
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
void VEMLClass::regWrite(uint8_t reg_ptr, uint16_t data) {
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
uint16_t VEMLClass::regRead(uint8_t reg_ptr) {
	/* Variables */
	unsigned char rx_data[2] = {0};

	/* Start transaction, send command code */
	WIRE.beginTransmission(I2C_ADDRESS);
    WIRE.write(reg_ptr);

	/* Repeated start, request 2 bytes */
	WIRE.endTransmission(false);
	WIRE.requestFrom(I2C_ADDRESS, 2);

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