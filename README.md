[![MCHP](images/microchip.png)](https://www.microchip.com)

# Vishay Semiconductors VEML3328 RGBCIR Color Sensor Driver - AVR-IoT Cellular Mini

## Description

Low-level Arduino driver for the VEML3328 RGBCIR Color Sensor [1], which is mainly focused on the 
AVR-IoT Cellular Mini development kit.

## Install

You can clone this library to the Arduino library folder, usually located in the **Arduino > Libraries** folder.

```
cd <path-to-library-folder>
git clone <https-link-to-repo>
```

## Usage

After installation, add this line to the top of your *.ino* file;

```c
#include <veml3328.h>
```

This adds the library header file to your project, which includes all of the library functions and data types.

You also need to run the [Begin](#begin) command in order to initialize the library. 
See the [Available Commands](#available-commands) section for more information

<br/>

## Available commands

List of the commands available for the MCP9808 library. 

### Begin
There are 2 ways to initialize the MCP9808 library:

```c
/**
 * @brief Initialize VEML3328 library
 *
 * @return int 0 if successful, 1 if failed
 */
int8_t begin(void);
```

This command initializes the VEML3328 library by starting a *TWI* instance and checks that the configuration
register is at a default value before use. 

<br/>

```c
/**
 * @brief Initialize VEML3328 library with custom I2C address
 *
 * @param address Custom I2C address
 * @return int 0 if successful, 1 if failed
 */
int8_t begin(uint8_t address);
```

This command initializes the VEML3328 library in a similar way to the command above, with an exception of the user being able to feed a custom I2C sensor address to the library.

<br/>

### Get color values

```c
/**
 * @brief Get RED channel value
 *
 * @return int16_t Channel value
 */
int16_t getRed(void);
```
```c
/**
 * @brief Get GREEN channel value
 *
 * @return int16_t Channel value
 */
int16_t getGreen(void);
```
```c
/**
 * @brief Get BLUE channel value
 *
 * @return int16_t Channel value
 */
int16_t getBlue(void);
```
```c
/**
 * @brief Get INFRARED(IR) channel value
 *
 * @return int16_t Channel value
 */
int16_t getIR(void);
```
```c
/**
 * @brief Get CLEAR channel value
 *
 * @return int16_t Channel value
 */
int16_t getClear(void);
```
These functions return the color values which is measured by the sensor. 
These values are returned as a 16-bit integer (*int16_t*).

<br/>

### Low power mode

```c
/**
 * @brief Shutdown VEML3328 (low power mode)
 *
 * @return int 0 if successful, 1 if failed
 */
uint8_t shutdown(void);
```

This command sets the device in a low-power (shutdown) mode.

<br/>

```c
/**
 * @brief Wake up VEML3328 (low power mode)
 *
 * @return int 0 if successful, 1 if failed
 */
uint8_t wake(void);
```
This command “wakes” the device up from low-power (shutdown) mode. 

<br/>

```c
/**
 * @brief Shutdown R and B channel on device
 * 
 * @return int 0 if successful, 1 if failed
 */
uint8_t rbShutdown(void);
```
This command effectively "turns of" the Red and Blue channel of the 
color sensor, where the value of these channels are returned as `0`.

<br/>

```c
/**
 * @brief Wake up R and B channel on device
 * 
 * @return int 0 if successful, 1 if failed
 */
uint8_t rbWakeup(void);
```
This command undoes the effect of the previous command, where the Red and Blue
color channels return regular values.

<br/>

### Configuration

#### Device ID
```c
/**
 * @brief Return device ID from sensor
 *
 * @return uint8_t Device ID byte
 */
uint16_t deviceID(void);
```
This command returns the device ID for the VEML3328 device.

<br/>

#### DG value
As stated in the device datasheet [1], there are 3 available options for the
device DG values:

- x 1 (default)
- x 2
- x 4

This is represented in the library as an enumeration struct:

```c
typedef enum 
{ 
	dg_x1 = 0x00, 
	dg_x2 = 0x01, 
	dg_x4 = 0x02 
} DG_t;
```

<br/>

The device DG value can be set by using the following command:
```c
/**
 * @brief Set DG value of device
 * 
 * @param val DG value enum member
 * @return int 0 if successful, 1 if failed
 */
uint8_t setDG(DG_t val);
```

<br/>

#### Sensor gain
As stated in the device datasheet [1], there are 4 available options for the
device sensor gain:

- x 1/2
- x 1 (default)
- x 2
- x 4

This is represented in the library as an enumeration struct:

```c
typedef enum 
{
	gain_x1_2 = 0x03,
	gain_x1 = 0x00,
	gain_x2 = 0x01,
	gain_x4 = 0x02
} gain_t;
```

<br/>

The sensor gain value can be set by using the following command:
```c
/**
 * @brief Set Gain value of device
 * 
 * @param val Gain value enum member
 * @return int 0 if successful, 1 if failed
 */
uint8_t setGain(gain_t val);
```

<br/>

#### Integration time
As stated in the device datasheet [1], there are 4 available options for the
device integration time:

- 50 ms (default)
- 100 ms
- 200 ms
- 400 ms

This is represented in the library as an enumeration struct:

```c
typedef enum
{
	time_50 = 0x00,
	time_100 = 0x01,
	time_200 = 0x02,
	time_400 = 0x03
} int_time_t;
```

<br/>

The device integration time can be set by using the following command:
```c
/**
 * @brief Set integration time of device
 * 
 * @param time Integration time enum member
 * @return int 0 if successful, 1 if failed
 */
uint8_t setIntTime(int_time_t time);
```

<br/>

### Read/write procedure

```c
/**
 * @brief 16-bit write procedure
 *
 * @param reg_ptr Register pointer
 * @param data 16-bit data
 */
void regWrite(uint8_t reg_ptr, uint16_t data);
```

```c
/**
 * @brief 16-bit read procedure
 *
 * @param reg_ptr Register pointer
 * @return uint16_t Returned data
 */
uint16_t regRead(uint8_t reg_ptr);
```

These commands are abstraction functions for reading/writing to the 16-bit registers in the VEML3328 device.

These commands are mostly meant for internal use in the commands mentioned above but are available through 
the library header file for general use. We do not recommend using these functions for functionality like 
reading sensor values, low-power mode etc, but rather use the dedicated functions like [getRed](#get-color-values), [shutdown](#low-power-mode), [wake](#low-power-mode) and similar. 

<br/>

## Examples

Examples can be found in the **examples** folder. There are 3 examples available:

- *color* - Example of getting a color reading from the sensor (Red, Green, Blue, Infrared (IR) and Clear) and sending that data through a serial interface. This example also utilizes commands for turning of 
the Red and Blue color channels.
- *config* - Example of using the configuration functions mentioned in [Configuration](#configuration).
- *device_id* - Example of retrieving the device ID of the sensor device. Mostly used for 
testing the I2C interface.

<br/>

---
References:<br/>
[1] [https://www.vishay.com/docs/84968/veml3328.pdf](https://www.vishay.com/docs/84968/veml3328.pdf)