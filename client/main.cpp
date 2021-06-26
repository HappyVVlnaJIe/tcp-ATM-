#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>

#define DEFAULT_PORT 5000
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

int main() {
    int client;
    struct sockaddr_in server_address;
    client = socket(AF_INET,SOCK_STREAM/*|SOCK_NONBLOCK*/,0);

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(DEFAULT_PORT);
    inet_pton(AF_INET,SERVER_IP,&server_address.sin_addr);

    int ret=connect(client, reinterpret_cast<struct sockaddr*>(&server_address), sizeof(server_address));
    if (ret==0) {
        std::cout<<"Connection to server: "
            <<inet_ntoa(server_address.sin_addr)
            <<" Port: "<<DEFAULT_PORT<<std::endl;
    }

    char buffer[BUFFER_SIZE];
    std::cout<<"Waiting for server..."<<std::endl;
    recv(client,buffer,BUFFER_SIZE,0);
    std::cout<<"Connection established"<<std::endl;
    std::cout<<buffer<<std::endl;
    while(true) {
        memset(buffer,0,BUFFER_SIZE);
        std::cin.getline(buffer,BUFFER_SIZE);
        send(client, buffer,BUFFER_SIZE,0); //проверить переполнение буфера
        memset(buffer,0,BUFFER_SIZE);
        recv(client,buffer,BUFFER_SIZE,0);
        std::cout<<buffer<<std::endl;
    }

	return 0;
}