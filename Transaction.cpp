#include "Transaction.h"

Transaction::Transaction(int accNum, Screen& s, BankDatabase& db, Keypad& kp)
    : accountNumber(accNum), screen(s), bankDatabase(db), keypad(kp) {}

int Transaction::getAccountNumber() const {
    return accountNumber;
}

Screen& Transaction::getScreen() const {
    return screen;
}

BankDatabase& Transaction::getBankDatabase() const {
    return bankDatabase;
}

Keypad& Transaction::getKeypad() const {
    return keypad;
}
//base class
