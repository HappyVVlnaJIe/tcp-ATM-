#include "ATM.h"

int main()
{
    std::string users_file_path="../cash-machine-users.txt";
    ATM atm = ATM(users_file_path);
    atm.Start();
    return 0;
}