#include "ATM.h"
#include <sstream>

ATM::ATM(std::string cash_machine_users_path, int port=5000, int buffer_size=1024) : Server(port, buffer_size), db(cash_machine_users_path) {}

std::string ATM::ProcessRequest(std::string message, Client& client) {
    std::istringstream str_stream(message);
    std::string comand;
    str_stream>>comand;
    if (request_translate.find(comand)==request_translate.end()) {
        throw WrongComandError();
    }

    Request current_request = request_translate[comand];

    if (current_request==Request::logout) {
        
    }

    if (!client.is_login&&current_request!=Request::login) {

    }

    switch (current_request)
    {
    case Request::add:
        break;

    case Request::lock:
        break;

    case Request::status:
        break;

    case Request::unlock:
        break;

    case Request::withdrawal:
        break;

    }

}