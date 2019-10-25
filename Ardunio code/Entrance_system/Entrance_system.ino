//#include <Eeprom_at24c256.h>

#include <Wire.h>
//#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <AT24CX.h>
//#include <Eeprom_at24c256.h>

#define ADD_NUMBER      'a'
#define ERASE_NUMBERS   'e'
#define READ_NUMBERS    'r'
#define NUMBER_COUNT    'c'    
#define PING            'p'
#define TEST            't'

#define LED_PIN         12
#define RELAY_PIN       7

//Eeprom_at24c256 eeprom(0x50);
AT24C256 eeprom(0x50);

//AltSoftSerial serial_gsm(10,12); //RX, TX
SoftwareSerial serial_gsm(10,13); //RX, TX

void setup() {
  pinMode(8, OUTPUT);
  pinMode(12,OUTPUT);
  Serial.begin(115200);
  serial_gsm.begin(19200);
  //show the callers number
  serial_gsm.write("AT+CLIP=1\r");
  delay(50);
  while(serial_gsm.available()) serial_gsm.read();
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
        //for(byte i = 0; i < 9; i++)
        //  Serial.print(number[i]);
        //Serial.println("");


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
      addTelNumber(number);
      Serial.print("adding number: ");
      for(byte a = 0; a < 9; a++) Serial.print(number[a]);
      Serial.println("");
      break;
    case ERASE_NUMBERS:
      eeprom.write(0, buff, 2);
      Serial.println("Memory cleared");
      break;  
    case READ_NUMBERS:
      readAll();
      break;
    case NUMBER_COUNT:
      Serial.print("count: ");
      Serial.println(getNumberCount());
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
  unsigned int number_count = getNumberCount();

  unsigned int address;
  char buff[9];
  for(byte i = 0; i < number_count; i++){
    address = (i * 9) + 2;
    eeprom.read(address, (byte *) buff, 9);
    //Serial.println(sizeof(buff));
    for(byte a = 0; a < 9; a++) Serial.print(buff[a]);
    Serial.println("");
  }
    
}

void callRecieved(char tel_number[9]){
  unsigned int number_count = getNumberCount();

  Serial.print("recieving: ");
  for(byte a = 0; a < 9; a++){
      Serial.print(tel_number[a]);
  }
  Serial.println();
  
  unsigned long address = 0;
  char buff[9];
 
  for(unsigned int i = 0; i < number_count; i++){
    
    address = (i * 9) + 2;
    
    eeprom.read(address, (byte *) buff, 9);
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
  delay(3000);
  digitalWrite(RELAY_PIN,LOW);
  digitalWrite(LED_PIN,LOW);
}

void addTelNumber(char number[9]){
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

String readTelNumber(unsigned int address){
  char buff[9];
  eeprom.read(address, (byte *) buff, sizeof(buff));
  return buff;
}

unsigned int getNumberCount(){
  byte buff[2];
  eeprom.read(0, buff, 2);
  return (buff[0] << 8) + buff[1];
}
