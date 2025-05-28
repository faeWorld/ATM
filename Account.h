#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <iostream>
#include <string> // <--- Add this for std::string

class Account {
private:
    int accountNumber;
    int pin;
    double availableBalance;
    double totalBalance;
    std::string userName; // <--- NEW: User's name

public:
    // Constructor
    Account(int accNum = 0, int p = 0, double availBal = 0.0, double totalBal = 0.0, std::string name = "Default User"); // <--- NEW: Add name parameter

    // Getters
    int getAccountNumber() const;
    int getPin() const;
    double getAvailableBalance() const;
    double getTotalBalance() const;
    std::string getUserName() const; // <--- NEW: Getter for user name

    // Methods
    bool validatePIN(int userPIN) const;
    void credit(double amount);
    void debit(double amount);
};

#endif // ACCOUNT_H