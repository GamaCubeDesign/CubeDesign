#include "RFModem.h"
#include <iostream>
using namespace std;

LoRa_ctl modem;
uint8_t rx_buffer[255];
unsigned int rx_buffer_pointer = 0;
unsigned int rx_buffer_size = 0;

uint8_t modem_available(){
  return rx_buffer_size;
}

uint8_t modem_read(){
  rx_buffer_size--;
  return rx_buffer[rx_buffer_pointer++];
}

void * rx_f(void *p){
  // cout << "Receive callback" << endl;
    rxData *rx = (rxData *)p;
    // printf("rx done \n");
    printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    // printf("string: ");//Data we'v received
    // for(int i = 0; i < rx->size; i++){
    //     printf("%c", rx->buf[i]);
    // }
    // print("\n");
    printf("RSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);

    rx_buffer_pointer = 0;
    memcpy(rx_buffer, rx->buf, rx->size);
    rx_buffer_size = rx->size;

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
    // modem.tx.data.buf = txbuf;
    // memcpy(modem.tx.data.buf, "LoRa hello", 11);//copy data we'll sent to buffer
    // modem.tx.data.size = 11;//Payload len
  modem.rx.callback = rx_f;
  modem.eth.preambleLen=8;
  modem.eth.bw = BW125;//Bandwidth 125KHz
  modem.eth.sf = SF7;//Spreading Factor 8
  modem.eth.ecr = CR5;//Error coding rate CR4/6
  modem.eth.CRC = 1;//Turn on CRC checking
  modem.eth.freq = 433000000;// 434.8MHz
  modem.eth.resetGpioN = 5;//GPIO4 on lora RESET pin
  modem.eth.dio0GpioN = 6;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
  modem.eth.outPower = OP17;//Output power
  modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
  modem.eth.AGC = 1;//Auto Gain Control
  modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
  modem.eth.implicitHeader = 0;//Explicit header mode
  modem.eth.syncWord = 0x12;
  //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

    // LoRa_begin(&modem);
    // LoRa_send(&modem);

    // printf("Tsym: %f\n", modem.tx.data.Tsym);
    // printf("Tpkt: %f\n", modem.tx.data.Tpkt);
    // printf("payloadSymbNb: %u\n", modem.tx.data.payloadSymbNb);

    // printf("sleep %d seconds to transmitt complete\n", (int)modem.tx.data.Tpkt/1000);
    // sleep(((int)modem.tx.data.Tpkt/1000)+1);

    // printf("end\n");

    // LoRa_end(&modem);
  LoRa_begin(&modem);
  LoRa_receive(&modem);
}

void tx_send(uint8_t* buf, unsigned int size){
  LoRa_stop_receive(&modem);

  char txbuf[255];
  modem.tx.data.buf = txbuf;

  memcpy(modem.tx.data.buf, buf, size);//copy data we'll sent to buffer
  modem.tx.data.size = size;//Payload len

  LoRa_send(&modem);

  printf("Tsym: %f\n", modem.tx.data.Tsym);
  printf("Tpkt: %f\n", modem.tx.data.Tpkt);
  printf("payloadSymbNb: %u\n", modem.tx.data.payloadSymbNb);

  printf("sleep %d seconds to transmitt complete\n", (int)modem.tx.data.Tpkt/1000);
  sleep(((int)modem.tx.data.Tpkt/1000)+1);

  printf("end\n");
  LoRa_receive(&modem);
}

void modem_finish(){
  LoRa_end(&modem);
}