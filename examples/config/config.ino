#include <veml3328.h>

#define SerialDebug Serial3

void setup() {
    SerialDebug.begin(115200);

    if (Veml3328.begin()) {
        SerialDebug.println("Error: could not start VEML3328 library");
    }

    // Set differential gain value
    if (Veml3328.setDG(dg_x2)) {
        SerialDebug.println("Error: unable to set DG value");
        return;
    }

    // Set gain
    if (Veml3328.setGain(gain_x2)) {
        SerialDebug.println("Error: unable to set device gain");
        return;
    }

    // Set sensitivity (high sensitivity = false, low sensitivity = true)
    if (Veml3328.setSensitivity(true)) {
        SerialDebug.println("Error: unable to set device sensitivity");
        return;
    }

    // Set integration time
    if (Veml3328.setIntTime(time_400)) {
        SerialDebug.println("Error: unable to set device integration time");
        return;
    }
}

void loop() {}