/* -----------------------------------------------------------------
   ParallelEEPROM Library Example Sketch 4: Program Device
   https://github.com/Andy4495/ParallelEEPROM

   Program EEPROM with data defined in "eep_program.h" header file

   03/10/20 - A.T. - Original

*/

#include <stdio.h>
#include "ParallelEEPROM.h"
#include "eep_program.h"

#define LED_RED   19   // 75 is the MSP432 on-board green LED
#define LED_GREEN 18   // 76 is the MSP432 on-board red LED
#define SWITCH     5   // 73 is the MSP432 on-board PUSH1

int switch_state = 0;

uint16_t i, j;
byte data;

#define EEP_28C256     // Uncomment this to program a 28C256 32Kx8 EEPROM
//#define EEP_28C16    // Uncomment this to program a 28C16 2Kx8 EEPROM

#ifdef EEP_28C256
ParallelEEPROM   eep(23, 39, 24, 36, 7, 37, 38,       // A14 - A8
                     25, 26, 27, 28, 29, 30, 10, 9 ,  // A7 - A0
                     12, 13, 14, 15, 17, 11, 31, 32,  // D7 - D0
                     8, 6, 40,                        // EEPROM: CE, OE, WE
                     33, 35);                         // 74LVC245: OE, DIR
#endif

#ifdef EEP_28C16
// 28C16
ParallelEEPROM   eep( 7, 37, 38,                       // A10 - A8
                      25, 26, 27, 28, 29, 30, 10, 9 ,  // A7 - A0
                      12, 13, 14, 15, 17, 11, 31, 32,  // D7 - D0
                      8, 6, 36,                        // EEPROM: CE, OE, WE
                      33, 35);                         // 74LVC245: OE, DIR
#endif

void setup() {

  delay(1000);                // Wait a little to get Serial Monitor ready

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(SWITCH, INPUT_PULLUP);

  eep.begin();
  Serial.begin(115200);

  Serial.println(" ");
  Serial.println("Parallel EEPROM Programmer: Press button to start programming. ");

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, HIGH);  // Turn on Green LED to indicate we are ready to program

}

void loop() {
  bool verification_status = true;
  uint16_t data_index = 0;
  uint16_t data_size = sizeof(eep_data);
  char buffer[8];

  while (digitalRead(SWITCH) == HIGH) ; // Wait until button is pressed

  // Start programming
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_RED, HIGH);

  while (data_index < data_size) {
    eep.writeWithPolling(data_index, eep_data[data_index]);
    data_index++;
  }

  Serial.println("Finished writing data. Verifying...");
  delay(100); // Manually wait for write cycle to complete
  data_index = 0;

  while (data_index < data_size) {
    if ( data_index % 16 == 0) {
      snprintf(buffer, 7, "%04x", data_index);
      Serial.println(" ");
      Serial.print(buffer);
      Serial.print(": ");
    }
    data = eep.read(data_index);
    snprintf(buffer, 7, "%02x", data);
    Serial.print(buffer);
    Serial.print(", ");

    if (data != eep_data[data_index]) {
      Serial.println(" ");
      Serial.println("Verification Failed!");
      data_index = data_size;    // Force an end to the while loop
      verification_status = false;
    }
    data_index++;
  }

  Serial.println(" ");
  digitalWrite(LED_RED, LOW);

  // Update status LEDs
  if (verification_status == false) {
    delay(500);
    digitalWrite(LED_RED, HIGH);
    delay(500);
    digitalWrite(LED_RED, LOW);
    delay(500);
    digitalWrite(LED_RED, HIGH);
    delay(500);
    digitalWrite(LED_RED, LOW);
    delay(500);
    digitalWrite(LED_RED, HIGH);
  }
  else {
    Serial.println("Verification Passed.");
    digitalWrite(LED_GREEN, HIGH);
    delay(500);
    digitalWrite(LED_GREEN, LOW);
    delay(500);
    digitalWrite(LED_GREEN, HIGH);
    delay(500);
    digitalWrite(LED_GREEN, LOW);
    delay(500);
    digitalWrite(LED_GREEN, HIGH);

  }

  delay(300);
  Serial.println(" ");
  Serial.println("Parallel EEPROM Programmer: Press button to start programming. ");

}
