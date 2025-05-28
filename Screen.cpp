#include "Screen.h"
#include<iomanip>

void Screen::displayMessage(std::string message) const {
    std::cout << message;
}

void Screen::displayMessageLine(std::string message) const {
    std::cout << message << std::endl;
}

void Screen::displayDollarAmount(double amount) const {
    std::cout << "$" << std::fixed << std::setprecision(2) << amount;
}