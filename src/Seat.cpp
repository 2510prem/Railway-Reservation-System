#include "Seat.h"
#include <iostream>

using namespace std;

Seat::Seat(int seatNumber, string seatType) {

    this->seatNumber = seatNumber;
    this->seatType = seatType;

    available = true;
}

void Seat::display() const {

    cout << "Seat Number : " << seatNumber << '\n';
    cout << "Seat Type   : " << seatType << '\n';
    cout << "Status      : "
         << (available ? "Available" : "Occupied")
         << '\n';

    cout << "-------------------------\n";
}

int Seat::getSeatNumber() const {
    return seatNumber;
}

string Seat::getSeatType() const {
    return seatType;
}

bool Seat::isAvailable() const {
    return available;
}

void Seat::book() {
    available = false;
}

void Seat::release() {
    available = true;
}