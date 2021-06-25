#include <Wire.h>

#include "MemoryReader.h"
#include "GsmCommunicator.h"
#include "SerialCommunicator.h"

#define GSM_RX  10
#define GSM_TX  8

#define LED_PIN   13
#define RELAY_PIN 7

SerialCommunicator serial;

SoftwareSerial gsm_serial(GSM_RX, GSM_TX);
GsmCommunicator gsm(&gsm_serial, RELAY_PIN, LED_PIN);

void setup() {
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN,OUTPUT);

  Wire.begin();
}

void loop() {
  serial.handleCommunication();
  gsm.handleCommunication();

  delay(1);
}