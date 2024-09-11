#include "Client.h"
#include "SocketIO.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <print>

uint64_t Client::compute(uint64_t number) {

    SocketIO* client = connectionPool.pop();
    char sendBuffer[1024];
    snprintf(sendBuffer, sizeof(sendBuffer), "%llu\n", number);
    client->sendBuffer(sendBuffer, strlen(sendBuffer));
    char receiveBuffer[1024];
    client->getLine(receiveBuffer, 1024);
    uint64_t result = strtoull(receiveBuffer, nullptr, 10);
    connectionPool.push(client);
    return result;
}

Client::Client(int port, int connections) {
    this->port = port;
    this->connections = connections;
    for (int i=0; i<connections; ++i) {
        int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            std::cout << "Error creating socket" << std::endl;
            return;
        }
        sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_port = htons(port);
        serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
        const int enable = 1;
        setsockopt(clientSocket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));
        setsockopt(clientSocket, SOL_SOCKET, SO_REUSEPORT, &enable, sizeof(int));
        setsockopt(clientSocket, IPPROTO_TCP, TCP_NODELAY, &enable, sizeof(int));

        int connectResult = connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        if (connectResult == -1) {
            char errorBuffer[1024];
            strerror_r(errno, errorBuffer, sizeof(errorBuffer));
            std::cout << errorBuffer << std::endl;
            return;
        }

        SocketIO* socketIo = new SocketIO(clientSocket);
        connectionPool.push(socketIo);
    }
}

Client::~Client() {
    for (int i=0; i<connections; ++i) {
        SocketIO* socketIo = connectionPool.pop();
        close(socketIo->socket);
        delete socketIo;
    }
}
