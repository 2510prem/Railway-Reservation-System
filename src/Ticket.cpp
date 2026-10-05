#include "Ticket.h"
#include <iostream>

using namespace std;

Ticket::Ticket(
    const Passenger& passenger,
    int coachNumber,
    int seatNumber,
    double fare
)
    : passenger(passenger),
      coachNumber(coachNumber),
      seatNumber(seatNumber),
      fare(fare) {
}

const Passenger& Ticket::getPassenger() const {
    return passenger;
}

int Ticket::getCoachNumber() const {
    return coachNumber;
}

int Ticket::getSeatNumber() const {
    return seatNumber;
}

double Ticket::getFare() const {
    return fare;
}

void Ticket::display() const {

    cout << "\nPassenger : "
         << passenger.getName();

    cout << "\nAge       : "
         << passenger.getAge();

    cout << "\nGender    : "
         << passenger.getGender();

    cout << "\nPhone     : "
         << passenger.getPhone();

    cout << "\nCoach     : "
         << coachNumber;

    cout << "\nSeat      : "
         << seatNumber;

    cout << "\nFare      : ₹"
         << fare << '\n';
}