#include "Coach.h"
#include "Train.h"
#include <iostream>

using namespace std;

Coach::Coach(int coachNumber,
             string coachType,
             int numberOfSeats)
{
    this->coachNumber = coachNumber;
    this->coachType = coachType;

    for (int i = 1; i <= numberOfSeats; i++) {

        string seatType;

        if (i % 3 == 1)
            seatType = "Lower";
        else if (i % 3 == 2)
            seatType = "Middle";
        else
            seatType = "Upper";

        seats.emplace_back(i, seatType);
    }
}

void Coach::display() const {

    cout << "\nCoach Number : " << coachNumber << '\n';
    cout << "Coach Type   : " << coachType << '\n';

    cout << "-------------------------\n";

    for (const Seat& seat : seats) {
        seat.display();
    }
}

int Coach::getCoachNumber() const {
    return coachNumber;
}

string Coach::getCoachType() const {
    return coachType;
}

int Coach::getAvailableSeats() const {

    int count = 0;

    for (const Seat& seat : seats) {

        if (seat.isAvailable()) {
            count++;
        }
    }

    return count;
}

Seat* Coach::getAvailableSeat() {

    for (Seat& seat : seats) {

        if (seat.isAvailable()) {
            return &seat;
        }
    }

    return nullptr;
}

Seat* Train::getAvailableSeat() {

    for (Coach& coach : coaches) {

        Seat* seat = coach.getAvailableSeat();

        if (seat != nullptr) {
            return seat;
        }
    }

    return nullptr;
}

Seat* Coach::findSeat(int seatNumber) {

    for (Seat& seat : seats) {

        if (seat.getSeatNumber() == seatNumber) {
            return &seat;
        }
    }

    return nullptr;
}

int Coach::getTotalSeats() const {
    return seats.size();
}