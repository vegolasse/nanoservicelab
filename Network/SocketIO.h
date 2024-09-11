#pragma once

class SocketIO {
public:
    SocketIO(int socket);

    bool isOpen;
    int socket;

    void sendBuffer(char *buffer, int length);

    void getLine(char *buffer, int length);
};
