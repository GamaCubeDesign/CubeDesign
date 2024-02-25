#include "Arduino.h"
#include "GSComm.h"

// uint8_t txChan = 23;
uint8_t txAddh = 0xa1;
uint8_t txAddl = 0x06;
// uint8_t rxChan = 10;
uint8_t rxAddh = 0x8f;
uint8_t rxAddl = 0xf7;

uint8_t bandwidth = 7;
unsigned long spi_frequency = 8E6;
unsigned long frequency = 433E6;
uint8_t spreading_factor = 9;
uint8_t tx_power = 17;

bool talking = false;
unsigned long int communication_timeout;
const unsigned long int communication_timeout_limit = 1000;
bool telemetry_received = false;
uint8_t telemetry_state = 0;

Operation operation = {
  .switch_active_thermal_control = false,
  .switch_attitude_control = false,
  .switch_imaging = false,
  .switch_imaging_mode = false,
  .switch_stand_by_mode = false,
};

uint8_t rx_pointer = 0;

SatPacket satPacket;int satPacketSize = sizeof(SatPacket);
GSPacket gsPacket;int gsPacketSize = sizeof(GSPacket);
long tmp_var = sizeof(long);
long tmp_var2 = sizeof(int);

void sendGSPacket(){
  Serial.print(PRINT_STR);
  Serial.print("Sending a message:Length:");
  Serial.println(gsPacket.length);

  LoRa.beginPacket();                                 // start packet
  // LoRa.write(txAddh);                                 // add destination high address
  // LoRa.write(txAddl);                                 // add destination low address
  // LoRa.write(rxAddh);                                 // add sender high address
  // LoRa.write(rxAddl);                                 // add sender low address
  // LoRa.write(gsPacket.length);                        // add payload length
  LoRa.write((uint8_t*)&gsPacket, gsPacket.length);   // add payload
  LoRa.endPacket();                                   // finish packet and send it

  communication_timeout = millis() + communication_timeout_limit;
}

// void sendCommand(){
//   gsPacket.length = 2;
//   gsPacket.operation.protocol = GS_GREETING;
//   unsigned long int t0 = millis();
//   sendGSPacket();
//   if(!getTransmissionResult(500)){
//     //Serial.println("Transmission failed.\nSkipping to next iteration.");
//   } else{
//     //Serial.print("Communication took ");
//     //Serial.println(millis() - t0);
    // //Serial.println("Waiting for response");
    // if(listenForResponse(1000)){
    //   gsPacket.length = 1;
    //   gsPacket.operation = PROTOCOL_START_TELEMETRY_TRANSMISSION;
    //   t0 = millis();
    //   sendGSPacket();
    //   if(!getTransmissionResult(500)){
    //     //Serial.println("Transmission failed.\nSkipping to next iteration.");
    //   } else{
    //     //Serial.print("Communication took ");
    //     //Serial.println(millis() - t0);
    //     //Serial.println("Waiting for response");
    //     if(listenForResponse(1000)){
    //       //Serial.println(satPacket.data.numberOfPackets);
    //       //Serial.println(satPacket.data.telemetryPacket.index);
    //     } else{
    //       //Serial.print("No response");
    //     }
    //   }
    // } else{
    //   //Serial.println("No response");
    // }
//   }
// }

bool listenForResponse(unsigned long int timeout){
  unsigned long int to = millis() + timeout;
  while(millis() < to){
    updateRFComm();
    if(telemetry_received){
      telemetry_received = false;
      return true;
    }
  }
  return false;
}

void updateRFComm(){
  uint8_t b;

  // parse for a packet, and call onReceive with the result:
  unsigned int packetSize = LoRa.parsePacket();
  if(packetSize > 0){
    delay(50);
    // unsigned int recipient = (LoRa.read()<<8) | LoRa.read();          // recipient address
    // unsigned int sender = (LoRa.read()<<8) | LoRa.read();            // sender address
    // unsigned int incomingLength = LoRa.read();    // incoming msg length

    uint8_t b;
    while(LoRa.available()){
      b = LoRa.read();
      ((uint8_t*)(&satPacket))[rx_pointer++] = b;
      if(rx_pointer>0 && rx_pointer==satPacket.length){
        telemetry_received = true;
        onReceive();
        rx_pointer = 0;
      }
      if(rx_pointer >= sizeof(satPacket)){
        rx_pointer = 0;
      }
    }
    communication_timeout = millis() + communication_timeout_limit;
  }

  if(talking){
    if(millis() > communication_timeout){
      //Serial.println("Timeout");
      rx_pointer = 0;
      talking = false;
    }
  }
}

