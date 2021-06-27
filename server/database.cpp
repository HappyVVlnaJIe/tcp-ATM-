#include "database.h"

std::ostream& operator<<(std::ostream& os, const User& user) {
    os<<user.pin<<" "<<user.balance<<" "<<user.card_blocked;
    return os;
}

std::istream& operator>>(std::istream& os, User& user) {
    os>>user.pin>>user.balance>>user.card_blocked;
    return os;
}

Database::Database(const std::string& cash_machine_users_path) : cash_machine_users_path(cash_machine_users_path) {
    std::ifstream ifs(cash_machine_users_path);
    if (ifs.is_open()) {
        int card_number;
        int position=0;
        while(ifs>>card_number>>users[card_number]) {}
    }
    ifs.close();
}

void Database::WriteChanges(int card_number) {
    std::ofstream ofs(cash_machine_users_path);                
    for (const auto& user : users) {
        ofs<<user.first<<" "<<user.second<<std::endl;
    }
    ofs.close();
}

bool Database::Login(int card_number, uint16_t pin){
    auto it=users.find(card_number);
    return it!=users.end()&&it->second.pin==pin;
} 

int Database::Status(int card_number){
    return users.at(card_number).balance;
}

void Database::Withdrawal(int card_number, int funds){
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    if (funds>users[card_number].balance) {
        throw LowBalanceError();
    }
    users[card_number].balance-=funds;
    this->WriteChanges(card_number);
}

void Database::Add(int card_number, int funds){
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    users[card_number].balance+=funds;
    this->WriteChanges(card_number);
}

void Database::Lock(int card_number){
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    users[card_number].card_blocked=true;
    this->WriteChanges(card_number);
}

void Database::Unlock(int card_number){
    if (!users[card_number].card_blocked) {
        throw UnblockedCardError();
    }
    users[card_number].card_blocked=false;
    this->WriteChanges(card_number);
};

std::string Database::Help() {
    return 
        "login 'card number' 'pin'- User authorization by card number and pin code\n"
        "status - Status of the card balance\n"
        "remove 'number'- Withdraw funds from the card\n"
        "add 'number'- Replenishment of the card for a certain amount\n"
        "lock - Lock the card\n"
        "unlock - Unlock the card\n";
}