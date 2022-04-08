#include <veml3328.h>
#define SerialDebug Serial3 // Add this to print via Serial
#define DEBUG

uint8_t err = 0;

void setup() {
    /* Initialize serial interface */
    SerialDebug.begin(115200);

    /* Initialize VEML3328 library */
    err = Veml3328.begin();
    if (err < 0) {
        SerialDebug.println("Error: could not start VEML3328 library");
    }
}

void loop() {
    /**
     * NOTE: All of these functions have extra print options that can be
     * toggled by adding `#define DEBUG` after the include statement at the top
     * of this file
     */

    /* Set device DG value */
    err = Veml3328.setDG(dg_x2);

    /* Set device gain */
    err = Veml3328.setGain(gain_x2);

    /* Set device sensitivity */
    err = Veml3328.setSensitivity(true);

    /* Set device integration time */
    err = Veml3328.setIntTime(time_400);
}