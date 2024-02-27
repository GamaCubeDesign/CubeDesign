#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

#include "DataFIFO.h"
#include "../CommunicationProtocol.h"

class StatusDataServer{
  public:
    static const int bufsize=256;
    StatusFIFO *_fifo;
    Operation *_operation;
    int client, server;
    bool isExit = false;
    bool closing = false;
    char buffer[bufsize];
    unsigned int _portNum;

    struct sockaddr_in server_addr;
    socklen_t size;

    StatusDataServer(StatusFIFO *fifo, Operation *operation, unsigned int portNum){
      _fifo = fifo;
      _portNum = portNum;
      _operation = operation;

      HealthData newPacket;
      for(int i = 0; i < 5; i++){
        newPacket.time = i;
        newPacket.index = 0;
        newPacket.sd_memory_usage = 0;
        // newPacket.battery_charge = 0;
        // newPacket.battery_current = 0;
        // newPacket.battery_temperature = 0;
        newPacket.battery_voltage = 0;
        newPacket.external_temperature = 0;
        newPacket.internal_temperature = 0;
        newPacket.altitude = 1.0;
        newPacket.pressure = 1.0;
        newPacket.accel_x = 1;
        newPacket.accel_y = 1;
        newPacket.giros_x = 1;
        newPacket.giros_y = 1;
        _fifo->write(newPacket);
      }
  }

  void run_server(){

    // init socket.

    client = socket(AF_INET, SOCK_STREAM, 0);

    if(client < 0){
      cout << "(Status) Error establishing connection." << endl;
      exit(1);
    }

    cout <<  "(Status) Sserver Socket connection created..." << endl;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(_portNum);

    // bindind socket

    if(bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
      cout << "(Status) Error binding socket..." << endl;
      exit(1);
    }

    size = sizeof(server_addr);
    cout << "(Status) Looking for clients..." << endl;

    // listening socket

    listen(client, 2);

    HealthData newPacket;

    // cout << "Status Waiting for new packet" << endl;
    
    while(!isExit){

      cout << "Status waiting for new packet" << endl;

    //accept client
    // select();

      server = accept(client, (struct sockaddr*)&server_addr, &size);

      if(server < 0){
        cout << "(Status) Error on accepting client..." << endl;
        exit(1);
      }

      if(server > 0){
        // strcpy(buffer, "Server connected...\n");
        // send(server, buffer, 20, 0);

        cout << "(Status) connected with client..." << endl;

        strcpy(buffer, "Ok\n");
        send(server, buffer, 3, 0);
        int recvN = recv(server, (uint8_t*)&newPacket, sizeof(HealthData), 0);
        if(recvN > 0){
          _fifo->write(newPacket);
        }
        cout << "(Status) Packet received" << endl;

        // unsigned int recvN = recv(server, buffer, bufsize, 0);
        // if(recvN==0){
        //   isExit = true;
        //   continue;
        // }
        // if(strcmp(buffer, "SendPacket\n")==0){
        //   // cout << "(Status) Receiving packet" << endl;
        //   strcpy(buffer, "Ok\n");
        //   send(server, buffer, 3, 0);
        //   int recvN = recv(server, (uint8_t*)&newPacket, sizeof(HealthData), 0);
        //   cout << "(Status) Packet received" << endl;
        //   if(recvN > 0){
        //     _fifo->write(newPacket);
        //   }
        // } else if(strcmp(buffer, "RequestUpdate\n")==0){
        //   if(!closing){
        //     strcpy(buffer, "Ok\n");
        //     send(server, buffer, 3, 0);
        //   } else{
        //     strcpy(buffer, "Close\n");
        //     send(server, buffer, 6, 0);
        //     isExit = true;
        //   }
        // } else{
        //   cout << "Unknown packet: " << buffer << endl;
        // }
      }
    }
    
    cout << "(Status) Connection terminated..." << endl;
    cout << "(Status) Goodbye..." << endl;
    
    close(client);
  }

  void close_server(){
    closing = true;
  }
};