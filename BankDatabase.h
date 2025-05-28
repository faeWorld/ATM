#ifndef BANKDATABASE_H
#define BANKDATABASE_H

#include "Account.h"
#include <vector>
#include <string>

class BankDatabase {
private:
    std::vector<Account> accounts;
    const std::string filename = "accounts.txt"; // File to store account data

    void loadAccounts(); // Loads account data from file
    void saveAccounts(); // Saves account data to file

public:
    BankDatabase(); // Constructor loads accounts

    Account* getAccount(int accountNumber); // Returns pointer to account if found
    bool authenticateUser(const std::string& accountNumber, const std::string& pin);
    double getAvailableBalance(int accountNumber);
    double getTotalBalance(int accountNumber);
    void credit(int accountNumber, double amount);
    void debit(int accountNumber, double amount);
};

#endif // BANKDATABASE_Hs