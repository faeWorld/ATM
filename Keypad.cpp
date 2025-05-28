#include "Keypad.h"
#include <string>
#include <iostream>

std::string Keypad::getInput() const {
    std::string input;
    std::getline(std::cin, input);
    return input;
}