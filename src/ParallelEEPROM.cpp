/* -----------------------------------------------------------------
   ParallelEEPROM Library
   https://github.com/Andy4495/ParallelEEPROM

   03/05/20 - A.T. - Original

*/

#include "ParallelEEPROM.h"

// 28C256/X28256 with 74LVC245 transceiver
ParallelEEPROM::ParallelEEPROM(byte A14, byte A13, byte A12, byte A11, byte A10, byte A9, byte A8,
                               byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                               byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                               byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE,
                               byte  LVC_245_OE, byte LVC_245_DIR) {
  _Addr[14] = A14;
  _Addr[13] = A13;
  _Addr[12] = A12;
  _Addr[11] = A11;
  _Addr[10] = A10;
  _Addr[9]  = A9;
  _Addr[8]  = A8;
  _Addr[7]  = A7;
  _Addr[6]  = A6;
  _Addr[5]  = A5;
  _Addr[4]  = A4;
  _Addr[3]  = A3;
  _Addr[2]  = A2;
  _Addr[1]  = A1;
  _Addr[0]  = A0;
  _Data[7]  = D7;
  _Data[6]  = D6;
  _Data[5]  = D5;
  _Data[4]  = D4;
  _Data[3]  = D3;
  _Data[2]  = D2;
  _Data[1]  = D1;
  _Data[0]  = D0;
  _EEPROM_CE  = EEPROM_CE;
  _EEPROM_OE  = EEPROM_OE;
  _EEPROM_WE  = EEPROM_WE;
  _LVC245_OE  = LVC_245_OE;
  _LVC245_DIR = LVC_245_DIR;
  _lastAddressWritten = 0xFFFF; // Use 0xFFFF as flag that we haven't written anything yet. This won't work if we start supporting 28C512 chips.
}

// 28C16 with 74LVC245 transceiver
ParallelEEPROM::ParallelEEPROM(byte A10, byte A9, byte A8,
                               byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                               byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                               byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE,
                               byte  LVC_245_OE, byte LVC_245_DIR) {
  _Addr[14] = NO_PIN;
  _Addr[13] = NO_PIN;
  _Addr[12] = NO_PIN;
  _Addr[11] = NO_PIN;
  _Addr[10] = A10;
  _Addr[9]  = A9;
  _Addr[8]  = A8;
  _Addr[7]  = A7;
  _Addr[6]  = A6;
  _Addr[5]  = A5;
  _Addr[4]  = A4;
  _Addr[3]  = A3;
  _Addr[2]  = A2;
  _Addr[1]  = A1;
  _Addr[0]  = A0;
  _Data[7]  = D7;
  _Data[6]  = D6;
  _Data[5]  = D5;
  _Data[4]  = D4;
  _Data[3]  = D3;
  _Data[2]  = D2;
  _Data[1]  = D1;
  _Data[0]  = D0;
  _EEPROM_CE  = EEPROM_CE;
  _EEPROM_OE  = EEPROM_OE;
  _EEPROM_WE  = EEPROM_WE;
  _LVC245_OE  = LVC_245_OE;
  _LVC245_DIR = LVC_245_DIR;
  _lastAddressWritten = 0xFFFF; // Use 0xFFFF as flag that we haven't written anything yet. This won't work if we start supporting 28C512 chips.
}

// 28C256/X28256 direct connect to microprocessor (no transceiver on data bus)
ParallelEEPROM::ParallelEEPROM(byte A14, byte A13, byte A12, byte A11, byte A10, byte A9, byte A8,
                               byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                               byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                               byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE) {
  _Addr[14] = A14;
  _Addr[13] = A13;
  _Addr[12] = A12;
  _Addr[11] = A11;
  _Addr[10] = A10;
  _Addr[9]  = A9;
  _Addr[8]  = A8;
  _Addr[7]  = A7;
  _Addr[6]  = A6;
  _Addr[5]  = A5;
  _Addr[4]  = A4;
  _Addr[3]  = A3;
  _Addr[2]  = A2;
  _Addr[1]  = A1;
  _Addr[0]  = A0;
  _Data[7]  = D7;
  _Data[6]  = D6;
  _Data[5]  = D5;
  _Data[4]  = D4;
  _Data[3]  = D3;
  _Data[2]  = D2;
  _Data[1]  = D1;
  _Data[0]  = D0;
  _EEPROM_CE  = EEPROM_CE;
  _EEPROM_OE  = EEPROM_OE;
  _EEPROM_WE  = EEPROM_WE;
  _LVC245_OE  = NO_PIN;
  _LVC245_DIR = NO_PIN;
  _lastAddressWritten = 0xFFFF; // Use 0xFFFF as flag that we haven't written anything yet. This won't work if we start supporting 28C512 chips.
}

