#ifndef VEML3328_H
#define VEML3328_H

#include <Arduino.h>

typedef enum 
{ 
	dg_x1 = 0x00, 
	dg_x2 = 0x01, 
	dg_x4 = 0x02 
} DG_t;

typedef enum 
{
	gain_x1_2 = 0x03,
	gain_x1 = 0x00,
	gain_x2 = 0x01,
	gain_x4 = 0x02
} gain_t;

typedef enum
{
	time_50 = 0x00,
	time_100 = 0x01,
	time_200 = 0x02,
	time_400 = 0x03
} int_time_t;

class VEMLClass {
    private:
    /**
     * @brief Constructor is hidden to enforce a single instance of this class
     * through a singleton.
     */
    VEMLClass(){};

    public:
    /* Singleton instance */
    static VEMLClass &instance(void) {
        static VEMLClass instance;
        return instance;
    }

    /**
     * @brief Initialize VEML3328 library
     *
     * @return int 0 if successful, 1 if failed
     */
    uint8_t begin(void);

    /**
     * @brief Initialize VEML3328 library with custom I2C address
     *
     * @param address Custom I2C address
     * @return int 0 if successful, 1 if failed
     */
    uint8_t begin(uint8_t address);

    /**
     * @brief Wake up VEML3328 (low power mode)
     *
     * @return int 0 if successful, 1 if failed
     */
    uint8_t wake(void);

    /**
     * @brief Shutdown VEML3328 (low power mode)
     *
     * @return int 0 if successful, 1 if failed
     */
    uint8_t shutdown(void);

    /**
     * @brief Get RED channel value
     *
     * @return int16_t Channel value
     */
    int16_t getRed(void);

    /**
     * @brief Get GREEN channel value
     *
     * @return int16_t Channel value
     */
    int16_t getGreen(void);

    /**
     * @brief Get BLUE channel value
     *
     * @return int16_t Channel value
     */
    int16_t getBlue(void);

    /**
     * @brief Get INFRARED(IR) channel value
     *
     * @return int16_t Channel value
     */
    int16_t getIR(void);

    /**
     * @brief Get CLEAR channel value
     *
     * @return int16_t Channel value
     */
    int16_t getClear(void);

    /**
     * @brief Return device ID from sensor
     *
     * @return uint8_t Device ID byte
     */
    uint16_t deviceID(void);

	/**
	 * @brief Shutdown R and B channel on device
	 * 
	 * @return int 0 if successful, 1 if failed
	 */
    uint8_t rbShutdown(void);

	/**
	 * @brief Wake up R and B channel on device
	 * 
	 * @return int 0 if successful, 1 if failed
	 */
	uint8_t rbWakeup(void);

	/**
	 * @brief Set DG value of device
	 * 
	 * @param val DG value enum member
	 * @return int 0 if successful, 1 if failed
	 */
    uint8_t setDG(DG_t val);

	/**
	 * @brief Set Gain value of device
	 * 
	 * @param val Gain value enum member
	 * @return int 0 if successful, 1 if failed
	 */
	uint8_t setGain(gain_t val);

	/**
	 * @brief Set sensitivity of device
	 * 
	 * @param high_low_sens High sensitivity (false, default), low sensitivity (true) (1/3)
	 * @return int 0 if successful, 1 if failed
	 */
	uint8_t setSensitivity(bool high_low_sens);

	/**
	 * @brief Set integration time of device
	 * 
	 * @param time Integration time enum member
	 * @return int 0 if successful, 1 if failed
	 */
    uint8_t setIntTime(int_time_t time);
};
    extern VEMLClass Veml3328;

#endif