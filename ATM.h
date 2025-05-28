#ifndef ATM_H
#define ATM_H

#include "Screen.h"
#include "Keypad.h"
#include "CashDispenser.h"
#include "DepositSlot.h"
#include "BankDatabase.h"
#include "Transaction.h" // For polymorphism
#include "BalanceInquiry.h"
#include "Withdrawal.h"
#include "Deposit.h"

// Ensure string is included for std::string
#include <string>

class ATM {
private:
    bool userAuthenticated; // True if user is currently authenticated
    // Changed to std::string to match setCurrentUserAccount/getCurrentUserAccount methods
    std::string currentUserAccount; 
    int currentAccountNumber; 
    Screen screen;
    Keypad keypad;
    CashDispenser cashDispenser;
    DepositSlot depositSlot;
    BankDatabase bankDatabase;

    // These methods are for the console version and will not be directly used by web routes
    void authenticateUser();
    void performTransactions();
    Transaction* createTransaction(int type); // Polymorphic creation

public:
    ATM(); // Constructor
    void run(); // Main function to run the ATM simulation (for console, not web)

    // Public getters for accessing internal components from main.cpp (Crow routes)
    BankDatabase& getBankDatabase();
    CashDispenser& getCashDispenser();

    // Methods to manage the currently authenticated user account number (as a string)
    void setCurrentUserAccount(const std::string& accountNum);
    std::string getCurrentUserAccount() const;
    int getCurrentAccountNumber() const;
};

#endif // ATM_H