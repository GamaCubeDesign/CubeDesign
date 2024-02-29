#include "ImagingServer.h"

using namespace std;

ImagingDataServer::ImagingDataServer(ImagingFIFO *fifo, Operation *operation, unsigned int portNum){
  _fifo = fifo;
  _portNum = portNum;
  _operation = operation;

  ImagingData newPacket;
  for(int i = 0; i < 5; i++){
    newPacket.duration = 0;
    newPacket.index = i;
    newPacket.radius = 0;
    newPacket.type = 0;
    newPacket.x = 0;
    newPacket.y = 0;
    _fifo->write(newPacket);
  }
}

void ImagingDataServer::run_server(){

  // init socket.

  client = socket(AF_INET, SOCK_STREAM, 0);

  if(client < 0){
    cout << "(Imaging) Error establishing connection." << endl;
    exit(1);
  }

  cout <<  "(Imaging) Server Socket connection created..." << endl;

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htons(INADDR_ANY);
  server_addr.sin_port = htons(_portNum);

  // bindind socket

  if(bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
    cout << "(Imaging) Error binding socket..." << endl;
    exit(1);
  }

  size = sizeof(server_addr);
  cout << "(Imaging) Looking for clients..." << endl;

  // listening socket

  listen(client, 2);
  
  do{

    //accept client

    server = accept(client, (struct sockaddr*)&server_addr, &size);

    if(server < 0){
      cout << "(Imaging) Error on accepting..." << endl;
      exit(1);
    }

    ImagingData newPacket;

    if(server > 0){
      strcpy(buffer, "Server connected...\n");
      send(server, buffer, bufsize, 0);

      cout << "(Imaging) connected with client..." << endl;
      
      unsigned int recvN = 0;
      do{
        recvN = recv(server, buffer, bufsize, 0);
        if(recvN > 0){
          // cout << "(Imaging) Received message: ";
          // for(unsigned int i = 0; i < recvN; i++){
          //   cout << buffer[i];
          // }
          // cout << endl;
          if(strcmp(buffer, "SendPacket\n")==0){
            cout << "(Imaging) Receiving packet" << endl;
            strcpy(buffer, "4");
            send(server, buffer, 1, 0);
            int recvN = recv(server, (uint8_t*)&newPacket, sizeof(ImagingData), 0);
            cout << "(Imaging) Packet received" << endl;
            if(recvN > 0){
              _fifo->write(newPacket);
            }
          } else if(strcmp(buffer, "RequestUpdate\n")==0){
            if(!closing){
              if(!_operation->switch_imaging){
                strcpy(buffer, "0");
                send(server, buffer, 1, 0);
              } else if(_operation->switch_imaging_mode == 0){
                strcpy(buffer, "1");
                send(server, buffer, 1, 0);
              } else if(_operation->switch_imaging_mode == 1){
                strcpy(buffer, "2");
                send(server, buffer, 1, 0);
              }
              // send(server, buffer, bufsize, 0);
            } else{
              strcpy(buffer, "3");
              send(server, buffer, 1, 0);
              isExit = true;
            }
          } else{
            cout << "Unknown packet: " << buffer << endl;
          }
        } else{
          cout << "Client disconnected, waiting for new connection" << endl;
        }
      } while(recvN > 0);
    }
  } while(!isExit);
    
  cout << "(Imaging) Connection terminated..." << endl;
  cout << "(Imaging) Goodbye..." << endl;
  // exit(1);
  
  close(client);
}

void ImagingDataServer::close_server(){
  closing = true;
}