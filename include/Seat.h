#ifndef SEAT_H
#define SEAT_H

#include <string>
#include "Displayable.h"

// One seat (berth) in a coach.
//
// A seat does not store whether it is booked. The same seat can be
// booked on one date and free on another, so availability is worked
// out from the bookings for a journey date (see RailwaySystem).
class Seat : public Displayable {
private:
    int seatNumber;
    std::string seatType;

public:
    Seat(int seatNumber, std::string seatType);

    void display() const override;

    int getSeatNumber() const;
    std::string getSeatType() const;
};

#endif
