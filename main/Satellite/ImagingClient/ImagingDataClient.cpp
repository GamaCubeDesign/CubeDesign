#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>

using namespace std;

#include "/mnt/DADOS/Workbench/GCD/TTC/TTEC-Gamacubedesign/main/CommunicationProtocol.h"

class ImagingDataClient{
  public:
    int client, server;
    int portNum;
    bool isExit = false;
    bool is_connected = false;
    bool imaging_on = true;
    static const int bufsize=256;
    char buffer[bufsize];
    const char *ip = "127.0.0.1";

    struct sockaddr_in server_addr;
    ImagingDataClient(int portNum){
      this->portNum = portNum;
    }

    int connect_to_socket(){

      // init socket

      client = socket(AF_INET, SOCK_STREAM, 0);

      if(client < 0){
        cout << "Error creating socket..." << endl;
        exit(1);
      }

      cout << "Client socket created" << endl;

      server_addr.sin_family = AF_INET;
      server_addr.sin_port = htons(portNum);

      // connecting socket server

      if(connect(client, (struct sockaddr*)&server_addr, sizeof(server_addr)) == 0){
        cout << "Connecting to server..." << endl;
      } else{
        cout << "Could not connect" << endl;
        cout << "Goodbye" << endl;

        close(client);

        return -1;
      }

      unsigned int recvN = recv(client, buffer, bufsize, 0);
      if(recvN > 0){
        cout << "Connection confirmed" << endl;
      } else{
        cout << "Could not connect" << endl;
        cout << "Goodbye" << endl;

        close(client);

        return -1;
      }

      is_connected = true;
      return 0;
    }

    void send_packet(ImagingData newPacket){
      strcpy(buffer, "SendPacket\n");
      send(client, buffer, bufsize, 0);
      unsigned int recvN = recv(client, buffer, bufsize, 0);
      if(strcmp(buffer, "Ok\n")==0){
        cout << "Sending packet..." << endl;
        send(client, (uint8_t*)&newPacket, sizeof(ImagingData), 0);
      }
    }

    int update(){
      strcpy(buffer, "RequestUpdate\n");
      cout << "Requesting update" << endl;
      send(client, buffer, bufsize, 0);
      recv(client, buffer, bufsize, 0);
      cout << "Update received" << endl;
      if(strcmp(buffer, "Close\n")==0){
        isExit = true;
        cout << "Closing connection" << endl;
        
        close_client();

        cout << "Connection terminated..." << endl;
        cout << "Goodbye" << endl;
      } else if(strcmp(buffer, "Ok 0\n")==0){
        cout << "Nominal operation, imaging off" << endl;
        return 0;
      } else if(strcmp(buffer, "Ok 1\n")==0){
        cout << "Nominal operation, imaging on, type 1" << endl;
        return 1;
      } else if(strcmp(buffer, "Ok 2\n")==0){
        cout << "Nominal operation, imaging on, type 2" << endl;
        return 2;
      } else{
        cout << "Unknown response: " << buffer << endl;
        return 0;
      }
    }

    void close_client(){
      is_connected = false;
      close(client);
    }
};