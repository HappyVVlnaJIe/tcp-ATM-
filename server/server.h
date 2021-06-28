#pragma once

#include <string>
#include <iostream> 
#include <list>
#include <set>
#include <map>
#include "errors.h"

enum class Message {
    CONNECTED,
    DISCONNECTED,
    NOT_LOG_IN,
    ALREADY_CONNECTED,
    COMPLETE,
    ALREADY_LOG_IN
};

struct Client {
    Client(int descriptor);
    int card_number;
    int descriptor;
    bool is_login;
};

class Server {
public:
    Server(int port=5000, int buffer_size=1024);
    void Start();
    virtual std::string ProcessRequest(std::string message, Client& client)=0;
    bool AddActiveCard(int card_number);
    void SetMessage();
    std::string ServerMessage(Message message);
private:
    void AddClient();
    int Send(std::string message, int descriptor);
    std::string Recv(int descriptor);
    int socketFD, buffer_size;
    std::list<Client> clients;
    std::set<int> active_cards;
    std::map<Message, std::string> server_message;
};

