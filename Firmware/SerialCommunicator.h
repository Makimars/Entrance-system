#ifndef SERIAL_COMMUNICATOR_H
#define SERIAL_COMMUNICATOR_H

#include <Wire.h>
#include "MemoryReader.h"

#define ADD_NUMBER      'a'
#define ERASE_NUMBERS   'e'
#define READ_NUMBERS    'r'
#define NUMBER_COUNT    'c'    
#define PING            'p'
#define TEST            't'

class SerialCommunicator{
  public:
    SerialCommunicator();

    void handleCommunication();

  private:
    MemoryReader *memory;

    void readAll();
};

#endif