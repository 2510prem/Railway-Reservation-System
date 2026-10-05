#ifndef COACH_H
#define COACH_H

#include <vector>
#include <string>
#include "Seat.h"

class Coach {
private:
    int coachNumber;
    std::string coachType;

    std::vector<Seat> seats;

public:
    Coach(int coachNumber,
          std::string coachType,
          int numberOfSeats);

    void display() const;

    int getCoachNumber() const;
    std::string getCoachType() const;

    int getTotalSeats() const;
    int getAvailableSeats() const;

    Seat* getAvailableSeat();
    Seat* findSeat(int seatNumber);

};

#endif