#include "CashDispenser.h"
#include <iostream> // For std::cerr

CashDispenser::CashDispenser() : count(500) {} // 500 $20 bills initially

void CashDispenser::dispenseCash(double amount) {
    int billsRequired = static_cast<int>(amount / 20); // Assuming only $20 bills
    if (billsRequired <= count) {
        count -= billsRequired;
        std::cout << "\nYour cash has been dispensed. Please take your money.\n";
    } else {
        std::cerr << "Error: Not enough cash in dispenser to fulfill request.\n";
    }
}

bool CashDispenser::isSufficientCashAvailable(double amount) const {
    int billsRequired = static_cast<int>(amount / 20); // Assuming only $20 bills
    return (billsRequired <= count);
}