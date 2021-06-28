#include "client.h"
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <chrono>
#include <thread>

const char* disconnect_mess="Goodbye";

Client::Client(int port, int buffer_size, const char *server_ip) : buffer_size(buffer_size)
{
    struct sockaddr_in server_address;
    socketFD = socket(AF_INET, SOCK_STREAM /*|SOCK_NONBLOCK*/, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, server_ip, &server_address.sin_addr);

    int ret = connect(socketFD, reinterpret_cast<struct sockaddr *>(&server_address), sizeof(server_address));
    if (ret == 0)
    {
        std::cout << "Connection to server: "
                  << inet_ntoa(server_address.sin_addr)
                  << " Port: " << port << std::endl;
    }

    char buffer[buffer_size];
    std::cout << "Waiting for server..." << std::endl;
    recv(socketFD, buffer, buffer_size, 0);
    std::cout << "Connection established" << std::endl;
    std::cout << buffer << std::endl;
}

int Client::Send(std::string message, int descriptor) {
    return send(descriptor, message.c_str(), message.size(), 0);
}

std::string Client::Recv(int descriptor) {
    std::string message = "";
    char buffer[buffer_size];
    while(message=="") {
        while ((recv(descriptor, buffer, buffer_size, MSG_DONTWAIT)) > 0)
        {
            message += buffer;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
    return message;
}

void Client::Start()
{
    char buffer[buffer_size];
    while (true)
    {
        memset(buffer,0,buffer_size);
        std::cin.getline(buffer,buffer_size);
        Send(buffer, socketFD); 
        memset(buffer,0,buffer_size);
        std::cout<<std::endl<<Recv(socketFD)<<std::endl<<std::endl;
        if (strcmp(buffer,disconnect_mess)==0) {
            break;
        }
        std::this_thread::sleep_for (std::chrono::milliseconds(10));
    }
}