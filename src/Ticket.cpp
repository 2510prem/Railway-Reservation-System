#include "Ticket.h"
#include <iostream>
#include <sstream>

using namespace std;

Ticket::Ticket(
    const Passenger& passenger,
    int coachNumber,
    int seatNumber,
    double fare,
    const string& status
)
    : passenger(passenger),
      coachNumber(coachNumber),
      seatNumber(seatNumber),
      fare(fare),
      status(status) {
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

const string& Ticket::getStatus() const {
    return status;
}

bool Ticket::isConfirmed() const {
    return status == "Confirmed";
}

bool Ticket::isWaiting() const {
    return status == "Waiting";
}

bool Ticket::isCancelled() const {
    return status == "Cancelled";
}

void Ticket::cancel() {
    status = "Cancelled";
}

void Ticket::confirm(int seatNumber) {

    this->seatNumber = seatNumber;

    status = "Confirmed";
}

string Ticket::serialize() const {

    ostringstream text;

    text << passenger.getName() << '|'
         << passenger.getAge() << '|'
         << passenger.getGender() << '|'
         << passenger.getPhone() << '|'
         << coachNumber << '|'
         << seatNumber << '|'
         << fare << '|'
         << status;

    return text.str();
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

    // A waiting passenger has no seat yet
    if (seatNumber == 0) {
        cout << "\nSeat      : Not allotted";
    }

    else {
        cout << "\nSeat      : " << seatNumber;
    }

    cout << "\nStatus    : "
         << status;

    cout << "\nFare      : ₹"
         << fare << '\n';
}
