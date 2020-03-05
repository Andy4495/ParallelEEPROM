/* -----------------------------------------------------------------
   ParallelEEPROM Library Example Sketch 1: Read Bytes
   https://github.com/Andy4495/ParallelEEPROM

   03/01/20 - A.T. - Original

*/

#include "ParallelEEPROM.h"

uint16_t i, j;
byte data;

ParallelEEPROM eep(23, 39, 24, 36, 7, 37, 38,       // A14 - A8
                   25, 26, 27, 28, 29, 30, 10, 9 ,  // A7 - A0
                   12, 13, 14, 15, 17, 11, 31, 32,  // D7 - D0
                   8, 6, 40,                        // EEPROM: CE, OE, WE
                   33, 35);                         // 74LVC245: OE, DIR

void setup() {
  eep.begin();
  Serial.begin(115200);

  Serial.println(" ");
  Serial.println("EEPROM Programmer Example 1: Read bytes");
  delay(3000);


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