// 28C16 direct connect to microprocessor (no transceiver on data bus)
ParallelEEPROM::ParallelEEPROM(byte A10, byte A9, byte A8,
              byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
              byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
              byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE) {

  _Addr[14] = NO_PIN;
  _Addr[13] = NO_PIN;
  _Addr[12] = NO_PIN;
  _Addr[11] = NO_PIN;
  _Addr[10] = A10;
  _Addr[9]  = A9;
  _Addr[8]  = A8;
  _Addr[7]  = A7;
  _Addr[6]  = A6;
  _Addr[5]  = A5;
  _Addr[4]  = A4;
  _Addr[3]  = A3;
  _Addr[2]  = A2;
  _Addr[1]  = A1;
  _Addr[0]  = A0;
  _Data[7]  = D7;
  _Data[6]  = D6;
  _Data[5]  = D5;
  _Data[4]  = D4;
  _Data[3]  = D3;
  _Data[2]  = D2;
  _Data[1]  = D1;
  _Data[0]  = D0;
  _EEPROM_CE  = EEPROM_CE;
  _EEPROM_OE  = EEPROM_OE;
  _EEPROM_WE  = EEPROM_WE;
  _LVC245_OE  = NO_PIN;
  _LVC245_DIR = NO_PIN;
  _lastAddressWritten = 0xFFFF; // Use 0xFFFF as flag that we haven't written anything yet. This won't work if we start supporting 28C512 chips.
}

void ParallelEEPROM::begin() {
  byte i;

  for (i = 0; i < NUM_ADDR_LINES; i++) {
    if (_Addr[i] != NO_PIN) pinMode(_Addr[i], OUTPUT);
  }

  setDataInputMode();
  digitalWrite(_EEPROM_CE, HIGH);
  pinMode(_EEPROM_CE, OUTPUT);
  pinMode(_EEPROM_OE, OUTPUT);
  digitalWrite(_EEPROM_OE, HIGH);
  pinMode(_EEPROM_WE, OUTPUT);
  digitalWrite(_EEPROM_WE, HIGH);
  if (_LVC245_OE != NO_PIN) {
    digitalWrite(_LVC245_OE, HIGH);
    pinMode(_LVC245_OE, OUTPUT);
  }
  if (_LVC245_DIR != NO_PIN) {
    pinMode(_LVC245_DIR, OUTPUT);
    digitalWrite(_LVC245_DIR, HIGH);
  }
}

void ParallelEEPROM::write(uint16_t address, byte data) {
  int i;

  setAddressLines(address);
  setDataOutputMode();
  if (_LVC245_DIR != NO_PIN) digitalWrite(_LVC245_DIR, LOW);
  if (_LVC245_OE != NO_PIN) digitalWrite(_LVC245_OE, LOW);
  for (i = 0; i < NUM_DATA_LINES; i++) {
    if (data & 0x01) digitalWrite(_Data[i], HIGH);
    else digitalWrite(_Data[i], LOW);
    data = data >> 1;
  }
  digitalWrite(_EEPROM_CE, LOW);
  // tCS Write Setup Time 0 ns (no delay needed)
  digitalWrite(_EEPROM_WE, LOW);
  // tWP Write Pulse Width 150 ns minimum - 6 clock cycles on MSP432 48 MHz
  // digitalWrite calls GPIO_write which easily takes more than 6 cycles
  digitalWrite(_EEPROM_WE, HIGH);
  // tCH min 0 ns (no delay needed)
  digitalWrite(_EEPROM_CE, HIGH);
  // No delay needed, data already read above
  if (_LVC245_OE != NO_PIN) digitalWrite(_LVC245_OE, HIGH);
  setDataInputMode();
  _lastAddressWritten = address;
  _lastByteWritten    = data;
}

// This method uses Data Polling to check if prevous write
// cycle completed before starting another write.
// When using this method, no explicit delays are required after writes.
void ParallelEEPROM::writeWithPolling(uint16_t address, byte data) {
  // First check if this is the first write to the chip.
  // NOTE: This implementation will need to change if this library ever supports 28C512 chips
  if (_lastAddressWritten != 0xFFFF) {
    // Check if bit 7 is the complement of previous written bit.
    // Once bit 7 is correct, then write may proceed.
    while ((read(_lastAddressWritten) & 0x80) != (_lastByteWritten & 0x80) ) ;
  }
  write(address, data);
}

// This method uses Toggle Bit Polling to check if prevous write
// cycle completed before starting another write.
// When using this method, no explicit delays are required after writes.
void ParallelEEPROM::writeWithTogglePolling(uint16_t address, byte data) {
  byte prevData, currentData;
  // Check if bit 6 is toggling on subsequent read operations.
  // Once bit 6 stops toggling, then write may proceed.
  prevData = read(address);
  currentData = read(address);
  while ((currentData & 0x40) != (prevData & 0x40)) {
    prevData = currentData;
    currentData = read(address);
  }
  write(address, data);
}

