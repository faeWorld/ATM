#include "BalanceInquiry.h"

BalanceInquiry::BalanceInquiry(int accNum, Screen& s, BankDatabase& db, Keypad& kp)
    : Transaction(accNum, s, db, kp) {}

void BalanceInquiry::execute() {
    BankDatabase& bankDatabase = getBankDatabase();
    Screen& screen = getScreen();

    double availableBalance = bankDatabase.getAvailableBalance(getAccountNumber());
    double totalBalance = bankDatabase.getTotalBalance(getAccountNumber());

    screen.displayMessageLine("\nBalance Information:");
    screen.displayMessage(" - Available balance: ");
    screen.displayDollarAmount(availableBalance);
    screen.displayMessageLine(""); // Newline
    screen.displayMessage(" - Total balance:     ");
    screen.displayDollarAmount(totalBalance);
    screen.displayMessageLine("\n"); // Newline
}