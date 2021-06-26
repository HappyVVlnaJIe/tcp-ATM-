#include "ATM.h"
#include <sstream>


std::pair<Request, std::vector<int>> ParseComand(std::string message) {
    std::istringstream str_stream(message);
    std::string command;
    str_stream>>command;  

    if (request_translate.find(command)==request_translate.end()) {
        throw WrongCommandError();
    }

    std::pair<Request, std::vector<int>> result;
    result.first = request_translate[command];      
    int card_number=0, funds=0;  //сделать card_number uint32_t, funds сделать некое ограничение? как проверять тогда его?
    uint16_t pin=0;

    switch (result.first)
    {
    case Request::add:
        str_stream>>funds;
        if (funds==0) {
            throw WrongCommandFormatError();
        }
        result.second.push_back(funds);
        break;

    case Request::lock:
        break;

    case Request::login:
        str_stream>>card_number>>pin;
        if (card_number==0 || pin==0) {
            throw WrongCommandFormatError();
        }
        result.second.push_back(card_number);
        result.second.push_back(pin);
        break;

    case Request::logout:
        break;

    case Request::status:
        break;

    case Request::unlock:
        break;

    case Request::withdrawal:
        str_stream>>funds;
        if (funds==0) {
            throw WrongCommandFormatError();
        }
        result.second.push_back(funds);
        break;
    }
    return result;                         //добавить проверку на конец потока
} 

ATM::ATM(std::string cash_machine_users_path, int port, int buffer_size) : Server(port, buffer_size), db(cash_machine_users_path) {}

std::string ATM::ProcessRequest(std::string message, Client& client) {
    std::pair<Request, std::vector<int>> request_data = ParseComand(message);
    if (!client.is_login&&request_data.first!=Request::login) {
        return "You need to log in";
    }

    if (!client.is_login) {
        int card_number=request_data.second[0];
        uint16_t pin = request_data.second[1];
        //std::cout<<"card_number="<<card_number<<" ,pin="<<pin<<std::endl;
        if (db.Login(card_number, pin)) {
            client.card_number=card_number;
            client.is_login=true;
            return "Complete";
        }
        else {
            return "Wrong card number or pin";
        }
    }

    if (request_data.first==Request::login&&client.is_login) {
        return "You are already logged in";
    }

    switch (request_data.first)
    {
    case Request::add:
        db.Add(client.card_number, request_data.second[0]);
        break;

    case Request::lock:
        db.Lock(client.card_number);
        break;

    case Request::status:
        return std::to_string(db.Status(client.card_number));
        break;

    case Request::unlock:
        db.Unlock(client.card_number);
        break;
    case Request::withdrawal:
        //std::cout<<"card_number="<<client.card_number<<" ,funds="<<request_data.second.size()<<std::endl;
        db.Withdrawal(client.card_number, request_data.second[0]);
        break;
    }

    return "Complete";
}