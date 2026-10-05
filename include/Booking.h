#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include "Passenger.h"

class Booking {
private:
    std::string pnr;
    int trainNumber;
    int CoachNumber;
    Passenger passenger;

    int seatNumber;
    double fare;
    std::string journeyDate;
    std::string status;

public:
    Booking(std::string pnr,
            int trainNumber,
            int CoachNumber,
            Passenger passenger,
            int seatNumber,
            double fare,
            std::string journeyDate);

    void display() const;
    void cancel();

    std::string getPNR() const;
    int getTrainNumber() const;
    int getCoachNumber() const;
    int getSeatNumber() const;
    double getFare() const;
    std::string getJourneyDate() const;
    std::string getStatus() const;
    
    const Passenger& getPassenger() const;

};

#endif