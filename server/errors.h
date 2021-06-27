#pragma once

#include<string>
#include <exception>

class BlockedCardError : public std::runtime_error{
public:
    BlockedCardError():std::runtime_error{"the card is blocked"}{};
};

class LowBalanceError : public std::runtime_error{
public:
    LowBalanceError() :std::runtime_error{"there are not enough funds on the card"}{};
};

class UnblockedCardError : public std::runtime_error {
public:
    UnblockedCardError() :std::runtime_error{"the card is unblocked"}{};
};

class WrongCommandError : public std::runtime_error {
public:
    WrongCommandError(): std::runtime_error{"no such command exists"}{};
};

class WrongCommandFormatError : public std::runtime_error {
public:
    WrongCommandFormatError(): std::runtime_error{"wrong command format"}{};
};

class WrongCardOrPinError : public std::runtime_error {
public:
    WrongCardOrPinError(): std::runtime_error{"Wrong card number or pin"}{};
};

class ClientDisconnectError : public std::runtime_error {
public:
    ClientDisconnectError(): std::runtime_error{"client disconnect"}{};
};