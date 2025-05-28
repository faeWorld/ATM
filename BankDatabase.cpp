#include "BankDatabase.h"
#include <fstream>
#include <sstream> // For stringstream
#include <limits>  // For std::numeric_limits
#include <iostream>

BankDatabase::BankDatabase() {
    loadAccounts(); // Load accounts when BankDatabase is created
}

void BankDatabase::loadAccounts() {
    std::ifstream inFile(filename);
    if (inFile.is_open()) {
        std::string line;
        while (std::getline(inFile, line)) {
            std::stringstream ss(line);
            int accNum, pin;
            double availBal, totalBal;
            std::string name; // <--- NEW: For reading the name

            // Read numerical data
            if (ss >> accNum && ss.ignore() && // Read accNum, then ignore comma
                ss >> pin && ss.ignore() &&    // Read pin, then ignore comma
                ss >> availBal && ss.ignore() && // Read availBal, then ignore comma
                ss >> totalBal && ss.ignore(std::numeric_limits<std::streamsize>::max(), ',') // Read totalBal, then ignore the comma and any spaces after it until the name
               )
            {
                // Read the rest of the line as the name (which might contain spaces)
                std::getline(ss, name); // <--- NEW: Read the rest of the line for the name
                accounts.emplace_back(accNum, pin, availBal, totalBal, name);
            } else {
                std::cerr << "Warning: Skipping malformed line in " << filename << ": " << line << std::endl;
            }
        }
        inFile.close();
    } else {
        // If file doesn't exist or can't be opened, create some dummy accounts
        std::cerr << "Info: accounts.txt not found. Creating default accounts.\n";
        // <--- NEW: Add user names to default accounts
        accounts.emplace_back(12345, 54321, 1000.0, 1200.0, "John Doe");
        accounts.emplace_back(98765, 56789, 200.0, 200.0, "Jane Smith");
        saveAccounts(); // Save these initial accounts
    }
}

void BankDatabase::saveAccounts() {
    std::ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& acc : accounts) {
            outFile << acc.getAccountNumber() << ","
                    << acc.getPin() << ","
                    << acc.getAvailableBalance() << ","
                    << acc.getTotalBalance() << "," // <--- NEW: Add comma before name
                    << acc.getUserName() << std::endl; // <--- NEW: Save the user name
        }
        outFile.close();
    } else {
        std::cerr << "Error: Unable to save account data to " << filename << std::endl;
    }
}

Account* BankDatabase::getAccount(int accountNumber) {
    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == accountNumber) {
            return &acc;
        }
    }
    return nullptr; // Account not found
}

bool BankDatabase::authenticateUser(const std::string& accountNumber, const std::string& pin) {
    try {
        int accNum = std::stoi(accountNumber);
        int pinNum = std::stoi(pin);
        
        Account* userAccount = getAccount(accNum);
        if (userAccount != nullptr) {
            return userAccount->validatePIN(pinNum);
        }
    } catch (...) {
        // Invalid number format
    }
    return false;
}

double BankDatabase::getAvailableBalance(int accountNumber) {
    Account* userAccount = getAccount(accountNumber);
    if (userAccount != nullptr) {
        return userAccount->getAvailableBalance();
    }
    return 0.0; // Account not found, return 0
}

double BankDatabase::getTotalBalance(int accountNumber) {
    Account* userAccount = getAccount(accountNumber);
    if (userAccount != nullptr) {
        return userAccount->getTotalBalance();
    }
    return 0.0; // Account not found, return 0
}

void BankDatabase::credit(int accountNumber, double amount) {
    Account* userAccount = getAccount(accountNumber);
    if (userAccount != nullptr) {
        userAccount->credit(amount);
        saveAccounts(); // Save changes after credit
    }
}

void BankDatabase::debit(int accountNumber, double amount) {
    Account* userAccount = getAccount(accountNumber);
    if (userAccount != nullptr) {
        userAccount->debit(amount);
        saveAccounts(); // Save changes after debit
    }
}