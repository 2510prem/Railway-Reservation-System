#ifndef TICKET_H
#define TICKET_H

#include "Passenger.h"

class Ticket {
private:
    Passenger passenger;
    int coachNumber;
    int seatNumber;
    double fare;

public:
    Ticket(
        const Passenger& passenger,
        int coachNumber,
        int seatNumber,
        double fare
    );

    const Passenger& getPassenger() const;
    int getCoachNumber() const;
    int getSeatNumber() const;
    double getFare() const;

    void display() const;

};

#endif