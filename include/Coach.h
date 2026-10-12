#ifndef COACH_H
#define COACH_H

#include <vector>
#include <string>
#include "Seat.h"
#include "Displayable.h"

// [OOP: Inheritance - sub and super classes]
// Coach is the SUPER class (base class) for every kind of coach.
// The sub classes SleeperCoach, AC3TierCoach, AC2TierCoach and
// FirstACCoach are declared in CoachTypes.h.
//
// Coach itself inherits from the Displayable interface.
//
// [OOP: Abstract class]
// Coach has pure virtual functions (see below), which makes it an
// ABSTRACT class. No object of type Coach can be created; only
// objects of its sub classes can. Coach describes what every coach
// has in common and leaves the details to the sub classes.
class Coach : public Displayable {

// [OOP: Access specification - protected]
// "protected" sits between private and public:
//   - code outside the class cannot touch these members (like private)
//   - but sub classes of Coach can (unlike private)
// The sub classes need this to build their own seat layout.
protected:
    int coachNumber;

    std::vector<Seat> seats;

    // Only a sub class can call the constructor.
    Coach(int coachNumber);

    // Shared helper for the sub classes: creates the seats.
    // berthsPerBay is 3 (Lower/Middle/Upper) or 2 (Lower/Upper).
    void addSeats(int numberOfSeats,
                  int berthsPerBay);

public:
    // [OOP: Virtual destructor]
    // Train deletes its coaches through Coach pointers
    // ("delete coach;" where coach is a Coach*). The destructor must
    // be virtual so that the destructor of the real sub class runs
    // too. Without "virtual" only the Coach part would be destroyed.
    virtual ~Coach() {}

    // [OOP: Pure virtual functions]
    // "= 0" means Coach gives no body for these functions. Every
    // sub class MUST override them, otherwise it is abstract too.
    virtual std::string getCoachType() const = 0;
    virtual double getFare() const = 0;

    // Returns a new copy of this coach, of the same sub class.
    // Train needs this to copy its coaches without knowing their
    // real types.
    virtual Coach* clone() const = 0;

    // Overrides display() from the Displayable interface.
    void display() const override;

    // [OOP: Virtual versus non-virtual functions]
    // The functions below are NOT virtual. They work the same way for
    // every kind of coach, so the sub classes have no reason to
    // replace them. The compiler decides which function to call while
    // compiling (static binding).
    //
    // getFare() above IS virtual. Given "Coach* coach", the call
    // "coach->getFare()" is decided while the program runs, by looking
    // at the real type of the object the pointer points to (dynamic
    // binding). The same line gives 500 for a SleeperCoach and 2000
    // for a FirstACCoach.
    int getCoachNumber() const;

    int getTotalSeats() const;

    // [OOP: Operator overloading]
    // Gives the [] operator a meaning for Coach objects, so that a
    // seat can be picked like an array element: coach[3] is seat 3.
    // Seat numbers start at 1.
    //
    // [Exceptions and operator overloading]
    // An operator cannot return an error code: coach[99] has to give
    // back a Seat. When the seat does not exist, the operator throws
    // NotFoundException instead.
    const Seat& operator[](int seatNumber) const;

};

#endif
