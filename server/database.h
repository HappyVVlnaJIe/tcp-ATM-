#pragma once

#include <map>
#include <vector>
#include <fstream>
#include "errors.h"

enum class Request {
    login, //Авторизация пользователя по номеру карты и пин коду
    status, //Запрос состояния счёта пользователя
    withdrawal, //Запрос на снятие средств со счёта
    add, //Пополнение средств на определённую сумму
    lock, //Блокировка карты 
    unlock, //Разблокировка карты
    //help, //список команд
    logout //выход
};

static std::map<std::string, Request> request_translate = {
    {"login", Request::login},
    {"status", Request::status},
    {"withdrawal", Request::withdrawal},
    {"add", Request::add},
    {"lock", Request::lock},
    {"unlock", Request::unlock},
    //{"help", Request::help},
    {"logout", Request::logout},
};

struct User
{
    //User(uint16_t pin, int balance, int index, bool card_blocked=false);
    uint16_t pin; //сделать конст, emplace
    int balance, index;
    bool card_blocked;
    /* data */
};

class Database {
public:
    Database(std::string cash_machine_users_path);
    bool Login(int card_number, uint16_t pin);
    int Status(int card_number);
    void Withdrawal(int card_number, int funds);
    void Add(int card_number, int funds);
    void Lock(int card_number);
    void Unlock(int card_number);
    //std::vector<std::string> Help();
    void WriteChanges(int index);//убрать в private
private:
    std::map<int, User> users;
    std::string _cash_machine_users_path;
};

std::ostream& operator<<(std::ostream& os, const User& user);


std::istream& operator>>(std::istream& os, const User& user);
//как записывать изменения? не переписывать же весь файл