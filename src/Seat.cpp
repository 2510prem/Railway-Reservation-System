#include "Seat.h"
#include <iostream>

using namespace std;

Seat::Seat(int seatNumber, string seatType) {

    this->seatNumber = seatNumber;
    this->seatType = seatType;
}

void Seat::display() const {

    cout << "Seat " << seatNumber
         << " (" << seatType << ")\n";
}

int Seat::getSeatNumber() const {
    return seatNumber;
}

string Seat::getSeatType() const {
    return seatType;
}
