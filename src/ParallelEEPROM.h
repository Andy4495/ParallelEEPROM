/* -----------------------------------------------------------------
   ParallelEEPROM Library
   https://github.com/Andy4495/ParallelEEPROM

   03/05/20 - A.T. - Original
   03/06/20 - A.T. - Use a separate flag to indicate the first write to EEPROM

*/

#ifndef ParallelEEPROM_H
#define ParallelEEPROM_H

#include "Arduino.h"

class ParallelEEPROM {
public:
  // 28C256/X28256 with 74LVC245 transceiver
  ParallelEEPROM(byte A14, byte A13, byte A12, byte A11, byte A10, byte A9, byte A8,
                 byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                 byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                 byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE,
                 byte  LVC_245_OE, byte LVC_245_DIR);

 // 28C16 with 74LVC245 transceiver
 ParallelEEPROM(byte A10, byte A9, byte A8,
                byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
                byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
                byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE,
                byte  LVC_245_OE, byte LVC_245_DIR);

// 28C256/X28256 direct connect to microprocessor (no transceiver on data bus)
ParallelEEPROM(byte A14, byte A13, byte A12, byte A11, byte A10, byte A9, byte A8,
               byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
               byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
               byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE);

// 28C16 direct connect to microprocessor (no transceiver on data bus)
ParallelEEPROM(byte A10, byte A9, byte A8,
               byte  A7, byte  A6, byte  A5, byte  A4, byte  A3, byte A2, byte A1, byte A0,
               byte  D7, byte  D6, byte  D5, byte  D4, byte  D3, byte D2, byte D1, byte D0,
               byte  EEPROM_CE, byte EEPROM_OE, byte EEPROM_WE);


  void begin();
  void write(uint16_t address, byte data);
  byte read(uint16_t address);
  void writeWithPolling(uint16_t address, byte data);
  byte readWithPolling(uint16_t address);
  void writeWithTogglePolling(uint16_t address, byte data);
  byte readWithTogglePolling(uint16_t address);

private:
  enum {NO_PIN = 255};
  enum {NUM_ADDR_LINES = 15};
  enum {NUM_DATA_LINES = 8};
  byte _Addr[NUM_ADDR_LINES];
  byte _Data[NUM_DATA_LINES];
  byte _EEPROM_CE, _EEPROM_OE, _EEPROM_WE;
  byte _LVC245_OE, _LVC245_DIR;
  byte _lastByteWritten;
  uint16_t _lastAddressWritten;
  bool _isThisTheFirstWrite;    // Set to true in constructor, set to false after a write

  void setAddressLines(uint16_t address);
  void setDataOutputMode();
  void setDataInputMode();
};

#endif
