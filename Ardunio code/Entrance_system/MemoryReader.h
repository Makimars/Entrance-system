#include <Wire.h>

#ifndef MEMORY_READER_H
#define MEMORY_READER_H

#include <AT24CX.h>

class MemoryReader {

  public:
    static MemoryReader *getInstance();

    void addTelNumber(char number[9]);
    String readTelNumber(unsigned int address);
    unsigned int getNumberCount();
    void eraseNumbers();

  private:
    static MemoryReader *instance;

    MemoryReader(){}
    AT24C256 eeprom;  

};

#endif