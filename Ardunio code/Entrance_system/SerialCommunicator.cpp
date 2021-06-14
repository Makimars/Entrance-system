#include "SerialCommunicator.h"

SerialCommunicator::SerialCommunicator(){
  this->memory = MemoryReader::getInstance();

  Serial.begin(115200);
  Serial.println("init");
}

void SerialCommunicator::handleCommunication(){
  if(!Serial.available()) return;

  char cmd = Serial.read();
  unsigned int number_count = 0;

  switch(cmd){
    case ADD_NUMBER:
      char number[9];
      Serial.readBytes(number, 9);
      memory->addTelNumber(number);
      Serial.println("ok");
      break;
    case ERASE_NUMBERS:
      memory->eraseNumbers();
      Serial.println("Memory cleared");
      break;
    case READ_NUMBERS:
      readAll();
      break;
    case NUMBER_COUNT:
      Serial.print("count: ");
      Serial.println(memory->getNumberCount());
      break;
    case PING:
      Serial.println("ping");
      break;
    case TEST:
      char num[9];
      Serial.readBytes(num, 9);
      //callRecieved(num);
      break;
    default:
      Serial.print(cmd);
      Serial.println(" not recognized");
      break;
    }
}

void SerialCommunicator::readAll(){
  unsigned int number_count = memory->getNumberCount();

  unsigned int address;
  String tel_number;

  for(byte i = 0; i < number_count; i++){
    address = (i * 9) + 2;
    tel_number = memory->readTelNumber(address);
    Serial.println(tel_number);
  }
}
