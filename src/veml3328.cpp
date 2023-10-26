#include "veml3328.h"

#define DEFAULT_ADDRESS (0x10)

#define CONFIG_SHUTDOWN_bm          (0b1000000000000001)
#define CONFIG_SHUTDOWN_RB_bm       (0b0100000000000000)
#define CONFIG_DIFFERENTIAL_GAIN_bm (0b0011000000000000)
#define CONFIG_DIFFERENTIAL_GAIN_bp (12)
#define CONFIG_GAIN_bm              (0b0000110000000000)
#define CONFIG_GAIN_bp              (10)
#define CONFIG_SENSITIVITY_bm       (0b0000000001000000)
#define CONFIG_SENSITIVITY_bp       (6)
#define CONFIG_INTEGRATION_TIME_bm  (0b0000000000110000)
#define CONFIG_INTEGRATION_TIME_bp  (4)

#define CONFIG_REGISTER_ADDRESS    (0x00)
#define CLEAR_REGISTER_ADDRESS     (0x04)
#define RED_REGISTER_ADDRESS       (0x05)
#define GREEN_REGISTER_ADDRESS     (0x06)
#define BLUE_REGISTER_ADDRESS      (0x07)
#define IR_REGISTER_ADDRESS        (0x08)
#define DEVICE_ID_REGISTER_ADDRESS (0x0C)

VEMLClass Veml3328 = VEMLClass::instance();

uint8_t VEMLClass::begin(const uint8_t address,
                         TwoWire *wire_,
                         const uint8_t pin_swap) {
    this->wire = wire_;
    this->device_address = address;

    this->wire->swap(pin_swap);
    this->wire->begin();

    return wake();
}

uint8_t VEMLClass::begin(TwoWire *wire, const uint8_t pin_swap) {
    return begin(DEFAULT_ADDRESS, wire, pin_swap);
}

uint16_t VEMLClass::deviceID() {
    return (read(DEVICE_ID_REGISTER_ADDRESS) & 0xFF);
}

uint16_t VEMLClass::getRed() { return read(RED_REGISTER_ADDRESS); }
uint16_t VEMLClass::getGreen() { return read(GREEN_REGISTER_ADDRESS); }
uint16_t VEMLClass::getBlue() { return read(BLUE_REGISTER_ADDRESS); }
uint16_t VEMLClass::getIR() { return read(IR_REGISTER_ADDRESS); }
uint16_t VEMLClass::getClear() { return read(CLEAR_REGISTER_ADDRESS); }

uint8_t VEMLClass::wake() {
    return writeConfirm(CONFIG_REGISTER_ADDRESS, CONFIG_SHUTDOWN_bm, 0x0);
}

void VEMLClass::shutdown() {
    write(CONFIG_REGISTER_ADDRESS, CONFIG_SHUTDOWN_bm, CONFIG_SHUTDOWN_bm);
}

uint8_t VEMLClass::rbShutdown() {
    return writeConfirm(
        CONFIG_REGISTER_ADDRESS, CONFIG_SHUTDOWN_RB_bm, CONFIG_SHUTDOWN_RB_bm);
}

uint8_t VEMLClass::rbWakeup() {
    return writeConfirm(CONFIG_REGISTER_ADDRESS, CONFIG_SHUTDOWN_RB_bm, 0x0);
}

uint8_t VEMLClass::setDG(const DG_t differential_gain) {
    return writeConfirm(CONFIG_REGISTER_ADDRESS,
                        CONFIG_DIFFERENTIAL_GAIN_bm,
                        differential_gain,
                        CONFIG_DIFFERENTIAL_GAIN_bp);
}

uint8_t VEMLClass::setGain(const gain_t gain) {
    return writeConfirm(
        CONFIG_REGISTER_ADDRESS, CONFIG_GAIN_bm, gain, CONFIG_GAIN_bp);
}

uint8_t VEMLClass::setSensitivity(const bool sensitivity) {
    return writeConfirm(CONFIG_REGISTER_ADDRESS,
                        CONFIG_SENSITIVITY_bm,
                        sensitivity ? 1 : 0,
                        CONFIG_SENSITIVITY_bp);
}

uint8_t VEMLClass::setIntTime(const int_time_t integration_time) {
    return writeConfirm(CONFIG_REGISTER_ADDRESS,
                        CONFIG_INTEGRATION_TIME_bm,
                        integration_time,
                        CONFIG_INTEGRATION_TIME_bp);
}

uint16_t VEMLClass::read(const uint8_t register_address) {

    if (wire == nullptr) {
        return 0;
    }

    unsigned char data[2] = {0};

    wire->beginTransmission(device_address);
    wire->write(register_address);
    wire->endTransmission(false);
    wire->requestFrom(device_address, 2);

    for (uint8_t i = 0; i < 2; i++) {
        // Wait some for the result
        delay(10);

        // We don't busy wait here to prevent dead lock. We rather want a wrong
        // result here than the device hanging here if we use a loop to check
        // the result here
        if (!wire->available()) {
            break;
        }

        data[i] = wire->read();
    }

    return (((uint16_t)data[1]) << 8) | data[0];
}

void VEMLClass::write(const uint8_t register_address,
                      const uint16_t mask,
                      const uint16_t data,
                      const uint8_t shift) {

    if (wire == nullptr) {
        return;
    }

    const uint16_t current_data = read(register_address);
    const uint16_t data_to_write = (current_data & (~mask)) | (data << shift);

    wire->beginTransmission(device_address);

    wire->write(register_address);
    wire->write(data_to_write & 0xff);
    wire->write(data_to_write >> 8);

    wire->endTransmission(true);
}

uint8_t VEMLClass::writeConfirm(const uint8_t register_address,
                                const uint16_t mask,
                                const uint16_t data,
                                const uint8_t shift) {

    write(register_address, mask, data, shift);

    return (((read(register_address) & mask) >> shift) != data) ? 1 : 0;
}