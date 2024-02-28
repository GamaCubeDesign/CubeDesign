#ifndef IMAGING_DATA_SERVER
#define IMAGING_DATA_SERVER

#include <iostream>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>

#include "DataFIFO.h"
#include "../CommunicationProtocol.h"

class ImagingDataServer{
  public:
    static const int bufsize=256;
    ImagingFIFO *_fifo;
    Operation *_operation;
    int client, server;
    bool isExit = false;
    bool closing = false;
    char buffer[bufsize];
    unsigned int _portNum;

    struct sockaddr_in server_addr;
    socklen_t size;

    ImagingDataServer(ImagingFIFO *fifo, Operation *operation, unsigned int portNum);

    void run_server();

    void close_server();
};

#endif