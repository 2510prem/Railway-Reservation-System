#include "RailwaySystem.h"

#include <exception>
#include <iostream>

int main() {

    // The menu loop inside run() handles its own errors. This outer
    // try block is a last line of defence: if an exception escapes
    // from anywhere else, the program ends with a message instead of
    // crashing.
    try {

        // [OOP: Objects and Classes]
        // RailwaySystem is a class; "system" is an object of that
        // class. Creating the object runs its constructor, which
        // loads the trains and bookings from the data files.
        RailwaySystem system;

        // Calling a member function on the object.
        system.run();
    }
    catch (const std::exception& error) {

        std::cerr << "\nFatal error: " << error.what() << '\n';

        return 1;
    }

    return 0;
}
