#ifndef SEAT_H
#define SEAT_H

#include <string>

class Seat {
private:
    int seatNumber;
    std::string seatType;
    bool available;

public:
    Seat(int seatNumber, std::string seatType);

    void display() const;

    int getSeatNumber() const;
    std::string getSeatType() const;
    bool isAvailable() const;

    void book();
    void release();
};

#endif