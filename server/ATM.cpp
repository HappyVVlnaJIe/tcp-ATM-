#include "ATM.h"

std::string ATM::Login(std::istringstream& str_stream, Client& client) {
    uint32_t card_number;
    uint16_t pin;
    str_stream>>card_number>>pin;
    if (card_number==0 || pin==0) {
        throw WrongCommandFormatError();
    }
    if (db.Login(card_number, pin)) {
        if (!AddActiveCard(card_number)) {
            return ServerMessage(Message::ALREADY_CONNECTED);
        }
        client.card_number=card_number;
        client.is_login=true;
    }
    else {
        throw WrongCardOrPinError();
    }
    return ServerMessage(Message::COMPLETE);
}

std::string ATM::Add(std::istringstream& str_stream, int card_number) {
    uint32_t funds=0;
    str_stream>>funds;
    if (funds==0) {
        throw WrongCommandFormatError();
    }
    db.Add(card_number, funds);
    return ServerMessage(Message::COMPLETE);
}

std::string ATM::Lock(std::istringstream& str_stream, int card_number) {
    db.Lock(card_number);
    return ServerMessage(Message::COMPLETE);
}

std::string ATM::Remove(std::istringstream& str_stream, int card_number) {
    uint32_t funds=0;
    str_stream>>funds;
    if (funds==0) {
        throw WrongCommandFormatError();
    }
    db.Withdrawal(card_number, funds);
    return ServerMessage(Message::COMPLETE);
}

std::string ATM::Status(std::istringstream& str_stream, int card_number) {
    return std::to_string(db.Status(card_number));
}

std::string ATM::Unlock(std::istringstream& str_stream, int card_number) {
    db.Unlock(card_number);
    return ServerMessage(Message::COMPLETE);
}

ATM::ATM(std::string cash_machine_users_path, int port, int buffer_size) : Server(port, buffer_size), db(cash_machine_users_path) {
    requests_function = {
        {Request::add, &ATM::Add},
        {Request::lock, &ATM::Lock},
        {Request::remove, &ATM::Remove},
        {Request::status, &ATM::Status},
        {Request::unlock, &ATM::Unlock}
    };
}

std::string ATM::ProcessRequest(std::string message, Client& client) {
    std::istringstream str_stream(message);
    std::string command;
    str_stream>>command;  

    if (request_translate.find(command)==request_translate.end()) {
        throw WrongCommandError();
    }
    
    Request current_request = request_translate[command];

    if (current_request==Request::logout) {
        return ServerMessage(Message::DISCONNECTED);
    }

    if (!client.is_login&&current_request!=Request::login) {
        return ServerMessage(Message::NOT_LOG_IN);
    }

    if (!client.is_login) {
        return this->Login(str_stream, client);
    }

    if (client.is_login&&current_request==Request::login) {
        return ServerMessage(Message::ALREADY_LOG_IN);
    }

    return (this->*requests_function[current_request])(str_stream, client.card_number);
}