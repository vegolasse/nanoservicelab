#include "Server.h"
#include "../ActualFunction.h"
#include "SocketIO.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <unistd.h>


Server::~Server() {
    for (int i=0; i<workers.size(); ++i) {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = INADDR_ANY;
        const int enable = 1;
        setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        setsockopt(clientSocket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
        setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(int));

        connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        SocketIO socketIo(clientSocket);
        socketIo.sendBuffer("end\n",4);
        if (!socketIo.isOpen) {
            // Print error
            std::cout << "error" << std::endl;
        }
        close(clientSocket);
    }
    for (std::thread & worker : workers) {
        worker.join();
    }
}

Server::Server(int workerThreads, int port) {
    this->port = port;
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    const int enable = 1;
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
    setsockopt(serverSocket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
    setsockopt(serverSocket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(int));

    bind(serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress));
    listen(serverSocket, 128);
    for (int i = 0; i < workerThreads; i++) {
        ActualFunction actualFunction;
        workers.push_back(std::thread([this, &actualFunction] {
            bool keepAccepting = true;
            while (keepAccepting) {
                int clientSocket = accept(serverSocket, nullptr, nullptr);
                if (clientSocket == -1) {
                    std::cout << errno << std::endl;
                }
                SocketIO socketIo(clientSocket);
                while (socketIo.isOpen) {
                    char buffer[1024] = {0};
                    socketIo.getLine(buffer, sizeof(buffer));
                    if (strncmp(buffer, "end", 3) == 0) {
                        keepAccepting = false;
                        break;
                    }
                    if (!socketIo.isOpen) {
                        break;
                    }
                    uint64_t number = strtoull(buffer, nullptr, 10);
                    uint64_t result = actualFunction.compute(number);
                    char resultBuffer[1024] = {0};
                    snprintf(resultBuffer, sizeof(resultBuffer), "%llu\n", result);
                    socketIo.sendBuffer(resultBuffer, strlen(resultBuffer));
                }
                close(clientSocket);
            }
        }));
    }
}
