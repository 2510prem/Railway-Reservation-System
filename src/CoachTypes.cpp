#include "CoachTypes.h"

using namespace std;

// [OOP: Inheritance - constructors]
// A sub class constructor must first build the super class part of
// the object. ": Coach(coachNumber)" calls the Coach constructor
// before the body below runs.
//
// addSeats() is a protected member of Coach. It cannot be called
// from outside the class, but a sub class is allowed to use it.
//
// [OOP: Method overriding - subtype polymorphism]
// getCoachType(), getFare() and clone() are the overridden
// functions. Each sub class answers in its own way.
//
// clone() uses the copy constructor of the sub class
// ("new SleeperCoach(*this)") to make an exact copy of the object.


// ---------- SleeperCoach ----------

SleeperCoach::SleeperCoach(int coachNumber, int numberOfSeats)
    : Coach(coachNumber) {

    addSeats(numberOfSeats, 3);
}

string SleeperCoach::getCoachType() const {
    return "Sleeper";
}

double SleeperCoach::getFare() const {
    return 500;
}

Coach* SleeperCoach::clone() const {
    return new SleeperCoach(*this);
}


// ---------- AC3TierCoach ----------

AC3TierCoach::AC3TierCoach(int coachNumber, int numberOfSeats)
    : Coach(coachNumber) {

    addSeats(numberOfSeats, 3);
}

string AC3TierCoach::getCoachType() const {
    return "AC 3 Tier";
}

double AC3TierCoach::getFare() const {
    return 1000;
}

Coach* AC3TierCoach::clone() const {
    return new AC3TierCoach(*this);
}


// ---------- AC2TierCoach ----------

AC2TierCoach::AC2TierCoach(int coachNumber, int numberOfSeats)
    : Coach(coachNumber) {

    addSeats(numberOfSeats, 2);
}

string AC2TierCoach::getCoachType() const {
    return "AC 2 Tier";
}

double AC2TierCoach::getFare() const {
    return 1500;
}

Coach* AC2TierCoach::clone() const {
    return new AC2TierCoach(*this);
}


// ---------- FirstACCoach ----------

FirstACCoach::FirstACCoach(int coachNumber, int numberOfSeats)
    : Coach(coachNumber) {

    addSeats(numberOfSeats, 2);
}

string FirstACCoach::getCoachType() const {
    return "First AC";
}

double FirstACCoach::getFare() const {
    return 2000;
}

Coach* FirstACCoach::clone() const {
    return new FirstACCoach(*this);
}


// [OOP: Inheritance and subtyping]
// The return type is Coach*, but the object created is one of the
// sub classes. A pointer to a super class can point to an object of
// any of its sub classes.
Coach* createCoach(const string& coachType,
                   int coachNumber,
                   int numberOfSeats) {

    if (coachType == "Sleeper") {
        return new SleeperCoach(coachNumber, numberOfSeats);
    }

    if (coachType == "AC 3 Tier") {
        return new AC3TierCoach(coachNumber, numberOfSeats);
    }

    if (coachType == "AC 2 Tier") {
        return new AC2TierCoach(coachNumber, numberOfSeats);
    }

    if (coachType == "First AC") {
        return new FirstACCoach(coachNumber, numberOfSeats);
    }

    return nullptr;
}
