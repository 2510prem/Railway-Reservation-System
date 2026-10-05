#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include "Coach.h"

class Train {
private:
    int trainNumber;
    std::string trainName;
    std::string source;
    std::string destination;

    std::string departureTime;
    std::string arrivalTime;

    int availableSeats;

    std::vector<Coach> coaches;

public:
    Train(int trainNumber,
          std::string trainName,
          std::string source,
          std::string destination,
          std::string departureTime,
          std::string arrivalTime);

    void addCoach(const Coach& coach);

    void display() const;
    void displaySeats() const;

    int getTrainNumber() const;
    std::string getTrainName() const;
    std::string getSource() const;
    std::string getDestination() const;

    int getTotalSeats() const;
    int getAvailableSeats() const;

    Seat* getAvailableSeat();
    Seat* findSeat(int seatNumber);

    Coach* findCoach(int coachNumber);

    bool runsDaily() const;
};

#endif