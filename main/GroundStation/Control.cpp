#include "Arduino.h"
#include "Control.h"

const char PRINT_STR[] = "PRINT:";
const char CONTROL_STR[] = "CONTROL:";
const char SEPARATOR_STR = ':';

void printSeparator(){
  Serial.print(SEPARATOR_STR);
}
void printPrint(){
  Serial.print(PRINT_STR);
}
void printPrint(PRINT_ENUM cmd){
  Serial.print(PRINT_STR);
  Serial.print(cmd);
  printSeparator();
}
void printPrintln(PRINT_ENUM cmd){
  Serial.print(PRINT_STR);
  Serial.println(cmd);
}
void printControl(){
  Serial.print(CONTROL_STR);
}
void printControl(CONTROL_ENUM cmd){
  Serial.print(CONTROL_STR);
  Serial.print(cmd);
  printSeparator();
}
void printControlln(CONTROL_ENUM cmd){
  Serial.print(CONTROL_STR);
  Serial.println(cmd);
}

void printBIN(uint8_t *buf, unsigned int size){
  uint8_t b;
  Serial.print("BIN:");
  for(unsigned int i = 0; i < size; i++){
    b = buf[i];
    if(b == 11){
      Serial.write(11);
      Serial.write(11);
    } else if(b == 10){
      Serial.write(11);
      Serial.write(10);
    } else if(b == 13){
      Serial.write(11);
      Serial.write(13);
    } else{
      Serial.write(b);
    }
  }
}

bool serialFlag = false;

unsigned int message_data_index = 0;

bool state_request_status = false;
uint8_t send_request_status = 0;
bool state_request_imaging = false;
uint8_t send_request_imaging = 0;
bool state_command = false;
uint8_t send_command = 0;

void (*parsing_function)(uint8_t c) = parseSerial;

uint8_t rcv_b = 0;
void checkControl(){
  uint8_t c;
  while(Serial.available()){
    c = Serial.read();
    // Serial.println((int)c);
    if(c == ',' || c == '\n' || c == 225){
      parsing_function(rcv_b);
      rcv_b = 0;
      // if(serialFlag){
      //   serialFlag = false;
      // } else if(rcv_b==13){
      //   serialFlag = true;
      // } else if(rcv_b=='\n'){
      //   parsing_function = parseSerial;
      // } else if (rcv_b==255){
      //   parsing_function = parseSerial;
      // } else{
      //   parsing_function(c);
      // }
    } else{
      rcv_b *= 10;
      rcv_b += c - 48;
    }
  }
}

unsigned long number_parser_N = 0;
void (*number_parser_callback)();
void number_parser(uint8_t c){
  if(c==':'){
    number_parser_callback();
  } else{
    number_parser_N *= 10;
    number_parser_N += c-48;
  }
}

