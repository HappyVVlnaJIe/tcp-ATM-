#include "database.h"

//Server::Server(std::string cash_machine_users_path) : _cash_machine_users_path(cash_machine_users_path) {}

Database::Database(std::string cash_machine_users_path) {
    _cash_machine_users_path = cash_machine_users_path;
    std::ifstream ifs(_cash_machine_users_path);
    if (ifs.is_open()) {
        int card_number;
        while(ifs>>card_number>>users[card_number].pin>>users[card_number].balance) { //переопределить ввод и вывод, в воде индекс добавить
            
        }
    }
    ifs.close();
}

void Database::WriteChanges() {
    std::ofstream ofs(_cash_machine_users_path);//сохранять индекс и переписывать только 1 строку
    if (ofs.is_open()) {
        for (const auto& [card_number, user] : users) {
            ofs<<card_number<<' '<<user.pin<<' '<<user.balance<<' '<<user.card_blocked<<std::endl;
        }
    }
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
        return "the card is blocked";
    }
    if (funds>users[card_number].balance) {
        return "there are not enough funds on the card";
    }
    users[card_number].balance-=funds;
    this->WriteChanges();
    return "funds were debited";
};

std::string Database::Add(int card_number, int funds){
    if (users[card_number].card_blocked) {
        return "the card is blocked";
    }
    users[card_number].balance+=funds;
    this->WriteChanges();
    return "your balance is replenished";
};

std::string Database::Lock(int card_number){
    if (users[card_number].card_blocked) {
        return "the card is already blocked ";
    }
    users[card_number].card_blocked=true;
    this->WriteChanges();
    return "card is blocked";
};

std::string Database::Unlock(int card_number){
    if (!users[card_number].card_blocked) {
        return "the card is already unblocked";
    }
    users[card_number].card_blocked=false;
    this->WriteChanges();
    return "card is unblocked";
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