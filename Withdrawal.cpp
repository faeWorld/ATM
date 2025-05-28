#include "Withdrawal.h"

Withdrawal::Withdrawal(int accNum, Screen& s, BankDatabase& db, Keypad& kp)
    : Transaction(accNum, s, db, kp), amount(0.0) {}

void Withdrawal::execute() {
    BankDatabase& bankDatabase = getBankDatabase();
    Screen& screen = getScreen();
    Keypad& keypad = getKeypad();
    int selection = 0; // user's menu selection

    const int CANCELED = 6; // constant for cancel option

    // loop while user has not chosen to cancel
    while (selection != CANCELED) {
        screen.displayMessageLine("\nWithdrawal Options:");
        screen.displayMessageLine("1 - $20");
        screen.displayMessageLine("2 - $40");
        screen.displayMessageLine("3 - $60");
        screen.displayMessageLine("4 - $100");
        screen.displayMessageLine("5 - $200");
        screen.displayMessageLine("6 - Cancel transaction");
        screen.displayMessage("\nChoose a withdrawal amount: ");

        int input = std::stoi(keypad.getInput());

        switch (input) {
            case 1: amount = 20; break;
            case 2: amount = 40; break;
            case 3: amount = 60; break;
            case 4: amount = 100; break;
            case 5: amount = 200; break;
            case CANCELED:
                selection = CANCELED;
                screen.displayMessageLine("\nCanceling transaction...");
                return; // exit from execute method
            default:
                screen.displayMessageLine("\nInvalid selection. Try again.");
                continue; // continue loop to display menu again
        }

        // Check if user has enough money in their account
        double availableBalance = bankDatabase.getAvailableBalance(getAccountNumber());

        if (amount <= availableBalance) {
            bankDatabase.debit(getAccountNumber(), amount);
            screen.displayMessageLine("\nYour cash has been dispensed. Please take your money.");
            selection = CANCELED; // Transaction complete
        } else {
            screen.displayMessageLine("\nInsufficient funds in your account. Please choose a smaller amount or cancel.");
        }
    }
}