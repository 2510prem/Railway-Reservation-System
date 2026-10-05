#include "Booking.h"
#include <iostream>

using namespace std;

Booking::Booking(
    const string& pnr,
    int trainNumber,
    const string& journeyDate
)
    : pnr(pnr),
      trainNumber(trainNumber),
      journeyDate(journeyDate),
      status("Confirmed") {
}

void Booking::addTicket(const Ticket& ticket) {
    if (tickets.size() < 5) {
        tickets.push_back(ticket);
    }
}

const string& Booking::getPNR() const {
    return pnr;
}

int Booking::getTrainNumber() const {
    return trainNumber;
}

const string& Booking::getJourneyDate() const {
    return journeyDate;
}

const string& Booking::getStatus() const {
    return status;
}

const vector<Ticket>& Booking::getTickets() const {
    return tickets;
}

int Booking::getTotalPassengers() const {
    return static_cast<int>(tickets.size());
}

double Booking::getTotalFare() const {

    double total = 0;

    for (const Ticket& ticket : tickets) {
        total += ticket.getFare();
    }

    return total;
}

void Booking::cancel() {
    status = "Cancelled";
}

void Booking::display() const {

    cout << "\n========================================\n";
    cout << "              BOOKING DETAILS\n";
    cout << "========================================\n";

    cout << "PNR            : " << pnr << '\n';
    cout << "Train Number   : " << trainNumber << '\n';
    cout << "Journey Date   : " << journeyDate << '\n';
    cout << "Status         : " << status << '\n';
    cout << "Passengers     : " << tickets.size() << '\n';

    cout << "\n----------------------------------------\n";

    int passengerNumber = 1;

    for (const Ticket& ticket : tickets) {

        cout << "\nPassenger " << passengerNumber << '\n';
        cout << "----------------------------------------";

        ticket.display();

        passengerNumber++;
    }

    cout << "\n----------------------------------------\n";
    cout << "Total Fare     : ₹" << getTotalFare() << '\n';
    cout << "========================================\n";
}