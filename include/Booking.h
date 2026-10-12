#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>
#include "Ticket.h"
#include "Displayable.h"
#include "Serializable.h"

// A reservation made in one go: one PNR, one train, one journey date
// and 1 to 5 tickets (one per passenger).
//
// [OOP: Multiple inheritance]
// Like Ticket, Booking inherits from two interfaces.
class Booking : public Displayable, public Serializable {
private:
    std::string pnr;
    int trainNumber;
    std::string journeyDate;

    // [STL: vector]
    // std::vector is a resizable array. It keeps the tickets in the
    // order they were added and gives fast access by position
    // (tickets[0] is the first passenger).
    std::vector<Ticket> tickets;

public:
    Booking(
        const std::string& pnr,
        int trainNumber,
        const std::string& journeyDate
    );

    void addTicket(const Ticket& ticket);

    const std::string& getPNR() const;
    int getTrainNumber() const;
    const std::string& getJourneyDate() const;

    // The status of the booking is worked out from its tickets:
    //   "Cancelled" - every ticket is cancelled
    //   "Confirmed" - at least one passenger has a seat
    //   "Waiting"   - nobody has a seat yet; all are on the waiting list
    std::string getStatus() const;

    const std::vector<Ticket>& getTickets() const;

    int getTotalPassengers() const;

    // Total fare of the tickets that are not cancelled.
    double getTotalFare() const;

    // Cancels every ticket in the booking.
    void cancel();

    // Cancels one ticket. ticketIndex starts at 0.
    void cancelTicket(int ticketIndex);

    // Gives a seat to a waiting ticket. ticketIndex starts at 0.
    void confirmTicket(int ticketIndex, int seatNumber);

    void display() const override;

    // PNR|Train|Date|Status|TicketCount|<ticket>|<ticket>...
    std::string serialize() const override;
};

#endif
