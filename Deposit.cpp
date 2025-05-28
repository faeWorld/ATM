#include "Deposit.h"

Deposit::Deposit(int accNum, Screen& s, BankDatabase& db, Keypad& kp, DepositSlot& ds)
    : Transaction(accNum, s, db, kp), depositSlot(ds), amount(0.0) {}

void Deposit::execute() {
    BankDatabase& bankDatabase = getBankDatabase();
    Screen& screen = getScreen();
    Keypad& keypad = getKeypad();

    screen.displayMessage("\nPlease enter the deposit amount in CENTS (or 0 to cancel): ");
    int input = std::stoi(keypad.getInput());

    if (input == 0) {
        screen.displayMessageLine("\nCanceling transaction...");
        return;
    }

    // Convert cents to dollars
    amount = static_cast<double>(input) / 100.0;

    screen.displayMessage("\nPlease insert your deposit envelope containing ");
    screen.displayDollarAmount(amount);
    screen.displayMessageLine(" and press enter.");

    // Simulate deposit slot receiving envelope
    if (depositSlot.isEnvelopeReceived()) {
        screen.displayMessageLine("\nYour envelope has been received.");
        bankDatabase.credit(getAccountNumber(), amount);
        screen.displayMessageLine("\nYour deposit has been successfully processed.");
    } else {
        screen.displayMessageLine("\nNo envelope received. Transaction canceled.");
    }
}