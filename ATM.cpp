// ... (previous includes)
#include "ATM.h"
#include <limits>    // For numeric_limits
#include <string>    // Required for std::string
#include <iostream>  // Required for std::cin in authenticateUser (if still used directly)

// Enumeration for main menu options
enum MenuOption {
    BALANCE_INQUIRY = 1,
    WITHDRAWAL = 2,
    DEPOSIT = 3,
    EXIT = 4
};
T
ATM::ATM()
    : userAuthenticated(false),
      currentUserAccount(""),
      currentAccountNumber(0),
      screen(), keypad(), cashDispenser(), depositSlot(), bankDatabase() {
    // Constructor initializes all components
}

void ATM::run() {
    // This method is not used in the web version
    std::cout << "Please use the web interface at http://localhost:18080" << std::endl;
}

BankDatabase& ATM::getBankDatabase() {
    return bankDatabase;
}

CashDispenser& ATM::getCashDispenser() {
    return cashDispenser;
}

void ATM::setCurrentUserAccount(const std::string& accountNum) {
    currentUserAccount = accountNum;
    try {
        currentAccountNumber = std::stoi(accountNum);
    } catch (...) {
        currentAccountNumber = 0;
    }
}

std::string ATM::getCurrentUserAccount() const {
    return currentUserAccount;
}

int ATM::getCurrentAccountNumber() const {
    return currentAccountNumber;
}

void ATM::authenticateUser() {
    screen.displayMessage("\nPlease enter your account number: ");
    // IMPORTANT ASSUMPTION: keypad.getInput() now returns std::string
    // If keypad.getInput() still returns int, you MUST convert:
    // std::string accountNumberStr = std::to_string(keypad.getInput());
    std::string accountNumberStr = keypad.getInput();

    screen.displayMessage("Enter your PIN: ");
    // IMPORTANT ASSUMPTION: keypad.getInput() now returns std::string
    // If keypad.getInput() still returns int, you MUST convert:
    // std::string pinStr = std::to_string(keypad.getInput());
    std::string pinStr = keypad.getInput();

    // Clear the input buffer if keypad.getInput() reads from std::cin directly
    // This line might be removed if keypad.getInput() handles its own input buffer
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // BankDatabase::authenticateUser must accept std::string for account number and PIN
    userAuthenticated = bankDatabase.authenticateUser(accountNumberStr, pinStr);

    if (userAuthenticated) {
        currentUserAccount = accountNumberStr; // Store the string account number
        screen.displayMessageLine("\nUser authenticated successfully.");
    } else {
        screen.displayMessageLine("Invalid account number or PIN. Please try again.");
    }
}

// -----same as before
void ATM::performTransactions() {
    // NOTE: This method is for the console version.
    // Web application logic is handled by Crow API routes.
    Transaction* currentTransaction = nullptr; // pointer to current transaction

    bool userExited = false; // user has not chosen to exit

    while (!userExited) {
        screen.displayMessageLine("\nMain Menu:");
        screen.displayMessageLine("1 - View my balance");
        screen.displayMessageLine("2 - Withdraw cash");
        screen.displayMessageLine("3 - Deposit funds");
        screen.displayMessageLine("4 - Exit\n");
        screen.displayMessage("Enter a choice: ");

        // This assumes keypad.getInput() returns an int for menu selection
        int mainMenuSelection = std::stoi(keypad.getInput());

        currentTransaction = createTransaction(mainMenuSelection);

        if (currentTransaction != nullptr) {
            currentTransaction->execute(); // Polymorphism in action!
            delete currentTransaction; // Deallocate the dynamically created transaction
            currentTransaction = nullptr; // Set to nullptr to avoid dangling pointer
        } else if (mainMenuSelection == EXIT) {
            userExited = true; // User chose to exit
        } else {
            screen.displayMessageLine("\nYou did not enter a valid selection. Please try again.");
        }
    }
}

// Returns an object of a concrete Transaction subclass
Transaction* ATM::createTransaction(int type) {
    // NOTE: This method is for the console version.
    // Web application logic is handled by Crow API routes.
    Transaction* temp = nullptr; // base class pointer

    // Get the integer index from the string account number
    // int accountIndex = bankDatabase.getAccountIndex(currentUserAccount);
    // if (accountIndex == -1) {
    //     screen.displayMessageLine("Error: Account not found for transaction creation.");
    //     return nullptr; // Cannot create transaction without a valid account index
    // }

    switch (type) {
        case BALANCE_INQUIRY:
            // Transaction constructors likely expect an int account number/index
            temp = new BalanceInquiry(getCurrentAccountNumber(), screen, bankDatabase, keypad);
            break;
        case WITHDRAWAL:
            temp = new Withdrawal(getCurrentAccountNumber(), screen, bankDatabase, keypad, cashDispenser);
            break;
        case DEPOSIT:
            temp = new Deposit(getCurrentAccountNumber(), screen, bankDatabase, keypad, depositSlot);
            break;
    }
    return temp;
}