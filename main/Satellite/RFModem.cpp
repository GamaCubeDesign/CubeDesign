#include "RFModem.h"
#include <iostream>

#include "Timing.h"

using namespace std;

LoRa_ctl modem;
uint8_t rx_buffer[255];
unsigned int rx_buffer_pointer = 0;
unsigned int rx_buffer_size = 0;
bool transmitting = false;
bool tx_done = false;
bool receiving = false;
bool timeout = false;
unsigned long last_reception_timeout = 300000;
unsigned long last_reception_timer = 300000;
unsigned long wait_for_new_transmission = 100;
unsigned long next_transmission_time = 0;
unsigned long last_transmission_time = 0;

uint8_t modem_available(){
  return rx_buffer_size;
}

uint8_t modem_read(){
  rx_buffer_size--;
  return rx_buffer[rx_buffer_pointer++];
}

void * rx_f(void *p){
  last_reception_timeout = my_millis() + last_reception_timer;

  cout << "rx: Receive callback" << endl;
  rxData *rx = (rxData *)p;
  // printf("rx done \n");
  printf("rx: CRC error: %d\n", rx->CRC);
  printf("rx: Data size: %d\n", rx->size);
  // printf("string: ");//Data we'v received
  // for(int i = 0; i < rx->size; i++){
  //     printf("%c", rx->buf[i]);
  // }
  // print("\n");
  printf("rx: RSSI: %d\n", rx->RSSI);
  printf("rx: SNR: %f\n", rx->SNR);

  while(rx_buffer_size > 0);

  rx_buffer_pointer = 0;
  memcpy(rx_buffer, rx->buf, rx->size);
  rx_buffer_size = rx->size;

  free(p);
  cout << "rx: Receive callback done" << endl;
  last_transmission_time = my_millis();
  next_transmission_time = last_transmission_time + wait_for_new_transmission;
  return NULL;
}

void tx_f(txData *tx){
  printf("tx: done \n");
  if(timeout){
    cout << "tx: tx callback but a timout has occured" << endl;
  } else{
    cout << "tx: transmission nominal" << endl;
    transmitting = false;
    receiving = true;
    tx_done = true;
    LoRa_receive(&modem);
  }
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

  LoRa_begin(&modem);
  receiving = true;
  LoRa_receive(&modem);
}

// void check_reception_timeout(){
//   if(my_millis() > last_reception_timeout){
//     last_reception_timeout = my_millis() + last_reception_timer;
//     modem_reset();
//   }
// }

// void modem_reset(){
//   // while(!receiving);
//   // LoRa_stop_receive(&modem);
//   LoRa_end(&modem);
//   usleep(100000);
//   receiving = true;
//   transmitting = false;
//   timeout = false;
//   tx_done = false;
//   LoRa_begin(&modem);
//   LoRa_receive(&modem);
// }

int tx_transmitt(uint8_t* buf, unsigned int size){

  cout << "tx: starting transmission" << endl;
  timeout = false;

  char txbuf[255];
  modem.tx.data.buf = txbuf;

  memcpy(modem.tx.data.buf, buf, size);//copy data we'll sent to buffer
  modem.tx.data.size = size;//Payload len
  LoRa_send(&modem);

  printf("tx: Sleep %f miliseconds to transmitt complete\n", modem.tx.data.Tpkt);
  printf("tx: Sending packet with length %d\n", buf[0]);
  usleep(((int)modem.tx.data.Tpkt)*1000);

  long now = my_millis();
  long timeout_at = now + 1000;
  cout << "tx: now " << now << endl;
  cout << "tx: will timeout at " << timeout_at << endl;
  while(!tx_done && my_millis() < timeout_at){
    // cout << "tx: waiting " << my_millis() << endl;
  }
  if(!tx_done){
    timeout = true;
    return 1;
  }
  return 0;
}

void tx_send(uint8_t* buf, unsigned int size){
  while(my_millis() < next_transmission_time);
  LoRa_stop_receive(&modem);
  receiving = false;
  transmitting = true;
  tx_done = false;

  usleep(50000);

  int counter = 0;
  while(tx_transmitt(buf, size) && counter < 3){
    cout << "tx: Transmission timeout " << my_millis() << endl;
    counter++;
    cout << "tx: Trying again " << counter << endl;
    LoRa_end(&modem);
    LoRa_begin(&modem); // Restart LoRa and try again
  }
  if(!tx_done){ // Give up. Restart LoRa and start listening
    LoRa_end(&modem);
    LoRa_begin(&modem);
    transmitting = false;
    receiving = true;
    LoRa_receive(&modem);
  }
  // usleep(50000);

  // LoRa_send(&modem);

  // printf("Tsym: %f\n", modem.tx.data.Tsym);
  // printf("Tpkt: %f\n", modem.tx.data.Tpkt);
  // printf("payloadSymbNb: %u\n", modem.tx.data.payloadSymbNb);

  // printf("sleep %f miliseconds to transmitt complete\n", modem.tx.data.Tpkt);
  // printf("tx: Sending packet with length %d\n", buf[0]);
  // usleep(((int)modem.tx.data.Tpkt)*1000);

  // if(!tx_done){
  //   cout << "tx: Transmission timeout" << my_millis() << endl;

  //   // transmitting = false;
  //   // receiving = true;
  //   // tx_done = true;
  //   // timeout = true;
    // LoRa_receive(&modem);
  // }

  // printf("end\n");
}

void modem_finish(){
  LoRa_end(&modem);
}