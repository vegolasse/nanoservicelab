#pragma once

#include "../ServiceInterface.h"
#include "SocketIO.h"
#include "../ConcurrentQueue.h"

class Client : public ServiceInterface {
public:
    Client(int port, int connections);
    ~Client();
    uint64_t compute(uint64_t number) ;
    int port;

    ConcurrentQueue<SocketIO*> connectionPool;
    int connections;
};
