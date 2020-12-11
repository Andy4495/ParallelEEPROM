/* -----------------------------------------------------------------
   ParallelEEPROM Library Example Sketch 5: RAM Test
   https://github.com/Andy4495/ParallelEEPROM

   Test an external parallel RAM chip with various sequences of read/write data.

   Since a RAM chip has the same pinout and generally the same
   signalling structure as an EEPROM, this library can also be used to
   verify that a RAM Chip is functioning normally.

   This sketch is written for 256k-bit (32K x 8 bits) RAM.

   Test sequence (repeats a configurable number of times):
   1. Write 0xAA, 0x55 pattern to all memory, then verify.
   2. Write 0x55, 0xAA pattern to all memory, then verify.
   3. Write 0xFF, 0x00 pattern to all memory, then verify.
   4. Write 0x00, 0xFF pattern to all memory, then verify.
   5. Write a 257 byte Gray code pattern to all memory, then verify.

   12/11/2020 - A.T. - Original

*/

#include "ParallelEEPROM.h"

#define RAMSIZE 0x8000
#define NUMBER_OF_PASSES 3

uint16_t i, j;
byte data;

ParallelEEPROM   ram(23, 39, 24, 36, 7, 37, 38,       // A14 - A8
                     25, 26, 27, 28, 29, 30, 10, 9 ,  // A7 - A0
                     12, 13, 14, 15, 17, 11, 31, 32,  // D7 - D0
                     8, 6, 40,                        // RAM:      CE, OE, WE
                     33, 35);                         // 74LVC245: OE, DIR

unsigned char    pattern[] = {
  0x00, 0x01, 0x03, 0x02, 0x06, 0x07, 0x05, 0x04, // 0 - 7
  0x0c, 0x0d, 0x0f, 0x0e, 0x0a, 0x0b, 0x09, 0x08, // 8 - 15
  0x18, 0x19, 0x1b, 0x1a, 0x1e, 0x1f, 0x1d, 0x1c, // 16 - 23
  0x14, 0x15, 0x17, 0x16, 0x12, 0x13, 0x11, 0x10, // 24 - 31
  0x30, 0x31, 0x33, 0x32, 0x36, 0x37, 0x35, 0x34, // 32 - 39
  0x3c, 0x3d, 0x3f, 0x3e, 0x3a, 0x3b, 0x39, 0x38, // 40 - 47
  0x28, 0x29, 0x2b, 0x2a, 0x2e, 0x2f, 0x2d, 0x2c, // 48 - 55
  0x24, 0x25, 0x27, 0x26, 0x22, 0x23, 0x21, 0x20, // 56 - 63
  0x60, 0x61, 0x63, 0x62, 0x66, 0x67, 0x65, 0x64, // 64 - 71
  0x6c, 0x6d, 0x6f, 0x63, 0x6a, 0x6b, 0x69, 0x68, // 72 - 79
  0x78, 0x79, 0x7b, 0x7a, 0x7e, 0x7f, 0x7d, 0x7c, // 80 - 87
  0x74, 0x75, 0x77, 0x76, 0x72, 0x73, 0x71, 0x70, // 88 - 95
  0x50, 0x51, 0x53, 0x52, 0x56, 0x57, 0x55, 0x54, // 96 - 103
  0x5c, 0x59, 0x5f, 0x5e, 0x5a, 0x5b, 0x59, 0x58, // 104 - 111
  0x48, 0x49, 0x4b, 0x4a, 0x4e, 0x4f, 0x4d, 0x4c, // 112 - 119
  0x44, 0x45, 0x47, 0x46, 0x42, 0x43, 0x41, 0x40, // 120 - 127
  0xc0, 0xc1, 0xc3, 0xc2, 0xc6, 0xc7, 0xc5, 0xc4, // 128 - 135
  0xcc, 0xcd, 0xcf, 0xce, 0xca, 0xcb, 0xc9, 0xc8, // 136 - 143
  0xd8, 0xd9, 0xdb, 0xda, 0xde, 0xdf, 0xdd, 0xdc, // 144 - 151
  0xd4, 0xd5, 0xd7, 0xd6, 0xd2, 0xd3, 0xd1, 0xd0, // 152 - 159
  0xf0, 0xf1, 0xf3, 0xf2, 0xf6, 0xf7, 0xf5, 0xf4, // 160 - 167
  0xfc, 0xfd, 0xff, 0xfe, 0xfa, 0xfb, 0xf9, 0xf8, // 168 - 175
  0xe8, 0xe9, 0xeb, 0xea, 0xee, 0xef, 0xed, 0xec, // 176 - 183
  0xe4, 0xe5, 0xe7, 0xe6, 0xe2, 0xe3, 0xe1, 0xe0, // 184 - 191
  0xa0, 0xa1, 0xa3, 0xa2, 0xa6, 0xa7, 0xa5, 0xa4, // 192 - 199
  0xac, 0xad, 0xaf, 0xae, 0xaa, 0xab, 0xa9, 0xa8, // 200 - 207
  0xb8, 0xb9, 0xbb, 0xba, 0xbe, 0xbf, 0xbd, 0xbc, // 208 - 215
  0xb4, 0xb5, 0xb7, 0xb6, 0xb2, 0xb3, 0xb1, 0xb0, // 216 - 223
  0x90, 0x91, 0x93, 0x92, 0x96, 0x97, 0x95, 0x94, // 224 - 231
  0x9c, 0x9d, 0x94, 0x9e, 0x9a, 0x9b, 0x99, 0x98, // 232 - 239
  0x88, 0x89, 0x8b, 0x8a, 0x8e, 0x8f, 0x8d, 0x8c, // 240 - 247
  0x84, 0x85, 0x87, 0x86, 0x82, 0x83, 0x81, 0x80, // 248 - 255
  0x04  // This last one is not gray code, since we only have 8 bits but using a 257 byte pattern
};

