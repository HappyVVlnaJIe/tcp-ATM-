#pragma once

#include <map>
#include <vector>
#include <fstream>
#include "errors.h"
#include <iostream>

enum class Request {
    login, //Авторизация пользователя по номеру карты и пин коду
    status, //Запрос состояния счёта пользователя
    remove, //Запрос на снятие средств со счёта
    add, //Пополнение средств на определённую сумму
    lock, //Блокировка карты 
    unlock, //Разблокировка карты
    help, //список команд
    logout //выход
};

static std::map<std::string, Request> request_translate = {
    {"login", Request::login},
    {"status", Request::status},
    {"remove", Request::remove},
    {"add", Request::add},
    {"lock", Request::lock},
    {"unlock", Request::unlock},
    {"help", Request::help},
    {"logout", Request::logout},
};

struct User
{
    uint16_t pin; 
    int balance;
    bool card_blocked;
};

class Database {
public:
    Database(const std::string& cash_machine_users_path);
    bool Login(int card_number, uint16_t pin);
    int Status(int card_number);
    void Withdrawal(int card_number, int funds);
    void Add(int card_number, int funds);
    void Lock(int card_number);
    void Unlock(int card_number);
    std::string Help();

private:
    void WriteChanges(int index);
    std::map<int, User> users;
    std::string cash_machine_users_path;
};

std::ostream& operator<<(std::ostream& os, const User& user);


std::istream& operator>>(std::istream& os, User& user);
