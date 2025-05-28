#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <string>

class Screen {
public:
    void displayMessage(std::string message) const;
    void displayMessageLine(std::string message) const;
    void displayDollarAmount(double amount) const;
};

#endif // SCREEN_H