#ifdef __cplusplus
extern "C" {
#include "LoRa.h"
}
#endif

void tx_f(txData *tx){
    printf("tx done \n");
}

void * rx_f(void *p){
    rxData *rx = (rxData *)p;
    printf("rx done \n");
    printf("CRC error: %d\n", rx->CRC);
    printf("Data size: %d\n", rx->size);
    printf("string: ");//Data we'v received
    for(int i = 0; i < rx->size; i++){
        printf("%c", rx->buf[i]);
    }
    printf("\nRSSI: %d\n", rx->RSSI);
    printf("SNR: %f\n", rx->SNR);
    free(p);
    return NULL;
}

int main(){

    char txbuf[255];
    LoRa_ctl modem;

    //See for typedefs, enumerations and there values in LoRa.h header file
    modem.spiCS = 0;//Raspberry SPI CE pin number
    modem.tx.callback = tx_f;
    modem.tx.data.buf = txbuf;
    memcpy(modem.tx.data.buf, "LoRa hello", 11);//copy data we'll sent to buffer
    modem.tx.data.size = 11;//Payload len
    modem.rx.callback = rx_f;
    modem.eth.preambleLen=8;
    modem.eth.bw = BW125;//Bandwidth 125KHz
    modem.eth.sf = SF7;//Spreading Factor 8
    modem.eth.ecr = CR5;//Error coding rate CR4/6
    modem.eth.CRC = 1;//Turn on CRC checking
    modem.eth.freq = 433000000;// 434.8MHz
    modem.eth.resetGpioN = 4;//GPIO4 on lora RESET pin
    modem.eth.dio0GpioN = 17;//GPIO17 on lora DIO0 pin to control Rxdone and Txdone interrupts
    modem.eth.outPower = OP17;//Output power
    modem.eth.powerOutPin = PA_BOOST;//Power Amplifire pin
    modem.eth.AGC = 1;//Auto Gain Control
    modem.eth.OCP = 240;// 45 to 240 mA. 0 to turn off protection
    modem.eth.implicitHeader = 0;//Explicit header mode
    modem.eth.syncWord = 0x12;
    //For detail information about SF, Error Coding Rate, Explicit header, Bandwidth, AGC, Over current protection and other features refer to sx127x datasheet https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V5.pdf

    LoRa_begin(&modem);
    LoRa_receive(&modem);

    sleep(60);
    printf("end\n");
    LoRa_end(&modem);
}
