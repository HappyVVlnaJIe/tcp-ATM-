#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include "database.h"
#include <cstring>
#include <sstream> 
#include <thread>  

struct Client {
    Client(int descriptor);
    Client()=default;
    int card_number, descriptor;
    bool is_login;
};

class Server {
public:
    Server(int port=5000, int buffer_size=1024);
    void Start();
    int Send(std::string message, int descriptor);
    std::string Recv(int descriptor);
    virtual std::string ProcessRequest(std::string message, Client& client);
private:
    int socketFD, buffer_size;
    std::vector<Client> clients;
}