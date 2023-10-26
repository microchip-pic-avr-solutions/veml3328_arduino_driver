#include <veml3328.h>

#define SerialDebug Serial3

void setup() {
    SerialDebug.begin(115200);

    if (Veml3328.begin()) {
        SerialDebug.println("Error: could not start VEML3328 library");
    }
}

void loop() {

    SerialDebug.println();
    SerialDebug.printf("Red: %u\r\n", Veml3328.getRed());
    SerialDebug.printf("Green: %u\r\n", Veml3328.getGreen());
    SerialDebug.printf("Blue: %u\r\n", Veml3328.getBlue());
    SerialDebug.printf("IR: %u\r\n", Veml3328.getIR());
    SerialDebug.printf("Clear: %u\r\n\r\n", Veml3328.getClear());

    delay(2000);
}