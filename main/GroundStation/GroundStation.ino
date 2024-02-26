#include <SPI.h>              // include libraries
#include <LoRa.h>

#include "Control.h"
#include "GSComm.h"
// #include "C:\Users\Vitinho\Desktop\TTEC-Gamacubedesign\main\CommunicationProtocol.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"
// #include "../CommunicationProtocol.h"

const int csPin = 10;          // LoRa radio chip select
const int resetPin = 9;       // LoRa radio reset
const int irqPin = 8;         // change for your board; must be a hardware interrupt pin

void setup(){
  pinMode(GSCOM_LED, OUTPUT);
  digitalWrite(GSCOM_LED, LOW);
  delay(100);
  digitalWrite(GSCOM_LED, HIGH);
  delay(500);
  digitalWrite(GSCOM_LED, LOW);
  delay(100);

  Serial.begin(57600);                   // initialize serial
  while (!Serial);

  satPacket.data.healthData.accel_x = 10.0;
  satPacket.data.healthData.accel_y = 11.0;
  satPacket.data.healthData.giros_x = 12.0;
  satPacket.data.healthData.giros_y = 13.0;
  Serial.print("CONTROL:STATUS PACKET:");
  Serial.print(satPacket.data.healthData.accel_x);
  Serial.print(":");
  Serial.print(satPacket.data.healthData.accel_y);
  Serial.print(":");
  Serial.print(satPacket.data.healthData.giros_x);
  Serial.print(":");
  Serial.println(satPacket.data.healthData.giros_y);

  Serial.println("PRINT:Gama Ground Station communication system with LoRa Ra-01 rf module");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  LoRa.setSPIFrequency(20000);
  LoRa.setSpreadingFactor(7);
  LoRa.setCodingRate4(5);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setPreambleLength(8);
  LoRa.setTxPower(tx_power);
  LoRa.enableCrc();

  if (!LoRa.begin(433e6)) {             // initialize ratio at 433 MHz
    printControlln(PRINT_LORA_INIT_FAILED); // "CONTROL:LoRa init failed. Check your connections."
    while (true);                       // if failed, do nothing
  }
  printControlln(PRINT_DEVICE_READY); // "CONTROL:Device initiated successfully"
  Serial.println("PRINT:DEVICE READY");

  digitalWrite(GSCOM_LED, HIGH);
  delay(300);
  digitalWrite(GSCOM_LED, LOW);
  delay(300);
  digitalWrite(GSCOM_LED, HIGH);
  delay(300);
  digitalWrite(GSCOM_LED, LOW);
  delay(300);
}

bool state_sending = false;
uint8_t send_tx = 0;

unsigned long transmission_timer = 2000;
unsigned long next_transmission = 0;

void loop(){
  updateRFComm();
  checkControl();

  // Serial.println("sending LoRa hello");
  // LoRa.beginPacket();                                 // start packet
  // // LoRa.write(txAddh);                                 // add destination high address
  // // LoRa.write(txAddl);                                 // add destination low address
  // // LoRa.write(rxAddh);                                 // add sender high address
  // // LoRa.write(rxAddl);                                 // add sender low address
  // // LoRa.write(gsPacket.length);                        // add payload length
  // LoRa.println("LoRa hello");   // add payload
  // LoRa.endPacket();  

  if(millis() >= next_transmission){
    next_transmission += transmission_timer;
     if(state_request_status){
      startRequestStatusProtocol();
     } else if(send_request_status > 0){
      send_request_status--;
      startRequestStatusProtocol();
     } else if(state_request_imaging){
      startRequestImagingDataProtocol();
     } else if(send_request_imaging > 0){
      send_request_imaging--;
      startRequestImagingDataProtocol();
     } else if(state_command){
      startSetOperationProtocol();
    } else if(send_command > 0){
      send_command--;
      startSetOperationProtocol();
    } else{
      // ping();
    }
  }
}
