#pragma once

#include <string>

class Client {
public:
    Client(int port=5000, int buffer_size=1024, const char* server_ip="127.0.0.1");
    void Start();
    int Send(std::string message, int descriptor);
    std::string Recv(int descriptor);
private:
    int buffer_size;
    int socketFD;
};