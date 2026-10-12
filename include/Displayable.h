#ifndef DISPLAYABLE_H
#define DISPLAYABLE_H

// [OOP: Interfaces versus multiple inheritance]
// An INTERFACE is a class that only says WHAT a class can do,
// not HOW it does it. C++ has no "interface" keyword, so we write
// a class that contains only pure virtual functions ("= 0") and
// no data members.
//
// Any class that inherits from Displayable promises to provide a
// display() function. Train, Coach, Seat, Passenger, Ticket and
// Booking all make this promise.
class Displayable {
public:
    // "= 0" makes this a pure virtual function: it has no body here,
    // and every sub class must write its own. (Pure virtual functions
    // are a polymorphism topic.)
    virtual void display() const = 0;

    // A base class that is inherited from should have a virtual
    // destructor, so that destroying a sub class object through a
    // base class pointer works correctly.
    virtual ~Displayable() {}
};

#endif
