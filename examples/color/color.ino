#include <veml3328.h> 
#define SerialDebug Serial3 // Add this to print via Serial

uint8_t err = 0;
int16_t red, green, blue, ir, clear = 0;

void setup(void) {
    /* Initialize serial interface */
    SerialDebug.begin(115200);

    /* Initialize VEML3328 library */
    err = Veml3328.begin();
    if (err) {
        SerialDebug.println("Error: could not start VEML3328 library");
    }

    /* Initialize with custom I2C address (optional) */
    // err = Veml3328.begin(0x2);
    // if (err < 0) {
    //     SerialDebug.println("Error: could not start VEML3328 library with custom address");
    // }
    
    /* Wake up device */
    err = Veml3328.wake();
    if (err) {
        SerialDebug.println("Error: could not wake up the sensor");
    }
}

void loop() {
    red = Veml3328.getRed();
    green = Veml3328.getGreen();
    blue = Veml3328.getBlue();
    ir = Veml3328.getIR();
    clear = Veml3328.getClear();

    SerialDebug.printf("Red: %d\n", red);
    SerialDebug.printf("Green: %d\n", green);
    SerialDebug.printf("Blue: %d\n", blue);
    SerialDebug.printf("IR: %d\n", ir);
    SerialDebug.printf("Clear: %d\n\n", clear);
    
    /* Shutdown Red and Blue channels */
    SerialDebug.printf("\nShutting down R/B channels\n");
    err = Veml3328.rbShutdown();
    if (err) {
        SerialDebug.println("Error: could not shutdown R/B channels");
    }

    delay(100);

    red = Veml3328.getRed();
    green = Veml3328.getGreen();
    blue = Veml3328.getBlue();
    ir = Veml3328.getIR();
    clear = Veml3328.getClear();

    SerialDebug.printf("Red: %d\n", red);
    SerialDebug.printf("Green: %d\n", green);
    SerialDebug.printf("Blue: %d\n", blue);
    SerialDebug.printf("IR: %d\n", ir);
    SerialDebug.printf("Clear: %d\n\n", clear);

    /* Wake up Red and Blue channels */
    SerialDebug.printf("\nWaking up R/B channels\n");
    err = Veml3328.rbWakeup();
    if (err) {
        SerialDebug.println("Error: could not wake up R/B channels");
    }

    delay(1000);
}