void startRequestStatusProtocol(){
  if(talking){
    return;
  }
  talking = true;
  gsPacket.operation.protocol = PROTOCOL_STATUS;
  gsPacket.operation.operation = GS_STATUS_REQUEST;
  gsPacket.length = 2;
  sendGSPacket();
}

void startRequestImagingDataProtocol(){
  if(talking){
    return;
  }
  talking = true;
  gsPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
  gsPacket.operation.operation = GS_IMAGING_REQUEST;
  gsPacket.length = 2;
  sendGSPacket();
}

void startSetOperationProtocol(){
  if(talking){
    return;
  }
  gsPacket.operation.protocol = PROTOCOL_SET_OPERATION;
  gsPacket.operation.operation = GS_SET_OPERATION;
  gsPacket.data.operation.switch_active_thermal_control = operation.switch_active_thermal_control;
  gsPacket.data.operation.switch_attitude_control = operation.switch_attitude_control;
  gsPacket.data.operation.switch_imaging = operation.switch_imaging;
  gsPacket.data.operation.switch_imaging_mode = operation.switch_imaging_mode;
  gsPacket.data.operation.switch_stand_by_mode = operation.switch_stand_by_mode;

  gsPacket.length = 3;

  talking = true;
  sendGSPacket();
}

void ping(){
  if(talking){
    Serial.println("PRINT:Cant communicate while talking");
    return;
  }

  // Serial.println("sending LoRa hello");
  // LoRa.beginPacket();                                 // start packet
  // LoRa.println("LoRa hello");   // add payload
  // LoRa.endPacket();

  digitalWrite(GSCOM_LED, HIGH);
  delay(100);
  digitalWrite(GSCOM_LED, LOW);
  delay(100);

  gsPacket.operation.protocol = PROTOCOL_PING;
  gsPacket.operation.operation = GS_PING_REQUEST;

  gsPacket.length = 2;

  // talking = true;
  sendGSPacket();
}

void onReceive(){
  control_print_packet_info();
  switch(satPacket.operation.protocol){
    case PROTOCOL_STATUS:
      switchCaseStatusProtocol();
      break;
    case PROTOCOL_IMAGING_DATA:
      switchCaseImagingDataProtocol();
      break;
    case PROTOCOL_SET_OPERATION:
      switchCaseSetOperationProtocol();
      break;
    case PROTOCOL_PING:
      switchCasePingProtocol();
      break;
  }
}

void switchCaseStatusProtocol(){
  unsigned int k = 0;
  switch(satPacket.operation.operation){
    case SATELLITE_STATUS_PACKETS_AVAILABLE:
      printPrint();
      Serial.print("Status: packets available: ");
      Serial.println(satPacket.byte_data.number_of_packets);
      for(unsigned int i = 0; i < 32; i++){
        for(unsigned int j = 0; j < 8; j++){
          if(k < satPacket.byte_data.number_of_packets){
            bitSet(gsPacket.data.resend.packets[i],j);
          } else{
            bitClear(gsPacket.data.resend.packets[i],j);
          }
          k++;
        }
      }
      gsPacket.operation.operation = GS_STATUS_START_TRANSMISSION;
      gsPacket.length = 2;
      sendGSPacket();
      break;
    case SATELLITE_STATUS_PACKET:
      Serial.print(PRINT_STR);
      Serial.print("Status: Packet: ");
      Serial.println(satPacket.byte_data.index);
      bitClear(gsPacket.data.resend.packets[satPacket.byte_data.index>>3],satPacket.byte_data.index&0x07);
      control_print_status_packet();
      break;
    case SATELLITE_STATUS_PACKETS_DONE:
      Serial.print(PRINT_STR);
      Serial.println("Status: Packets done");
      gsPacket.data.resend.isDone = true;
      for(unsigned int i = 0; i < 32; i++){
        if(gsPacket.data.resend.packets[i]!=0){
          gsPacket.data.resend.isDone = false;
          break;
        }
      }
      if(gsPacket.data.resend.isDone){
        gsPacket.operation.protocol = PROTOCOL_STATUS;
        gsPacket.operation.operation = GS_STATUS_DONE;
        gsPacket.length = 2;
      } else{
        gsPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
        gsPacket.operation.operation = GS_IMAGING_RESEND_STATUS;
        gsPacket.length = sizeof(SatPacket);//35;
      }
      sendGSPacket();
      break;
    case SATELLITE_STATUS_DONE:
      Serial.print(PRINT_STR);
      Serial.println("Status: Done");
      talking = false;
      break;
  }
}

