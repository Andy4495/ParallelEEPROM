/* -----------------------------------------------------------------
   ParallelEEPROM Library Example Sketch 2: Read and Write
   https://github.com/Andy4495/ParallelEEPROM

   Writes a sequence of data to EEPROM memory and reads it back.

   From address 0x0000 to 0x00FF, data sequences from 0x00 to 0xFF
   From address 0x0100 to 0x01FF, data sequences from 0x01 to 0x00
   From address 0x0200 to 0x02ff, no data is written, so the data
        that is printed is whatever happened to be in memory previously.

   03/01/20 - A.T. - Original

*/

#include "ParallelEEPROM.h"

uint16_t i, j;
byte data;

ParallelEEPROM   eep(23, 39, 24, 36, 7, 37, 38,       // A14 - A8
                     25, 26, 27, 28, 29, 30, 10, 9 ,  // A7 - A0
                     12, 13, 14, 15, 17, 11, 31, 32,  // D7 - D0
                     8, 6, 40,                        // EEPROM: CE, OE, WE
                     33, 35);                         // 74LVC245: OE, DIR

void setup() {
  eep.begin();
  Serial.begin(115200);

  Serial.println(" ");
  Serial.println("EEPROM Programmer Example 2: Read and Write");
  delay(3000);

  data = 0;
  for (i = 0; i < 0x0100; i++) {
    if (i % 16 == 0) {
      Serial.print(i, HEX);
      Serial.println(": Write");
    }
    eep.write(i , data++);
    delay(6);
  }
  data++;
  for (i = 0x0100; i < 0x0200; i++) {
    eep.write(i, data++);
    delay(6);     // Need a minium 5 ms delay until library implements polling
  }

  for (i = 0; i < 0x0300 / 16; i++) {
    Serial.print(i * 16, HEX);
    Serial.print(": ");
    for (j = 0; j < 16; j++) {
      data = eep.read(i * 16 + j);
      Serial.print(data, HEX);
      Serial.print(" ");
    }
    Serial.println(" ");
  }

}

void loop() {

}
