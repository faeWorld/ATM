#ifndef DEPOSIT_H
#define DEPOSIT_H

#include "Transaction.h"
#include "DepositSlot.h"

class Deposit : public Transaction {
private:
    double amount; // Amount to deposit
    DepositSlot& depositSlot; // Reference to the ATM's deposit slot

public:
    Deposit(int accNum, Screen& s, BankDatabase& db, Keypad& kp, DepositSlot& ds);
    virtual void execute() override; // Overrides pure virtual function
};

#endif // DEPOSIT_H