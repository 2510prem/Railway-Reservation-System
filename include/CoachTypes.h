#ifndef COACHTYPES_H
#define COACHTYPES_H

#include <string>
#include "Coach.h"

// [OOP: Inheritance - sub and super classes]
// Each class below is a SUB class (derived class) of Coach.
// "class SleeperCoach : public Coach" reads as
// "a SleeperCoach IS A Coach".
//
// This is SINGLE inheritance: each sub class has exactly one
// super class.
//
// [OOP: Code reuse]
// The sub classes do not repeat any of the seat handling code.
// display(), findSeat(), getTotalSeats() and the rest are all
// inherited from Coach. Each sub class only states what is
// different about it: its name, its fare and its berth layout.
//
// [OOP: Method overriding - subtype polymorphism]
// Each sub class OVERRIDES the virtual functions of Coach: it
// provides its own version with the same name and parameters.
// The word "override" asks the compiler to check that a matching
// virtual function really exists in the super class.

// Sleeper class: bays of 3 berths (Lower, Middle, Upper).
class SleeperCoach : public Coach {
public:
    SleeperCoach(int coachNumber, int numberOfSeats);

    std::string getCoachType() const override;
    double getFare() const override;
    Coach* clone() const override;
};

// AC 3 Tier: bays of 3 berths (Lower, Middle, Upper).
class AC3TierCoach : public Coach {
public:
    AC3TierCoach(int coachNumber, int numberOfSeats);

    std::string getCoachType() const override;
    double getFare() const override;
    Coach* clone() const override;
};

// AC 2 Tier: bays of 2 berths (Lower, Upper).
class AC2TierCoach : public Coach {
public:
    AC2TierCoach(int coachNumber, int numberOfSeats);

    std::string getCoachType() const override;
    double getFare() const override;
    Coach* clone() const override;
};

// First AC: bays of 2 berths (Lower, Upper).
class FirstACCoach : public Coach {
public:
    FirstACCoach(int coachNumber, int numberOfSeats);

    std::string getCoachType() const override;
    double getFare() const override;
    Coach* clone() const override;
};

// Creates a coach of the sub class named by coachType
// ("Sleeper", "AC 3 Tier", "AC 2 Tier" or "First AC") using "new".
// Returns nullptr when the coach type is not known.
// The caller becomes responsible for deleting the coach.
Coach* createCoach(const std::string& coachType,
                   int coachNumber,
                   int numberOfSeats);

#endif
