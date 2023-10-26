#ifndef VEML3328_H
#define VEML3328_H

#include <Arduino.h>
#include <Wire.h>

typedef enum { dg_x1 = 0x00, dg_x2 = 0x01, dg_x4 = 0x02 } DG_t;

typedef enum {
    gain_x1_2 = 0x03,
    gain_x1 = 0x00,
    gain_x2 = 0x01,
    gain_x4 = 0x02
} gain_t;

typedef enum {
    time_50 = 0x00,
    time_100 = 0x01,
    time_200 = 0x02,
    time_400 = 0x03
} int_time_t;

class VEMLClass {

  public:
    /* Singleton instance */
    static VEMLClass &instance() {
        static VEMLClass instance;
        return instance;
    }

    /**
     * @brief Initialize VEML3328 library with defined address and optional wire
     * instance and pin swap.
     *
     * @param address [in] Address of the VEML3328.
     * @param wire [in] The wire to communicate with the VEML3328.
     * @param pin_swap [in] Which pin swap/multiplexing to use on the @p wire.
     *
     * @return 0 if successful, 1 if error occurred.
     */
    uint8_t begin(const uint8_t address,
                  TwoWire *wire = &Wire,
                  const uint8_t pin_swap = 2);

    /**
     * @see #begin(address, wire, pin_swap)
     */
    uint8_t begin(TwoWire *wire = &Wire, const uint8_t pin_swap = 2);

    /**
     * @return The sensor's device ID.
     */
    uint16_t deviceID();

    /**
     * @return Red channel value.
     */
    uint16_t getRed();

    /**
     * @return Green channel value.
     */
    uint16_t getGreen();

    /**
     * @return Blue channel value.
     */
    uint16_t getBlue();

    /**
     * @return Infrared channel value.
     */
    uint16_t getIR();

    /**
     * @return Clear channel value.
     */
    uint16_t getClear();

    /**
     * @brief Wake the VEML3328 from low power mode.
     *
     * @return 0 if successful, 1 if failed.
     */
    uint8_t wake();

    /**
     * @brief Shutdown the VEML3328 (enter low power mode)
     */
    void shutdown();

    /**
     * @brief Shutdown R and B channel.
     *
     * @return 0 if successful, 1 if failed.
     */
    uint8_t rbShutdown();

    /**
     * @brief Wake up R and B channel.
     *
     * @return 0 if successful, 1 if failed
     */
    uint8_t rbWakeup();

    /**
     * @brief Sets the differential gain, given by the #DG_t enumeration.
     *
     * @return 0 if successful, 1 if failed
     */
    uint8_t setDG(const DG_t differential_gain);

    /**
     * @brief Sets the gain, given by the #gain_t enumeration.
     *
     * @return 0 if successful, 1 if failed
     */
    uint8_t setGain(const gain_t gain);

    /**
     * @brief Sets the sensitivity.
     *
     * @param sensitivity High sensitivity (false, default), low sensitivity
     * (true)
     * @return 0 if successful, 1 if failed
     */
    uint8_t setSensitivity(const bool sensitivity);

    /**
     * @brief Sets integration time, given by the #int_time_t enumeration.
     *
     * @return 0 if successful, 1 if failed
     */
    uint8_t setIntTime(const int_time_t integration_time);

  private:
    /**
     * @brief Physical interface for the driver. Is swappable through begin().
     */
    TwoWire *wire = nullptr;

    /**
     * @brief The I2C address of the VEML3328 device.
     */
    uint8_t device_address;

    /**
     * @brief Constructor is hidden to enforce a single instance of this class
     * through a singleton.
     */
    VEMLClass(){};

    /**
     * @brief Reads a register in the VEML3328.
     *
     * @param register_address [in] The register address.
     *
     * @return The data of the register.
     */
    uint16_t read(const uint8_t register_address);

    /**
     * @brief Writes @p data masked with @p mask to @p register_address. Given a
     * mask of e.g. 0000 0011 and data of 0000 0010, the register will have the
     * value of xxxx xx10 after the write procedure.
     *
     * @param register_address [in] Register address.
     * @param mask [in] Which bits to set.
     * @param data [in] Data to write.
     * @param shift [in] Optional left shift of @p data.
     */
    void write(const uint8_t register_address,
               const uint16_t mask,
               const uint16_t data,
               const uint8_t shift = 0);

    /**
     * @brief See #write(register_address, mask, data). This function performs a
     * write followed by a read to confirm the write operation.
     *
     * @return 0 on success, 1 on failure (the value read is not equal to the
     * one written).
     */
    uint8_t writeConfirm(const uint8_t register_address,
                         const uint16_t mask,
                         const uint16_t data,
                         const uint8_t shift = 0);
};

extern VEMLClass Veml3328;

#endif