#ifndef RF_MODULE_H
#define RF_MODULE_H

#define DEBUGGING
#ifdef DEBUGGING
#define DBG_Print(x) std::cout << x
#define DBG_Println(x) std::cout << x << std::endl
#else
#define DBG_Print(x)
#define DBG_Println(x)
#endif

#include <cstdint>

#include "../CommunicationProtocol.h"
#include "RFModem.h"
#include "Timing.h"
#include "Logger.h"
#include "SerialConsole.h"

extern SerialConsole console;

extern uint8_t rxAddh;
extern uint8_t rxAddl;
extern uint8_t rxChan;
extern uint8_t txAddh;
extern uint8_t txAddl;
extern uint8_t txChan;

extern uint8_t rx_pointer;
extern SatPacket satPacket;
extern GSPacket gsPacket;

extern uint8_t number_of_packets;
extern const uint8_t N_status;
extern const uint8_t N_imaging;

void sendSatPacket();

void updateRFComm();
void onReceive();

void preStatusProtocol();
void updateStatusPacket(uint8_t index);
void postStatusProtocol();
void preImagingDataProtocol();
void updateImagingDataPacket(uint8_t index);
void postImagingDataProtocol();
void ping();

void switchCaseStatusProtocol();
void switchCaseImagingDataProtocol();
void switchCaseSetOperationProtocol();

void switch_status_file();
void switch_imaging_file();

#endif