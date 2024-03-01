#ifndef RF_MODEM_H
#define RF_MODEM_H

#ifdef __cplusplus
  extern "C" {
    #include "../sx1278-LoRa-RaspberryPi/LoRa.h"
  }
#endif

extern LoRa_ctl modem;

extern uint8_t rx_buffer[255];
extern unsigned int rx_buffer_pointer;
extern unsigned int rx_buffer_size;

extern uint8_t modem_available();
extern uint8_t modem_read();
extern void modem_reset();

extern void * rx_f(void *p);
extern void tx_f(txData *tx);

extern void initRFModule();

extern void tx_send(uint8_t* buf, unsigned int size);

extern void modem_finish();

#endif
