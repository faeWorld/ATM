#ifndef CASHDISPENSER_H
#define CASHDISPENSER_H

class CashDispenser {
private:
    int count; // number of 20-dollar bills remaining

public:
    CashDispenser(); // Constructor initializes count

    void dispenseCash(double amount);
    bool isSufficientCashAvailable(double amount) const;
};

#endif // CASHDISPENSER_H