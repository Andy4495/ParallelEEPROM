# ParallelEEPROM Library

[![Arduino Compile Sketches](https://github.com/Andy4495/ParallelEEPROM/actions/workflows/arduino-compile-sketches.yml/badge.svg)](https://github.com/Andy4495/ParallelEEPROM/actions/workflows/arduino-compile-sketches.yml)
[![Check Markdown Links](https://github.com/Andy4495/ParallelEEPROM/actions/workflows/CheckMarkdownLinks.yml/badge.svg)](https://github.com/Andy4495/ParallelEEPROM/actions/workflows/CheckMarkdownLinks.yml)

This is a parallel EEPROM programmer library. It currently supports 32Kx8 (28C256 and X28256) and 2Kx8 (28C16) EEPROMs. 32Kx8 and 2Kx8 chips with the same pinout as these devices have not been tested, but will probably work with this library. Other sizes may be supported in the future.  

Example program 4 in the `examples` folder (`ParallelEEPROM_EX4_program_device.ino`) implements an EEPROM programmer device, complete with Serial output text indicating the data programmed and pass/fail status.

Example program 5 in the `examples` folder (`ParallelEEPROM_EX5_RAM_Test.ino`) implements an external parallel RAM test. Since a RAM chip has the same pinout and generally the same signaling structure as an EEPROM, this library can also be used to verify that a RAM Chip is functioning normally.

The library has been tested with a 3.3V MSP432 microcontroller, and therefore also supports an optional 74LVC245 transceiver to level-shift the data signals between the EEPROM and the MSP432.

Since this is intended for a parallel EEPROM device, a large number of I/O pins are required. For example, with 28C256 devices and a 74LVC245 transceiver, a total of 28 I/O pins are required (15 address, 8 data, and 5 control pins). In addition, a large amount of program memory is required to hold to EEPROM program data. For this reason, this library was only tested on MPS432, but may work with other microcontrollers with sufficient program memory and I/O.

After a write operation to the EEPROM, reads from and writes to the EEPROM will not return valid data until the previous write operation has completed. The library supports both DATA polling and TOGGLE polling to detect the completion of a write cycle so that hard-coded delays in the sketch are not required. Since not all chips support the different polling types, methods are provided which support no polling (requiring a hard-coded delay after writes), DATA polling, or TOGGLE polling.

DATA polling uses the most significant data bit (IO7) to signal when a write operation has completed. TOGGLE polling uses IO6. For details on how polling works and which polling method is supported, see the specific device datasheet.

The library currently supports single-byte reads and writes. Future updates may include support for multi-byte reads and page writes.

## Usage

_Be sure to review the example sketches included with the library._

First, **include** the library header file:

```cpp
#include <ParallelEEPROM.h>
```

Next, **instantiate** an ParallelEEPROM object. The constructor used depends on the EEPROM type and whether you are using a 74LVC245 or similar transceiver on the data lines:

1. 32KB device (28C256/X28256) with 74LVC245:

    ```cpp
    ParallelEEPROM eep(byte A14, byte A13, byte A12, byte A11, byte A10, byte A9, byte A8,
                       byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                       byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                       byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE,
                       byte  245_OE, byte 245_DIR);
    ```

2. 2KB device (28C16) with 74LVC245:

    ```cpp
    ParallelEEPROM eep(byte A10, byte A9, byte A8,
                       byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                       byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                       byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE,
                       byte  245_OE, byte 245_DIR);
    ```

3. 32KB device (28C256/X28256) without 74LVC245:

    ```cpp
    ParallelEEPROM eep(byte A14, byte A13, byte A12, byte A11, byte A10, byte A9, byte A8,
                       byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                       byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                       byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE);
    ```

4. 2KB device (28C16) without 74LVC245:

    ```cpp
    ParallelEEPROM eep(byte A10, byte A9, byte A8,
                       byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                       byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                       byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE);
    ```

    The constructor parameters are the pin numbers for up to 15 address pins on the EEPROM, 8 data pins 74LVC245 (B side) or directly connected to EEPROM, control pins for the EEPROM (CE, OE, WE), and control pins for the 74LVC245 (OE and DIR) if used. Note that the library assumes that the A side of the 74LVC245 is connected to the EEPROM, and the B side is connected to the MSP432.

Then **initialize** the object (typically within `setup()`):

```cpp
eep.begin();
```

### Library Methods

```cpp
// No polling after a write. Use a hard-coded delay (per device datasheet) in sketch after write
void write(uint16_t address, byte data);

// Use DATA polling to automatically check if previous write completed before starting next write
void writeWithPolling(uint16_t address, byte data);

// Use TOGGLE polling to automatically check if previous write completed before starting next write
void writeWithTogglePolling(uint16_t address, byte data);

// No polling before a read. Make sure that sketch has proper delay after any previous write before reading data
byte read(uint16_t address);

// Use DATA polling to automatically check if previous write completed before reading
byte readWithPolling(uint16_t address);

// Use TOGGLE polling to automatically check if previous write completed before reading
byte readWithTogglePolling(uint16_t address);
```

### Hardware Notes

Be sure to connect 10K pull-up resistors as follows:

- /CE signal to +5V
  - 28C256/X28256 pin 20
  - 28C16 pin 18
- 74LVC245 /OE signal (pin 19) to +3.3V

Each chip should have a 0.1 uF decoupling capacitor connected between Vcc and GND as close to the chip as possible.

When using a 74LVC245 or other transceiver, the library assumes that the A side of the 74LVC245 is connected to the EEPROM data lines, and the B side is connected to the microcontroller.

In other words, connect as follows:

```text
EEPROM Pin  74LVC245 Pin  Chip Signal Name
----------  ------------  ----------------
    19           2          I/O7 <-> A1
    18           3          I/O6 <-> A2
    17           4          I/O5 <-> A3
    16           5          I/O4 <-> A4
    15           6          I/O3 <-> A5
    13           7          I/O2 <-> A6
    12           8          I/O1 <-> A7
    11           9          I/O0 <-> A8
```

In order to support both 28C256 and 28C16, the programming hardware needs to account for slightly different pinouts between the chips. In particular, the 28C256/X28256 has 28 pins and the 28C16 has 24. Pin 26 is an address pin on the 28C256/X28256, which corresponds to Vcc pin 24 on the 28C16. The programming hardware therefore has a jumper at pin 26/24 so that A13 can be selected when using a 28 pin device, and Vcc can be selected when using a 24 pin device.

## References

- Xicor X28256 [datasheet][1]
- Atmel 28C256 [datasheet][3]
- Atmel 28C16 [datasheet][4]
- TI 74LVC245 [datasheet][2]
- Parallel RAM 62256 (32K x 8 bit) [datasheet][5]

## License

The software and other files in this repository are released under what is commonly called the [MIT License][100]. See the file [`LICENSE`][101] in this repository.

[1]:./extras/hardware/Xicor%20x28256%20EEPROM.pdf
[2]:https://www.ti.com/lit/ds/symlink/sn74lvc245a.pdf
[3]:http://ww1.microchip.com/downloads/en/DeviceDoc/doc0006.pdf
[4]:https://www.jameco.com/Jameco/Products/ProdDS/74691AT.pdf
[5]:https://www.jameco.com/Jameco/Products/ProdDS/82472.pdf
[100]: https://choosealicense.com/licenses/mit/
[101]: ./LICENSE
[//]: # ([200]: https://github.com/Andy4495/ParallelEEPROM)

[//]: # (Dead link from older version of README: X28256 datasheet http://www.bgmicro.com/pdf/x28256.pdf)