void parseSerial(uint8_t c){
  switch(c){
    case READ_RX_ADDH:
      printControl(CONTROL_RX_ADDH);
      Serial.println((unsigned int)rxAddh, HEX);
      break;
    case READ_RX_ADDL:
      printControl(CONTROL_RX_ADDL);
      Serial.println((unsigned int)rxAddl, HEX);
      break;
    case READ_TX_ADDH:
      printControl(CONTROL_TX_ADDH);
      Serial.println((unsigned int)txAddh, HEX);
      break;
    case READ_TX_ADDL:
      printControl(CONTROL_TX_ADDL);
      Serial.println((unsigned int)txAddl, HEX);
      break;
    case READ_BANDWIDTH:
      printControl(CONTROL_BANDWIDTH);
      Serial.println((unsigned int)bandwidth);// Serial.print(" Hz");
      break;
    case READ_FREQUENCY:
      printControl(CONTROL_FREQUENCY);
      Serial.println(frequency);// Serial.println(" MHz");
      break;
    case READ_SPI_FREQUENCY:
      printControl(CONTROL_SPI_FREQUENCY);
      Serial.println(spi_frequency);// Serial.println(" Hz");
      break;
    case READ_SPREADING_FACTOR:
      printControl(CONTROL_SPREADING_FACTOR);
      Serial.println((unsigned int)spreading_factor);
      break;
    case READ_TRANSMISSION_POWER:
      printControl(CONTROL_TXPW);
      Serial.println((unsigned int)tx_power);// Serial.println(" dBm");
      break;
    case READ_ALL:
      // printConfiguration();
      break;
    
    case SET_RX_ADDH:
      parsing_function = control_setRxAddh;
      break;
    case SET_RX_ADDL:
      parsing_function = control_setRxAddl;
      break;
    case SET_TX_ADDH:
      parsing_function = control_setTxAddh;
      break;
    case SET_TX_ADDL:
      parsing_function = control_setTxAddl;
      break;
    case SET_BANDWIDTH:
      parsing_function = control_setBandwidth;
      break;
    case SET_FREQUENCY:
      parsing_function = number_parser;
      number_parser_callback = control_setFrequency;
      break;
    case SET_SPI_FREQUENCY:
      parsing_function = number_parser;
      number_parser_callback = control_setSPIFrequency;
      break;
    case SET_SPREADING_FACTOR:
      parsing_function = control_setSpreadingFactor;
      break;
    case SET_TRANSMISSION_POWER:
      parsing_function = control_setTxPower;
      break;
    
    case FLUSH:
      // telemetry_index = 0;
      break;
    case SET_MESSAGE_DATA:
      break;
    
    case TOGGLE_QUERY_STATUS:
      parsing_function = control_toggleQueryStatus;
      break;
    case SEND_QUERY_STATUS:
      send_request_status++;
      break;
    case TOGGLE_QUERY_IMAGING:
      parsing_function = control_toggleQueryImaging;
      break;
    case SEND_QUERY_IMAGING:
      send_request_imaging++;
      break;
    case TOGGLE_COMMAND:
      parsing_function = control_toggleCommand;
      break;
    case SEND_COMMAND:
      send_command++;
      break;
    
    case SET_ACTIVE_THERMAL_CONTROL:
      parsing_function = setActiveThermalControl;
      break;
    case SET_ATTITUDE_CONTROL:
      parsing_function = setAttitudeControl;
      break;
    case SET_IMAGING:
      parsing_function = setImaging;
      break;
    case SET_IMAGING_MODE:
      parsing_function = setImagingMode;
      break;
    case SET_STAND_BY_MODE:
      parsing_function = setStandByMode;
      break;
    case PING:
      ping();
      break;
    default:
      Serial.print("Unknown command ");Serial.println((int)c);
      break;
  }
}

void control_setRxAddh(uint8_t c){
  rxAddh = c;
  parsing_function = parseSerial;
}

void control_setRxAddl(uint8_t c){
  rxAddl = c;
  parsing_function = parseSerial;
}

void control_setTxAddh(uint8_t c){
  txAddh = c;
  parsing_function = parseSerial;
}

void control_setTxAddl(uint8_t c){
  txAddl = c;
  parsing_function = parseSerial;
}

void control_setBandwidth(uint8_t c){
  bandwidth = c;
  long sbw = 125E3;
  switch(bandwidth){
    case 0:
      sbw = 7.8E3;
      break;
    case 1:
      sbw = 10.4E3;
      break;
    case 2:
      sbw = 15.6E3;
      break;
    case 3:
      sbw = 20.8E3;
      break;
    case 4:
      sbw = 31.25E3;
      break;
    case 5:
      sbw = 41.7E3;
      break;
    case 6:
      sbw = 62.5E3;
      break;
    case 7:
      sbw = 125E3;
      break;
    case 8:
      sbw = 250E3;
      break;
    case 9:
      sbw = 500E3;
      break;
  }
  LoRa.setSignalBandwidth(sbw);
}

void control_setFrequency(){
  frequency = number_parser_N;
  LoRa.setFrequency(frequency);
  parsing_function = parseSerial;
}

