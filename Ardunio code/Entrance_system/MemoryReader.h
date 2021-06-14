#include <Wire.h>

#ifndef MEMORY_READER_H
#define MEMORY_READER_H

#include <AT24CX.h>

class MemoryReader {

 public:
    MemoryReader(){}

    void addTelNumber(char number[9]);
    String readTelNumber(unsigned int address);
    unsigned int getNumberCount();
    void eraseNumbers();

  private:
    AT24C256 eeprom;  

};


void MemoryReader::addTelNumber(char number[9]){
  unsigned int number_count = getNumberCount();

  unsigned int address = (number_count * 9) + 2;
  eeprom.write(address, number, 9);

  delay(200);
  number_count++;
  byte count_buff[2] = {
    number_count >> 8,
    number_count
  };
  eeprom.write(0, count_buff, 2);
}

String MemoryReader::readTelNumber(unsigned int address){
  char buff[9];
  eeprom.read(address, (byte *) buff, sizeof(buff));
  return buff;
}

unsigned int MemoryReader::getNumberCount(){
  byte buff[2];
  eeprom.read(0, buff, 2);
  return (buff[0] << 8) + buff[1];
}

void MemoryReader::eraseNumbers()
{
  byte buff[2] = {0,0};
  eeprom.write(0, buff, 2);
}

#endif