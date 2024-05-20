#include <iostream>
using namespace std;

#ifdef __cplusplus
  extern "C" {
    #include "sx1278-LoRa-RaspberryPi/LoRa.h"
  }
#endif

#include "Logger.h"

LoRa_ctl modem;
Logger logger;

unsigned long int status_write_period = 500;
unsigned long int imaging_write_period = 250;
unsigned long int status_write_time = 0;
unsigned long int imaging_write_time = 0;

bool enable_writing = false;

void * rx_f(void *p){
    rxData *rx = (rxData *)p;
    printf("rx done \n");
    printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    printf("string: %s\n", rx->buf);//Data we'v received
    printf("RSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
    free(p);
    return NULL;
}

void tx_f(txData *tx){
    printf("tx done \n");
}

void initRFModule(){
  char txbuf[255];

  //See for typedefs, enumerations and there values in LoRa.h header file
  modem.spiCS = 0;//Raspberry SPI CE pin number
  modem.tx.callback = tx_f;
  modem.tx.data.buf = txbuf;
  memcpy(modem.tx.data.buf, "LoRa", 5);//copy data we'll sent to buffer
  modem.tx.data.size = 5;//Payload len
  modem.eth.preambleLen=6;
  modem.eth.bw = BW62_5;//Bandwidth 62.5KHz
  modem.eth.sf = SF12;//Spreading Factor 12
  modem.eth.ecr = CR8;//Error coding rate CR4/8
  modem.eth.CRC = 1;//Turn on CRC checking
  modem.eth.freq = 434800000;// 434.8MHz
  modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pin
  modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
  modem.eth.outPower = OP20;//Output power
  modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
  modem.eth.AGC = 1;//Auto Gain Control
  modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
  modem.eth.implicitHeader = 0;//Explicit header mode
  modem.eth.syncWord = 0x12;
  //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

  LoRa_begin(&modem);
}

void tx_send(){
  LoRa_send(&modem);

  printf("Tsym: %f\n", modem.tx.data.Tsym);
  printf("Tpkt: %f\n", modem.tx.data.Tpkt);
  printf("payloadSymbNb: %u\n", modem.tx.data.payloadSymbNb);

  printf("sleep %d seconds to transmitt complete\n", (int)modem.tx.data.Tpkt/1000);
  sleep(((int)modem.tx.data.Tpkt/1000)+1);

  printf("end\n");

  LoRa_end(&modem);
}

void write_status_data(){
  HealthData he = {
    .index = writing_status_counter,
    .time = 1,
    .battery_voltage = 3.7,
    .battery_current = 2.0,
    .battery_charge = 100.0,
    .battery_temperature = 33.0,
    .internal_temperature = 37.9,
    .external_temperature = 51.2,
    .sd_memory_usage = (unsigned long)3,
    .rasp_data = {0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1, 0xa1},
  };
  logger.writeSatStatusPacket(he);
}
void write_imaging_data(){
  ImagingData im;
  for(int i = 0 ; i < 5; i++){
    im.lightnings[i].index = writing_imaging_counter;
    im.lightnings[i].duration = 5;
    im.lightnings[i].radius = 1;
    im.lightnings[i].x = i*10;
    im.lightnings[i].y = i*20;
  }
  logger.writeSatImagingDataPacket(im);
}

void loop(){
  // checkSerial();
  // updateRFComm();

  // unsigned long int t = millis();
  // if(t > status_write_time){
  //   status_write_time += status_write_period;
  //   if(enable_writing){
  //     std::cout << "Writing status data";
      // write_status_data();
  //   }
  // }
  // if(t > imaging_write_time){
  //   imaging_write_time += imaging_write_period;
  //   if(enable_writing){
  //     std::cout << "Writing imaging data";
  //     // write_imaging_data();
  //   }
  // }
}

/*
void checkSerial(){
  char c;
  while(Serial.available()){
    c = Serial.read();
    if(c=='0'){
      enable_writing = !enable_writing;
    } else if(c=='1'){
      switch_status_file();
    } else if(c=='2'){
      switch_imaging_file();
    }
    // if(c=='\n'){
    //   received_serial = receiving_serial;
    //   receiving_serial = "";
    //   serial_received = true;
    // } else{
    //   receiving_serial += c;
    // }
  // }

  // if(serial_received){
  //   serial_received = false;
  //   if(received_serial.length()==0){
  //     // state_sending = !state_sending;
  //     Serial.println("Sending telemetry");
  //     sendTelemetry();
  //     // Serial.print("Toggled communication ");
  //     // Serial.println(state_sending ? "ON":"OFF");
  //   } else if(received_serial[0] == 'T'){
  //     if(received_serial.startsWith("Tset tx chan ")){
  //       txChan = received_serial.substring(13).toInt();
  //       Serial.print("Set tx channel to ");
  //       Serial.println(txChan);
  //     } else if(received_serial.startsWith("Tset tx addh ")){
  //       txAddh = received_serial.substring(13).toInt();
  //       Serial.print("Set tx addh to ");
  //       Serial.println(txAddh);
  //     } else if(received_serial.startsWith("Tset tx addl ")){
  //       txAddl = received_serial.substring(13).toInt();
  //       Serial.print("Set tx addl to ");
  //       Serial.println(txAddl);
  //     }
  //   } else if(received_serial[0] == 'G'){
  //     if(received_serial.startsWith("GAddh")){
  //       printADDH();
  //     } else if(received_serial.startsWith("GAddl")){
  //       printADDL();
  //     } else if(received_serial.startsWith("GParity")){
  //       printParity();
  //     } else if(received_serial.startsWith("GAirDataRate")){
  //       printAirDataRate();
  //     } else if(received_serial.startsWith("GBaudRate")){
  //       printBaudRate();
  //     } else if(received_serial.startsWith("GChannel")){
  //       printChannel();
  //     } else if(received_serial.startsWith("GTxPower")){
  //       printTransmissionPower();
  //     }
  //   } else if(received_serial[0] == 'S'){
  //     parseMessage(received_serial.substring(1));
  //   } else if(received_serial[0] == 'M'){
  //     Serial.println("Set telemetry index to 0");
  //     comm_pointer = 0;
  //   }
  }
}

*/

void init_sd_logger(){
}

int main(){
  std::cout << "Testing Raspberry Gama Satellite communication system with LoRa Ra-01 rf module\n";

  initRFModule();
  std::cout << "Device initiated successfully\n";

  init_sd_logger();
  std::cout << "Logger initiated successfully\n";
  
  // setReceiveCallback(onReceive);

  // while(running){
  //   loop();
  // }

  return 0;
}