void control_setSPIFrequency(){
  LoRa.end();
  spi_frequency = number_parser_N;
  LoRa.setSPIFrequency(spi_frequency);
  if (!LoRa.begin(frequency)) {             // initialize ratio at set frequency
    printControl(PRINT_LORA_INIT_FAILED);
    Serial.println(); //"LoRa init failed. Check your connections."
    while (true);                       // if failed, do nothing
  }
  parsing_function = parseSerial;
}

void control_setSpreadingFactor(uint8_t c){
  spreading_factor = c;
  LoRa.setSpreadingFactor(spreading_factor);
  parsing_function = parseSerial;
}

void control_setTxPower(uint8_t c){
  tx_power = c;
  LoRa.setTxPower(tx_power);
  parsing_function = parseSerial;
}

void control_toggleQueryStatus(uint8_t c){
  Serial.print("PRINT:set status to ");Serial.println(c);
  state_request_status = c != 0;
  parsing_function = parseSerial;
}

void control_toggleQueryImaging(uint8_t c){
  state_request_imaging = c != 0;
  parsing_function = parseSerial;
}

void control_toggleCommand(uint8_t c){
  state_command = c != 0;
  parsing_function = parseSerial;
}

void setActiveThermalControl(uint8_t c){
  operation.switch_active_thermal_control = c==1;
}

void setAttitudeControl(uint8_t c){
  operation.switch_attitude_control = c==1;
}

void setImaging(uint8_t c){
  operation.switch_imaging = c==1;
}

void setImagingMode(uint8_t c){
  operation.switch_imaging_mode = c==1;
}

void setStandByMode(uint8_t c){
  operation.switch_stand_by_mode = c==1;
}

void control_print_status_packet(){
  printPrint(PRINT_STATUS_RECEIVED);
  printBIN((uint8_t*)&satPacket, sizeof(satPacket));
  // printSeparator();Serial.println(satPacket.byte_data.index);
  // Serial.print(PRINT_STATUS_PACKET);
  // printSeparator();Serial.print(satPacket.data.healthData.time);//Reading time:
  // printSeparator();Serial.print(satPacket.data.healthData.battery_voltage);//Battery voltage:
  // printSeparator();Serial.print(satPacket.data.healthData.battery_current);//Battery current:
  // printSeparator();Serial.print(satPacket.data.healthData.battery_charge);//Battery charge:
  // printSeparator();Serial.print(satPacket.data.healthData.battery_temperature);//Battery temperature:
  // printSeparator();Serial.print(satPacket.data.healthData.internal_temperature);//Internal temperature:
  // printSeparator();Serial.print(satPacket.data.healthData.external_temperature);//External temperature:
  // printSeparator();Serial.println(satPacket.data.healthData.sd_memory_usage);//SD memory usage:
  // for(uint8_t i = 0; i < 10; i++){
  //   Serial.println(newPacket.data.healthData.rasp_data[i]);
  // }
}

void control_print_imaging_packet(){
  printPrint(PRINT_IMAGING_RECEIVED);
  printBIN((uint8_t*)&satPacket, sizeof(satPacket));
  // printSeparator();Serial.println(satPacket.byte_data.index);
  // Serial.print(PRINT_IMAGING_PACKET);
  // printSeparator();Serial.print(satPacket.data.imagingData.lightnings[0].index);//"Index:"
  // printSeparator();Serial.print(satPacket.data.imagingData.lightnings[0].duration);//"Duration:"
  // printSeparator();Serial.print(satPacket.data.imagingData.lightnings[0].radius);//"Radius:"
  // printSeparator();Serial.print(satPacket.data.imagingData.lightnings[0].x);//"X:"
  // printSeparator();Serial.println(satPacket.data.imagingData.lightnings[0].y);//"Y:"
}

void control_print_packet_info(){
  printPrint(PRINT_PACKET_INFO);
  // Serial.print("Receiving telemetry:Length:");
  Serial.print((unsigned int)satPacket.length);
  printSeparator();
  // Serial.print(":Protocol:");
  Serial.print((unsigned int)satPacket.operation.protocol);
  printSeparator();
  // Serial.print(":Operation:");
  Serial.println((unsigned int)satPacket.operation.operation);
  // Serial.println(":Telemetry received!");
}