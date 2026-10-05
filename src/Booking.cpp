#include "Booking.h"
#include <iostream>

using namespace std;

Booking::Booking(string pnr,
                 int trainNumber,
                 int CoachNumber,
                 Passenger passenger,
                 int seatNumber,
                 double fare,
                string journeyDate)
    : pnr(pnr),
      trainNumber(trainNumber),
      CoachNumber(CoachNumber),
      passenger(passenger),
      seatNumber(seatNumber),
      fare(fare),
      journeyDate(journeyDate)
{
    status = "Confirmed";
}

void Booking::display() const {

    cout << "\n========== BOOKING DETAILS ==========\n";

    cout << "PNR          : " << pnr << '\n';
    cout << "Train Number : " << trainNumber << '\n';
    cout << "Coach Number : " << CoachNumber << '\n';
    cout << "Seat Number  : " << seatNumber << '\n';
    cout << "Journey Date : " << journeyDate << '\n';
    cout << "Fare         : ₹" << fare << '\n';
    cout << "Status       : " << status << '\n';

    cout << "\nPassenger Details:\n";
    passenger.display();

    cout << "=====================================\n";
}

string Booking::getPNR() const {
    return pnr;
}

int Booking::getTrainNumber() const {
    return trainNumber;
}

int Booking::getCoachNumber() const {
    return CoachNumber;
}

int Booking::getSeatNumber() const {
    return seatNumber;
}

double Booking::getFare() const {
    return fare;
}

string Booking::getStatus() const {
    return status;
}

void Booking::cancel() {
    status = "Cancelled";
}

string Booking::getJourneyDate() const {
    return journeyDate;
}

const Passenger& Booking::getPassenger() const {
    return passenger;
}