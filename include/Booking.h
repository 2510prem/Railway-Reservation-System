#ifndef BOOKING_H
#define BOOKING_H

#include <string>
#include <vector>
#include "Ticket.h"

class Booking {
private:
    std::string pnr;
    int trainNumber;
    std::string journeyDate;
    std::string status;

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
    const std::string& getStatus() const;

    const std::vector<Ticket>& getTickets() const;

    int getTotalPassengers() const;
    double getTotalFare() const;

    void cancel();

    void display() const;
};

#endif