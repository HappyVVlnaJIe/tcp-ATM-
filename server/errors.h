#pragma once

#include<string>
#include <exception>

class BlockedCardError : public std::exception{
public:
    const char* what() const noexcept {return "the card is blocked";}
};

class LowBalanceError : public std::exception{
public:
    const char* what() const noexcept {return "there are not enough funds on the card";}
};

class UnblockedCardError : public std::exception {
public:
    const char* what() const noexcept {return "the card is unblocked";}
};

class WrongComandError : public std::exception {
public:
    const char* what() const noexcept {return "no such command exists";}
};