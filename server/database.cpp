#include "database.h"

//Server::Server(std::string cash_machine_users_path) : _cash_machine_users_path(cash_machine_users_path) {}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os<<user.pin<<" "<<user.balance<<" "<<user.card_blocked<<std::endl;
    return os;
}

std::istream& operator>>(std::istream& os, User& user) {
    os>>user.pin>>user.balance>>user.card_blocked;
    return os;
}

//User::User(uint16_t pin, int balance, int index, bool card_blocked) : pin(pin), balance(balance), index(index), card_blocked(card_blocked) {};



Database::Database(std::string cash_machine_users_path) {
    _cash_machine_users_path = cash_machine_users_path;
    std::ifstream ifs(_cash_machine_users_path);
    if (ifs.is_open()) {
        int card_number;
        int position=ifs.tellg();
        while(ifs>>card_number>>users[card_number]) { 
            users[card_number].index=position;
            position=ifs.tellg();
        }
    }
    ifs.close();
}

void Database::WriteChanges(int card_number) {
    std::ofstream ofs(_cash_machine_users_path);//сохранять индекс и переписывать только 1 строку
    
    ofs.close();
}

bool Database::Login(int card_number, uint16_t pin){
    if (users.find(card_number)==users.end()) {
        return false;                               //мб лучше throw?
    }
    if (users[card_number].pin!=pin) {
        return false;
    }
    return true;
}; 

int Database::Status(int card_number){
    return users[card_number].balance;
};

void Database::Withdrawal(int card_number, int funds){
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    if (funds>users[card_number].balance) {
        throw LowBalanceError();
    }
    users[card_number].balance-=funds;
    this->WriteChanges(card_number);
};

void Database::Add(int card_number, int funds){
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    users[card_number].balance+=funds;
    this->WriteChanges(card_number);
};

void Database::Lock(int card_number){
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    users[card_number].card_blocked=true;
    this->WriteChanges(card_number);
};

void Database::Unlock(int card_number){
    if (!users[card_number].card_blocked) {
        throw UnblockedCardError();
    }
    users[card_number].card_blocked=false;
    this->WriteChanges(card_number);
};
/*
std::vector<std::string> Database::Help() {
    return {
        "login - User authorization by card number and pin code",
        "status - Status of the card balance",
        "withdrawal 'number'- Withdraw funds from the card",
        "add 'number'- Replenishment of the card for a certain amount",
        "lock - Lock the card",
        "unlock - Unlock the card",
        };
};*/