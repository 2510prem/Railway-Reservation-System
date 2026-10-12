#ifndef TICKET_H
#define TICKET_H

#include <string>
#include "Passenger.h"
#include "Displayable.h"
#include "Serializable.h"

// One passenger's ticket inside a booking.
//
// A ticket has one of three statuses:
//   "Confirmed" - the passenger has a seat
//   "Waiting"   - the coach was full; the passenger is on the
//                 waiting list and has no seat yet (seat number 0)
//   "Cancelled" - the ticket was cancelled
//
// [OOP: Multiple inheritance]
// Ticket has TWO base classes, so this is multiple inheritance.
// Both are interfaces: a Ticket can be displayed on screen and
// can be turned into text for saving to a file.
class Ticket : public Displayable, public Serializable {
private:
    Passenger passenger;
    int coachNumber;
    int seatNumber;
    double fare;
    std::string status;

public:
    Ticket(
        const Passenger& passenger,
        int coachNumber,
        int seatNumber,
        double fare,
        const std::string& status = "Confirmed"
    );

    const Passenger& getPassenger() const;
    int getCoachNumber() const;
    int getSeatNumber() const;
    double getFare() const;
    const std::string& getStatus() const;

    bool isConfirmed() const;
    bool isWaiting() const;
    bool isCancelled() const;

    // Marks the ticket as cancelled.
    void cancel();

    // Gives a waiting passenger a seat.
    void confirm(int seatNumber);

    void display() const override;

    // Name|Age|Gender|Phone|Coach|Seat|Fare|Status
    std::string serialize() const override;

};

#endif
