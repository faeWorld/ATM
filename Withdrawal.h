#ifndef WITHDRAWAL_H
#define WITHDRAWAL_H

#include "Transaction.h"

class Withdrawal : protected Transaction {
private:
    double amount; // Amount to withdraw

public:
    Withdrawal(int accNum, Screen& s, BankDatabase& db, Keypad& kp);
    virtual void execute() override; // Overrides pure virtual function
};

#endif // WITHDRAWAL_H