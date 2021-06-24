#pragma once

#include <string>
#include <iostream> 
#include <vector>

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
};

