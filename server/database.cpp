#include "database.h"


//Server::Server(std::string cash_machine_users_path) : _cash_machine_users_path(cash_machine_users_path) {}

std::ostream& operator<<(std::ostream& os, const User& user) {
    os<<user.pin<<" "<<user.balance<<" "<<user.card_blocked;
    return os;
}

std::istream& operator>>(std::istream& os, User& user) {
    os>>user.pin>>user.balance>>user.card_blocked;
    return os;
}

//User::User(uint16_t pin, int balance, int index, bool card_blocked) : pin(pin), balance(balance), index(index), card_blocked(card_blocked) {};



Database::Database(std::string cash_machine_users_path) : _cash_machine_users_path(cash_machine_users_path) {
    std::ifstream ifs(_cash_machine_users_path);
    //std::cout<<_cash_machine_users_path<<std::endl;
    if (ifs.is_open()) {
        int card_number;
        int position=0;
        //std::cout<<"file open"<<std::endl;
        while(ifs>>card_number>>users[card_number]) {
            //std::cout<<card_number<<" "<<users[card_number]; 
            users[card_number].line_index=position;
            position++;
        }
    }
    ifs.close();
}

void Database::WriteChanges(int card_number) {
    std::ofstream ofs(_cash_machine_users_path);//сохранять индекс и переписывать только 1 строку
    /*int user_line_index = users[card_number].line_index;
    for (int curr_line_index = 0; curr_line_index < user_line_index; ++curr_line_index){
        ofs.ignore(std::numeric_limits<std::streamsize>::max(), ofs.widen('\n')); 
    }*/
    for (const auto& user : users) {
        ofs<<user.first<<" "<<user.second<<std::endl;
    }
    ofs.close();
}

bool Database::Login(int card_number, uint16_t pin){
    if (users.find(card_number)==users.end()) {
        return false;                               
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
    //std::cout<<"Withdrawal"<<std::endl;
    if (users[card_number].card_blocked) {
        throw BlockedCardError();
    }
    if (funds>users[card_number].balance) {
        throw LowBalanceError();
    }
    users[card_number].balance-=funds;
    //std::cout<<"WriteChanges"<<std::endl;
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