void switchCaseImagingDataProtocol(){
  unsigned int k = 0;
  switch(satPacket.operation.operation){
    case SATELLITE_IMAGING_PACKETS_AVAILABLE:
      //Serial.print(PRINT_STR);
      //Serial.print("Imaging: Number of packets available: ");
      //Serial.println(satPacket.byte_data.number_of_packets);
      for(unsigned int i = 0; i < 32; i++){
        for(unsigned int j = 0; j < 8; j++){
          if(k < satPacket.byte_data.number_of_packets){
            bitSet(gsPacket.data.resend.packets[i],j);
          } else{
            bitClear(gsPacket.data.resend.packets[i],j);
          }
          k++;
        }
      }
      gsPacket.operation.operation = GS_IMAGING_START_TRANSMISSION;
      gsPacket.length = 2;
      sendGSPacket();
      break;
    case SATELLITE_IMAGING_PACKET:
      bitClear(gsPacket.data.resend.packets[satPacket.byte_data.index>>3],satPacket.byte_data.index&0x07);
      control_print_status_packet();
      break;
    case SATELLITE_IMAGING_PACKETS_DONE:
      //Serial.print(PRINT_STR);
      //Serial.println("Imaging: Packets done");
      gsPacket.data.resend.isDone = true;
      for(unsigned int i = 0; i < 32; i++){
        if(gsPacket.data.resend.packets[i]!=0){
          gsPacket.data.resend.isDone = false;
          break;
        }
      }
      if(gsPacket.data.resend.isDone){
        gsPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
        gsPacket.operation.operation = GS_IMAGING_DONE;
        gsPacket.length = 2;
      } else{
        gsPacket.operation.protocol = PROTOCOL_IMAGING_DATA;
        gsPacket.operation.operation = GS_IMAGING_RESEND_STATUS;
        gsPacket.length = sizeof(SatPacket);//35;
      }
      sendGSPacket();
      break;
    case SATELLITE_IMAGING_DONE:
      //Serial.print(PRINT_STR);
      //Serial.println("Imaging: Done");
      talking = false;
      break;
  }
}

void switchCaseSetOperationProtocol(){
  switch(satPacket.operation.operation){
    case SATELLITE_SET_OPERATION_ECHO:
      //Serial.print(PRINT_STR);
      //Serial.println("Set operation: Echo");
      if(satPacket.byte_data.byte == *((uint8_t*)&operation)){
        //Serial.print(PRINT_STR);
        //Serial.println("Set operation: Operation correct");
        gsPacket.operation.protocol = PROTOCOL_SET_OPERATION;
        gsPacket.operation.operation = GS_SET_OPERATION_DONE;
        gsPacket.length = 2;
        sendGSPacket();
      } else{
        //Serial.print(PRINT_STR);
        //Serial.println("Set operation: Operation incorrect, resending");
        startSetOperationProtocol();
      }
      break;
    case SATELLITE_SET_OPERATION_DONE:
      //Serial.print(PRINT_STR);
      //Serial.println("Set operation: Done");
      rx_pointer = 0;
      talking = false;
      break;
  }
}

void switchCasePingProtocol(){
  switch(satPacket.operation.operation){
    case SATELLITE_PING_RESPONSE:
      Serial.println("PRINT:Satellite ping!");
      digitalWrite(GSCOM_LED, HIGH);
      delay(100);
      digitalWrite(GSCOM_LED, LOW);
      delay(100);
      digitalWrite(GSCOM_LED, HIGH);
      delay(100);
      digitalWrite(GSCOM_LED, LOW);
      delay(100);
      digitalWrite(GSCOM_LED, HIGH);
      delay(100);
      digitalWrite(GSCOM_LED, LOW);
      break;
  }
}