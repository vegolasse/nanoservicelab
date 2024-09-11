#pragma once
#include <cstdint>
#include <thread>
#include <vector>

class Server {
public:
    Server(int workerThreads, int port);
    ~Server();
    std::vector<std::thread> workers;
    int serverSocket;
    int port;
};

