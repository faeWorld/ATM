#ifndef BALANCEINQUIRY_H
#define BALANCEINQUIRY_H

#include "Transaction.h"

class BalanceInquiry : public Transaction {
public:
    BalanceInquiry(int accNum, Screen& s, BankDatabase& db, Keypad& kp);
    virtual void execute() override; // Overrides pure virtual function
};

#endif // BALANCEINQUIRY_H