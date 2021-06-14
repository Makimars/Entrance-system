#include <Wire.h>

#include "MemoryReader.h"
#include "GsmCommunicator.h"

//#include <AltSoftSerial.h>
#include <SoftwareSerial.h>

//#include <Eeprom_at24c256.h>
#include <AT24CX.h>

#define ADD_NUMBER      'a'
#define ERASE_NUMBERS   'e'
#define READ_NUMBERS    'r'
#define NUMBER_COUNT    'c'    
#define PING            'p'
#define TEST            't'

#define LED_PIN         13
#define RELAY_PIN       7

#define OPENING_TIME 3000

MemoryReader memory;

//AltSoftSerial serial_gsm(10,12); //RX, TX
SoftwareSerial serial_gsm(10,8); //RX, TX

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN,OUTPUT);
  Serial.begin(115200);

    serial_gsm.begin(19200);
    //show the callers number
    serial_gsm.write("AT+CLIP=1\r");
    delay(50);
    while(serial_gsm.available()) serial_gsm.read();
    Serial.println("gsm");
  
  Serial.println("init");
  Wire.begin();
}

void loop() {
  if(Serial.available()){
    handleSerialCommunication();
  }
  
    if(serial_gsm.available()){
      handleGsmCommunication();
    }

  delay(1);
}

void handleGsmCommunication(){
  while(serial_gsm.available()){
    if(serial_gsm.read() == '+'){
      Serial.println("+ detected");

      //get the command (4 characters)
      String txt = "";
      txt += char(serial_gsm.read());
      txt += char(serial_gsm.read());
      txt += char(serial_gsm.read());
      txt += char(serial_gsm.read());

      Serial.println("command " + txt);
      
      if(txt == "CLIP"){
        
        // read until "
        char chr = ' ';
        do{
          chr = serial_gsm.read();
        }while(chr != '"');

        // remove the prefix 420
        for(byte i = 0; i < 3; i++)
            serial_gsm.read();

       //read number as nine chars
        char number[9];
        for(byte i = 0; i < 9; i++)
        {
          if(serial_gsm.available())
            number[i] = serial_gsm.read();
        }


        if(serial_gsm.read() == '"')
        {
          Serial.println("number recieved successfully");  
        }

        //recieved number
        for(byte i = 0; i < 9; i++)
          Serial.print(number[i]);
        Serial.println("");


        // read until the end
        while(serial_gsm.available()){
          serial_gsm.read();  
        }
        
        //send gsm to hold the call
        delay(500);
        serial_gsm.write("ATH\r");
        delay(500);
        
        callRecieved(number);
        
      }
    }
  }
} 

void handleSerialCommunication(){
  char cmd = Serial.read();
  //Serial.println(cmd);
  byte buff[2] = {0,0};
  unsigned int number_count = 0;

  switch(cmd){
    case ADD_NUMBER:
      char number[9];
      Serial.readBytes(number, 9);
      memory.addTelNumber(number);
      Serial.println("ok");
      break;
    case ERASE_NUMBERS:
      memory.eraseNumbers();
      break;  
    case READ_NUMBERS:
      readAll();
      break;
    case NUMBER_COUNT:
      Serial.print("count: ");
      Serial.println(memory.getNumberCount());
      break;
    case PING:
      Serial.println("ping");
      break;
    case TEST:
      char num[9];
      Serial.readBytes(num, 9);
      callRecieved(num);
      break;
   /* default:
      Serial.print(cmd);
      Serial.println(" not recognized");
      break;*/
  }
    
}

void readAll(){
  unsigned int number_count = memory.getNumberCount();

  unsigned int address;
  String tel_number;

  for(byte i = 0; i < number_count; i++){
    address = (i * 9) + 2;
    tel_number = memory.readTelNumber(address);
    Serial.println(tel_number);
  }
    
}

void callRecieved(char tel_number[9]){
  unsigned int number_count = memory.getNumberCount();

  Serial.print("recieving: ");
  for(byte a = 0; a < 9; a++){
      Serial.print(tel_number[a]);
  }
  Serial.println();
  
  unsigned long address = 0;
  String buff;
 
  for(unsigned int i = 0; i < number_count; i++){
    
    address = (i * 9) + 2;
    
    buff = memory.readTelNumber(address);
    //Serial.println(address);

    
    for(byte a = 0; a < 9; a++){
      if(buff[a] != tel_number[a]) break;
      if(a == 8){
        openDoor();
        goto final;
      }
    }
    
  }
  final: delay(1);
}

void openDoor(){
  Serial.println("door opened");
  digitalWrite(RELAY_PIN,HIGH);
  digitalWrite(LED_PIN,HIGH);
  delay(OPENING_TIME);
  digitalWrite(RELAY_PIN,LOW);
  digitalWrite(LED_PIN,LOW);
}

