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
      journeyDate(journeyDate) {
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

string Booking::getStatus() const {

    bool anyWaiting = false;

    for (const Ticket& ticket : tickets) {

        if (ticket.isConfirmed()) {
            return "Confirmed";
        }

        if (ticket.isWaiting()) {
            anyWaiting = true;
        }
    }

    if (anyWaiting) {
        return "Waiting";
    }

    return "Cancelled";
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

        if (!ticket.isCancelled()) {
            total += ticket.getFare();
        }
    }

    return total;
}

void Booking::cancel() {

    for (Ticket& ticket : tickets) {
        ticket.cancel();
    }
}

// tickets.at(i) is like tickets[i], but it checks the position and
// throws std::out_of_range if it is not valid.
void Booking::cancelTicket(int ticketIndex) {
    tickets.at(ticketIndex).cancel();
}

void Booking::confirmTicket(int ticketIndex, int seatNumber) {
    tickets.at(ticketIndex).confirm(seatNumber);
}

string Booking::serialize() const {

    string text = pnr + '|'
                + to_string(trainNumber) + '|'
                + journeyDate + '|'
                + getStatus() + '|'
                + to_string(tickets.size());

    // Each Ticket knows how to serialize itself, so Booking
    // only has to join the pieces together.
    for (const Ticket& ticket : tickets) {
        text += '|' + ticket.serialize();
    }

    return text;
}

void Booking::display() const {

    cout << "\n========================================\n";
    cout << "              BOOKING DETAILS\n";
    cout << "========================================\n";

    cout << "PNR            : " << pnr << '\n';
    cout << "Train Number   : " << trainNumber << '\n';
    cout << "Journey Date   : " << journeyDate << '\n';
    cout << "Status         : " << getStatus() << '\n';
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
