#ifndef GSM_COMMUNICATOR_H
#define GSM_COMMUNICATOR_H

#include <Wire.h>
#include <SoftwareSerial.h>
#include "MemoryReader.h"

#define OPENING_TIME 3000

class GsmCommunicator{
  public:
    GsmCommunicator(SoftwareSerial *serial, byte relay_pin, byte led_pin);
    void handleCommunication();

  private:
    byte relay_pin, led_pin;

    MemoryReader *memory;
    SoftwareSerial *soft_serial;

    void callRecieved(char tel_number[9]);
    void openDoor();
};

#endif