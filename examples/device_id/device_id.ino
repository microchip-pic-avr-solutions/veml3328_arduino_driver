#include <veml3328.h>

#define SerialDebug Serial3

int8_t err = 0;
uint16_t device_id = 0;

void setup(void) {
    /* Initialize serial interface */
    SerialDebug.begin(115200);

    /* Initialize VEML3328 library */
    err = Veml3328.begin();
    if (err) {
        SerialDebug.println("Error: could not start VEML3328 library");
    }
}

void loop(void) {
    device_id = Veml3328.deviceID();
    SerialDebug.printf("Device ID: %x\n", device_id);

    delay(1000);
}