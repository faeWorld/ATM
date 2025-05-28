#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "Screen.h"
#include "BankDatabase.h"
#include "Keypad.h"

class Transaction {
private:
    int accountNumber; // Current user's account number
    Screen& screen; // Reference to the ATM's screen
    BankDatabase& bankDatabase; // Reference to the ATM's bank database
    Keypad& keypad; // Reference to the ATM's keypad

public:
    // Constructor
    Transaction(int accNum, Screen& s, BankDatabase& db, Keypad& kp);

    // Virtual destructor - THIS IS THE FIX!
    virtual ~Transaction() = default; // Or {} if you prefer explicit empty body

    // Getters
    int getAccountNumber() const;
    Screen& getScreen() const;
    BankDatabase& getBankDatabase() const;
    Keypad& getKeypad() const;

    // Pure virtual function (abstract method)
    virtual void execute() = 0; // Must be implemented by derived classes
};

#endif // TRANSACTION_H