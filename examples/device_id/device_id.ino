#include <veml3328.h>

#define SerialDebug Serial3

void setup() {
    SerialDebug.begin(115200);

    if (Veml3328.begin()) {
        SerialDebug.println("Error: could not start VEML3328 library");
    }
}

void loop() {
    SerialDebug.printf("Device ID: %x\r\n", Veml3328.deviceID());
    delay(2000);
}