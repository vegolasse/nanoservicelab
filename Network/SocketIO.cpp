//
// Created by Lars Sjödin on 2024-09-06.
//

#include <cstdio>
#include <cstring>
#include <sys/socket.h>
#include <cerrno>
#include <iostream>
#include "SocketIO.h"

SocketIO::SocketIO(int socket) {
    isOpen = true;
    this->socket = socket;
}

void SocketIO::sendBuffer(char* buffer, int length) {
    ssize_t totalSentBytes = 0;
    while (isOpen && totalSentBytes<length) {
        ssize_t sentBytes = send(socket, buffer + totalSentBytes, length - totalSentBytes, 0);
        if (sentBytes < 1) {
            isOpen = false;
            if (sentBytes == -1) {
                char errorBuffer[1024];
                strerror_r(errno, errorBuffer, sizeof(errorBuffer));
                std::cout << errorBuffer << std::endl;
            }
        }
        totalSentBytes += sentBytes;
    }

}

void SocketIO::getLine(char* buffer, int length) {
    ssize_t totalReceivedBytes = 0;
    while (isOpen && strnstr(buffer, "\n", totalReceivedBytes) == NULL) {
        ssize_t receivedBytes = recv(socket, buffer + totalReceivedBytes, length - totalReceivedBytes, 0);
        if (receivedBytes < 1) {
            isOpen = false;
        }
        totalReceivedBytes += receivedBytes;
    }
    if (totalReceivedBytes >= length) {
        totalReceivedBytes = length - 1;
    }
    buffer[totalReceivedBytes] = 0;

}
