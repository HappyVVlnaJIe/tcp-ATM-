#pragma once

#include "server.h"
#include "database.h"

class ATM:public Server {
    ATM(std::string cash_machine_users_path, int port=5000, int buffer_size=1024);
    std::string ProcessRequest(std::string message, Client& client) override;
private:
    Database db;
};