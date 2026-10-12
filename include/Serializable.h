#ifndef SERIALIZABLE_H
#define SERIALIZABLE_H

#include <string>

// [OOP: Interfaces versus multiple inheritance]
// A second interface. A class that inherits from Serializable
// promises that it can turn itself into one line of text, which
// FileManager then writes to bookings.dat.
//
// Ticket and Booking inherit from BOTH Displayable and Serializable.
// That is multiple inheritance, but because both base classes are
// interfaces (no data members), none of the usual problems of
// multiple inheritance (duplicate data, ambiguous members) can occur.
class Serializable {
public:
    virtual std::string serialize() const = 0;

    virtual ~Serializable() {}
};

#endif
