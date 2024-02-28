#ifndef STATUS_SERVER_H
#define STATUS_SERVER_H

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

    StatusDataServer(StatusFIFO *fifo, Operation *operation, unsigned int portNum);

    void run_server();

    void close_server();
};

#endif