void setup() {
  byte v1, v2;
  int testResult = 0;
  int overallResult = 0;

  ram.begin();
  Serial.begin(115200);

  Serial.println(" ");
  Serial.println("EEPROM Library Example 4: RAM Test");
  delay(1000);

  for (int passes = 0; passes < NUMBER_OF_PASSES; passes++) {
    Serial.println("");
    Serial.print("Pass number ");
    Serial.print(passes + 1);
    Serial.print(" of ");
    Serial.println(NUMBER_OF_PASSES);
    Serial.println("Test 1: 0xAA, 0x55");
    v1 = 0xAA;
    v2 = 0x55;
    if (twoBytePattern(v1, v2) == 0) {
      Serial.println("Test 1 completed successfully");
    }
    else {
      overallResult = 1;
    }

    Serial.println("");
    Serial.println("Test 2: 0x55, 0xAA");
    v1 = 0x55;
    v2 = 0xAA;
    if (twoBytePattern(v1, v2) == 0) {
      Serial.println("Test 2 completed successfully");
    }
    else {
      overallResult = 1;
    }

    Serial.println("");
    Serial.println("Test 3: 0xFF, 0x00");
    v1 = 0xFF;
    v2 = 0x00;
    if (twoBytePattern(v1, v2) == 0) {
      Serial.println("Test 3 completed successfully");
    }
    else {
      overallResult = 1;
    }

    Serial.println("");
    Serial.println("Test 4: 0x00, 0xFF");
    v1 = 0x00;
    v2 = 0xFF;
    if (twoBytePattern(v1, v2) == 0) {
      Serial.println("Test 4 completed successfully");
    }
    else {
      overallResult = 1;
    }

    Serial.println("");
    Serial.println("Test 5: Gray pattern");
    j = 0;
    for (i = 0; i < RAMSIZE; i++) {
      ram.write(i , pattern[j++]);
      if (j > sizeof(pattern) - 1) j = 0; // Need to wrap around the pattern array index 
      if (i % 0x1000 == 0x800) Serial.print(". ");

    }
    Serial.println("");
    j = 0;
    for (i = 0; i < RAMSIZE; i++) {
      if (ram.read(i) != pattern[j++]) {
        Serial.print("Error Location: 0x");
        Serial.print(i, HEX);
        Serial.print(" Write: 0x");
        Serial.print(pattern[j - 1], HEX);
        Serial.print(" Read: 0x");
        Serial.println(ram.read(i), HEX);
        testResult = 1;
      }
      if (j > 256) j = 0;
    }
    if (testResult == 0) {
      Serial.println("Test 5 completed successfully");
    }
    else {
      overallResult = 1;
    }
  }
  Serial.println("");
  if (overallResult == 0) {
    Serial.println("Testing complete: PASSED");
  }
  else {
    Serial.println("Testing complete: FAILED");
  }
}

void loop() {
}

int twoBytePattern(byte a, byte b) {
  int result = 0;

  for (i = 0; i < RAMSIZE; i = i + 2) {
    ram.write(i , a);
    ram.write(i + 1, b);
    if (i % 0x1000 == 0x800) Serial.print(". "); // Print a little status dot periodically since writes are relatively slow
  }
  Serial.println("");
  for (i = 0; i < RAMSIZE; i = i + 2) {
    if (ram.read(i) != a) {
      Serial.print("Error Location: ");
      Serial.print(i, HEX);
      Serial.print(" Write: ");
      Serial.print(a, HEX);
      Serial.print(", Read: ");
      Serial.println(ram.read(i), HEX);
      result = 1;
    }
    if (ram.read(i + 1) != b) {
      Serial.print("Error Location: 0x");
      Serial.print(i, HEX);
      Serial.print(" Write: 0x");
      Serial.print(b, HEX);
      Serial.print(", Read: 0x");
      Serial.println(ram.read(i + 1), HEX);
      result = 1;
    }
  }
  return result;
}
