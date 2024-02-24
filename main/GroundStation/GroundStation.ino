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
  Serial.begin(57600);                   // initialize serial
  while (!Serial);

  Serial.println("PRINT:Gama Ground Station communication system with LoRa Ra-01 rf module");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins(csPin, resetPin, irqPin);// set CS, reset, IRQ pin

  LoRa.setSpreadingFactor(spreading_factor);
  LoRa.setSignalBandwidth(125E3);
  LoRa.setTxPower(tx_power);

  if (!LoRa.begin(frequency)) {             // initialize ratio at 915 MHz
    printControlln(PRINT_LORA_INIT_FAILED); // "CONTROL:LoRa init failed. Check your connections."
    while (true);                       // if failed, do nothing
  }
  printControlln(PRINT_DEVICE_READY); // "CONTROL:Device initiated successfully"
}

bool state_sending = false;
uint8_t send_tx = 0;

unsigned long transmission_timer = 2000;
unsigned long next_transmission = 0;

void loop(){
  updateRFComm();
  checkControl();

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
    }
  }
}
