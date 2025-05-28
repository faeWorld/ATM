#include "Account.h"

// <--- NEW: Add name parameter and initializer
Account::Account(int accNum, int p, double availBal, double totalBal, std::string name)
    : accountNumber(accNum), pin(p), availableBalance(availBal), totalBalance(totalBal), userName(name) {}

int Account::getAccountNumber() const {
    return accountNumber;
}

int Account::getPin() const {
    return pin;
}

double Account::getAvailableBalance() const {
    return availableBalance;
}

double Account::getTotalBalance() const {
    return totalBalance;
}

std::string Account::getUserName() const { // <--- NEW: Getter implementation
    return userName;
}

bool Account::validatePIN(int userPIN) const {
    return (userPIN == pin);
}

void Account::credit(double amount) {
    totalBalance += amount;
    availableBalance += amount; // For simplicity
}

void Account::debit(double amount) {
    availableBalance -= amount;
    totalBalance -= amount;
}