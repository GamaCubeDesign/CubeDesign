#ifndef GS_COMM_H
#define GS_COMM_H

#include <SPI.h>              // include libraries
#include <LoRa.h>

#include "Control.h"
// #include "C:\Users\Vitinho\Desktop\TTEC-Gamacubedesign\main\CommunicationProtocol.h"
#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"
// #include "../CommunicationProtocol.h"

#define GSCOM_LED 7

extern uint8_t txAddh;
extern uint8_t txAddl;
// extern uint8_t txChan;
extern uint8_t rxAddh;
extern uint8_t rxAddl;
// extern uint8_t rxChan;
extern uint8_t bandwidth;
extern unsigned long spi_frequency;
extern unsigned long frequency;
extern uint8_t spreading_factor;
extern uint8_t tx_power;

extern Operation operation;

// extern bool switch_active_thermal_control;
// extern bool switch_attitude_control;
// extern bool switch_imaging;
// extern bool switch_imaging_mode;
// extern bool switch_stand_by_mode;

// extern Message message;
// extern TelemetryData telemetryData;
// extern SatPacket satPacket;
// extern GSPacket gsPacket;

// extern uint8_t rx_pointer;

void sendCommand();

void sendGSPacket();
bool listenForResponse(unsigned long int timeout);

void updateRFComm();
void onReceive();

void startRequestStatusProtocol();
void startRequestImagingDataProtocol();
void startSetOperationProtocol();
void ping();

void switchCaseStatusProtocol();
void switchCaseImagingDataProtocol();
void switchCaseSetOperationProtocol();
void switchCasePingProtocol();

#endif