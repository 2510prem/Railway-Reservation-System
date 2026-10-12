#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <cctype>
#include <cstdio>
#include <iostream>
#include <limits>
#include <string>
#include "Exceptions.h"

// Functions for reading input from the keyboard safely.
// All of them throw InputClosedException when the input has ended.

// [Templates: generic function]
// A function template is a function with a "blank" type. The same
// code reads a whole number, a single letter or a word, depending on
// the type written between < >:
//
//     int number  = readValue<int>();
//     char letter = readValue<char>();
//     string word = readValue<std::string>();
//
// If the user types something that does not fit the type (for example
// "abc" when a number is expected), the function clears the error and
// asks again instead of letting the program misbehave.
template <typename T>
T readValue() {

    while (true) {

        T value;

        if (std::cin >> value) {

            // The value must be followed by a space or the end of
            // the line. This rejects input such as "12abc".
            int next = std::cin.peek();

            if (next == EOF || std::isspace(next)) {
                return value;
            }
        }

        if (std::cin.eof()) {
            throw InputClosedException();
        }

        // Reset the stream and throw away the rest of the line
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "Invalid input. Please try again: ";
    }
}


// Reads a whole number and keeps asking until it is between
// minimum and maximum (both included).
inline int readIntInRange(int minimum, int maximum) {

    while (true) {

        int value = readValue<int>();

        if (value >= minimum && value <= maximum) {
            return value;
        }

        std::cout << "Please enter a number from "
                  << minimum << " to " << maximum << ": ";
    }
}


// Reads a full line of text (it may contain spaces), skipping
// leading whitespace and removing trailing whitespace.
inline std::string readLine() {

    std::string text;

    if (!std::getline(std::cin >> std::ws, text)) {
        throw InputClosedException();
    }

    while (!text.empty() &&
           std::isspace(static_cast<unsigned char>(text.back()))) {

        text.pop_back();
    }

    return text;
}

#endif
