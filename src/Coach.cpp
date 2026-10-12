#include "Coach.h"
#include "Exceptions.h"
#include <iostream>

using namespace std;

Coach::Coach(int coachNumber)
{
    this->coachNumber = coachNumber;
}

// [OOP: Code reuse]
// Written once here in the super class and used by all four
// sub classes in CoachTypes.cpp.
void Coach::addSeats(int numberOfSeats,
                     int berthsPerBay) {

    for (int i = 1; i <= numberOfSeats; i++) {

        string seatType;

        if (berthsPerBay == 2) {

            if (i % 2 == 1)
                seatType = "Lower";
            else
                seatType = "Upper";
        }

        else {

            if (i % 3 == 1)
                seatType = "Lower";
            else if (i % 3 == 2)
                seatType = "Middle";
            else
                seatType = "Upper";
        }

        seats.emplace_back(i, seatType);
    }
}

void Coach::display() const {

    cout << "\nCoach Number : " << coachNumber << '\n';

    // [OOP: Abstraction through polymorphism]
    // Coach does not know its own type name; getCoachType() is pure
    // virtual. This call runs the version written in whichever
    // sub class the object really is.
    cout << "Coach Type   : " << getCoachType() << '\n';
    cout << "Total Seats  : " << getTotalSeats() << '\n';

    cout << "-------------------------\n";

    for (const Seat& seat : seats) {
        seat.display();
    }
}

int Coach::getCoachNumber() const {
    return coachNumber;
}

int Coach::getTotalSeats() const {
    return static_cast<int>(seats.size());
}

// [Exceptions and operator overloading]
const Seat& Coach::operator[](int seatNumber) const {

    if (seatNumber < 1 || seatNumber > getTotalSeats()) {

        throw NotFoundException(
            "Seat " + to_string(seatNumber) +
            " does not exist in Coach " + to_string(coachNumber) + "."
        );
    }

    // Seat 1 is stored at position 0
    return seats[seatNumber - 1];
}
