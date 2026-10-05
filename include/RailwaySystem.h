#ifndef RAILWAYSYSTEM_H
#define RAILWAYSYSTEM_H

#include <vector>

#include "Train.h"
#include "Booking.h"

class RailwaySystem {
private:
    std::vector<Train> trains;
    std::vector<Booking> bookings;

    int nextPNR;
    void updateNextPNR();

    bool isValidDate(const std::string& date) const;

    bool isSeatBooked(int trainNumber,
                  int coachNumber,
                  int seatNumber,
                  const std::string& journeyDate) const;

    bool isPNRExists(const std::string& pnr) const;

public:
    RailwaySystem();

    void addTrain(const Train& train);
    void displayAllTrains() const;
    void searchTrain();

    Train* findTrain(int trainNumber);

    void bookTicket();
    void displayAllBookings() const;
    void searchBooking() const;
    void cancelBooking();
    void displaySeats();

    void run();
};

#endif