byte ParallelEEPROM::read(uint16_t address) {
  byte data = 0;

  setAddressLines(address);
  digitalWrite(_EEPROM_CE, LOW);
  // tCE: Need at least 250 ns after CE low before reading data
  digitalWrite(_EEPROM_OE, LOW);
  // tOE: Need at least 100 ns after OE low before reading data
  // The digitalWrites that follow easily make a > 250 ns delay
  if (_LVC245_DIR != NO_PIN) digitalWrite(_LVC245_DIR, HIGH);
  if (_LVC245_OE != NO_PIN) digitalWrite(_LVC245_OE, LOW);
  //delayMicroseconds(3);

  if (digitalRead(_Data[7]) == HIGH) data |= 0x80;
  if (digitalRead(_Data[6]) == HIGH) data |= 0x40;
  if (digitalRead(_Data[5]) == HIGH) data |= 0x20;
  if (digitalRead(_Data[4]) == HIGH) data |= 0x10;
  if (digitalRead(_Data[3]) == HIGH) data |= 0x08;
  if (digitalRead(_Data[2]) == HIGH) data |= 0x04;
  if (digitalRead(_Data[1]) == HIGH) data |= 0x02;
  if (digitalRead(_Data[0]) == HIGH) data |= 0x01;

  if (_LVC245_OE != NO_PIN) digitalWrite(_LVC245_OE, HIGH);
  digitalWrite(_EEPROM_OE, HIGH);
  digitalWrite(_EEPROM_CE, HIGH);

  return data;

}

// This method uses Data Polling to check if prevous write
// cycle completed before reading.
// When using this method, no explicit delays are required after writes.
byte ParallelEEPROM::readWithPolling(uint16_t address) {
  // First check if there has been a write to the chip.
  // NOTE: This implementation will need to change if this library ever supports 28C512 chips
  if (_lastAddressWritten != 0xFFFF) {
    // Check if bit 7 is the complement of previous written bit.
    // Once bit 7 is correct, then write may proceed.
    while ((read(_lastAddressWritten) & 0x80) != (_lastByteWritten & 0x80) ) ;
  }
  read(address);
}

// This method uses Toggle Bit Polling to check if prevous write
// cycle completed before trying to read the EEPROM.
// When using this method, no explicit delays are required after writes.
byte ParallelEEPROM::readWithTogglePolling(uint16_t address) {
  byte prevData, currentData;
  // Check if bit 6 is toggling on subsequent read operations.
  // Once bit 6 stops toggling, then go ahead and read.
  prevData = read(address);
  currentData = read(address);
  while ((currentData & 0x40) != (prevData & 0x40)) {
    prevData = currentData;
    currentData = read(address);
  }
  return(currentData);
}

void ParallelEEPROM::setAddressLines(uint16_t address) {
  int i;

  if (_Addr[14] != NO_PIN) (address & 0x4000) ? digitalWrite(_Addr[14], HIGH) : digitalWrite(_Addr[14], LOW);
  if (_Addr[13] != NO_PIN) (address & 0x2000) ? digitalWrite(_Addr[13], HIGH) : digitalWrite(_Addr[13], LOW);
  if (_Addr[12] != NO_PIN) (address & 0x1000) ? digitalWrite(_Addr[12], HIGH) : digitalWrite(_Addr[12], LOW);
  if (_Addr[11] != NO_PIN) (address & 0x0800) ? digitalWrite(_Addr[11], HIGH) : digitalWrite(_Addr[11], LOW);
  (address & 0x0400) ? digitalWrite(_Addr[10], HIGH) : digitalWrite(_Addr[10], LOW);
  (address & 0x0200) ? digitalWrite(_Addr[9],  HIGH) : digitalWrite(_Addr[9],  LOW);
  (address & 0x0100) ? digitalWrite(_Addr[8],  HIGH) : digitalWrite(_Addr[8],  LOW);
  (address & 0x0080) ? digitalWrite(_Addr[7],  HIGH) : digitalWrite(_Addr[7],  LOW);
  (address & 0x0040) ? digitalWrite(_Addr[6],  HIGH) : digitalWrite(_Addr[6],  LOW);
  (address & 0x0020) ? digitalWrite(_Addr[5],  HIGH) : digitalWrite(_Addr[5],  LOW);
  (address & 0x0010) ? digitalWrite(_Addr[4],  HIGH) : digitalWrite(_Addr[4],  LOW);
  (address & 0x0008) ? digitalWrite(_Addr[3],  HIGH) : digitalWrite(_Addr[3],  LOW);
  (address & 0x0004) ? digitalWrite(_Addr[2],  HIGH) : digitalWrite(_Addr[2],  LOW);
  (address & 0x0002) ? digitalWrite(_Addr[1],  HIGH) : digitalWrite(_Addr[1],  LOW);
  (address & 0x0001) ? digitalWrite(_Addr[0],  HIGH) : digitalWrite(_Addr[0],  LOW);
}

void ParallelEEPROM::setDataOutputMode() {
  int i;

  for (i = 0; i < NUM_DATA_LINES; i++) {
    pinMode(_Data[i], OUTPUT);
  }
}

void ParallelEEPROM::setDataInputMode() {
  int i;
  for (i = 0; i < NUM_DATA_LINES; i++) {
    pinMode(_Data[i], INPUT);
  }
}
