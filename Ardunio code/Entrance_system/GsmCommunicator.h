#ifndef GSM_COMMUNICATOR_H
#define GSM_COMMUNICATOR_H

#include <Wire.h>
#include <SoftwareSerial.h>
#include "MemoryReader.h"

#define OPENING_TIME 3000

//is global variable, should be made a class var
SoftwareSerial soft_serial(10,8);

class GsmCommunicator{
  public:
    GsmCommunicator(byte RX, byte TX, byte relay_pin, byte led_pin);
    void handleCommunication();

  private:
    //SoftwareSerial soft_serial;
    byte relay_pin, led_pin;

    MemoryReader *memory;

    void callRecieved(char tel_number[9]);
    void openDoor();
};


GsmCommunicator::GsmCommunicator(byte RX, byte TX, byte relay_pin, byte led_pin)
{
  this->relay_pin = relay_pin;
  this->led_pin = led_pin;
  
  //this->soft_serial = SoftwareSerial(RX, TX, false);
  this->memory = MemoryReader::getInstance();

  soft_serial.begin(19200);
  //show the callers number in soft_serial
  soft_serial.write("AT+CLIP=1\r");
  delay(50);
  while(soft_serial.available()) soft_serial.read();
}

void GsmCommunicator::handleCommunication(){
  if(!soft_serial.available()) return;

  while(soft_serial.available()){
    if(soft_serial.read() == '+'){
      Serial.println("+ detected");

      //get the command (4 characters)
      String txt = "";
      txt += char(soft_serial.read());
      txt += char(soft_serial.read());
      txt += char(soft_serial.read());
      txt += char(soft_serial.read());

      Serial.println("command " + txt);

      if(txt == "CLIP"){

        // read until "
        char chr = ' ';
        do{
          chr = soft_serial.read();
        }while(chr != '"');

        // remove the prefix 420
        for(byte i = 0; i < 3; i++)
            soft_serial.read();

       //read number as nine chars
        char number[9];
        for(byte i = 0; i < 9; i++)
        {
          if(soft_serial.available())
            number[i] = soft_serial.read();
        }


        if(soft_serial.read() == '"')
        {
          Serial.println("number recieved successfully");
        }

        //recieved number
        for(byte i = 0; i < 9; i++)
          Serial.print(number[i]);
        Serial.println("");


        // read until the end
        while(soft_serial.available()){
          soft_serial.read();
        }

        //send gsm to hold the call
        delay(500);
        soft_serial.write("ATH\r");
        delay(500);

        callRecieved(number);

      }
    }
  }
}

void GsmCommunicator::callRecieved(char tel_number[9]){
  unsigned int number_count = memory->getNumberCount();

  Serial.print("recieving: ");
  for(byte a = 0; a < 9; a++){
      Serial.print(tel_number[a]);
  }
  Serial.println();

  unsigned long address = 0;
  String number;

  for(unsigned int i = 0; i < number_count; i++){

    address = (i * 9) + 2;
    number = memory->readTelNumber(address);

    for(byte a = 0; a < 9; a++){

      if(number[a] != tel_number[a]) break;
      if(a == 8){
        openDoor();
        goto final;
      }
    }

  }
  final: delay(1);
}

void GsmCommunicator::openDoor(){
  digitalWrite(relay_pin,HIGH);
  digitalWrite(led_pin,HIGH);
  delay(OPENING_TIME);
  digitalWrite(relay_pin,LOW);
  digitalWrite(led_pin,LOW);
}

#endif