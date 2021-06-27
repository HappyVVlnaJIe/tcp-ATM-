#pragma once

#include <string>
#include <iostream> 
#include <vector>
#include <set>
#include <map>

enum class Message {
    CONECTED,
    DISCONNECTED,
    NOT_LOG_IN,
    ALREADY_CONNECTED,
    COMPLETE,
    ALREADY_LOG_IN
};

struct Client {
    Client(int descriptor);
    Client()=default;
    uint32_t card_number, descriptor;
    bool is_login;
};

class Server {
public:
    Server(int port=5000, int buffer_size=1024);
    void Start();
    int Send(std::string message, int descriptor);
    std::string Recv(int descriptor);
    virtual std::string ProcessRequest(std::string message, Client& client)=0;
    bool AddActiveCard(int card_number);
    void SetMessage();
    std::string ServerMessage(Message message);
private:
    //struct sockaddr_in server_address;
    int socketFD, buffer_size;
    std::vector<Client> clients;
    std::set<int> active_cards;
    std::map<Message, std::string> server_message;
};

