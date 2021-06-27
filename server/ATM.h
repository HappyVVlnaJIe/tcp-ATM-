#pragma once

#include "server.h"
#include "database.h"
#include <sstream>

class ATM:public Server {
private:
    std::map<Request, std::string(ATM::*)(std::istringstream& str_stream, int card_number)> requests_function;
    Database db;
    std::string Login(std::istringstream& str_stream, Client& client);
    std::string Status(std::istringstream& str_stream, int card_number); 
    std::string Remove(std::istringstream& str_stream, int card_number);
    std::string Add(std::istringstream& str_stream, int card_number); 
    std::string Lock(std::istringstream& str_stream, int card_number); 
    std::string Unlock(std::istringstream& str_stream, int card_number); 
    std::string ProcessRequest(std::string message, Client& client) override;

public:
    ATM(std::string cash_machine_users_path, int port=5000, int buffer_size=1024);
};