#include "Train.h"
#include "Coach.h"
#include <iostream>

using namespace std;

Train::Train(int trainNumber,
             string trainName,
             string source,
             string destination,
             string departureTime,
             string arrivalTime)
{
    this->trainNumber = trainNumber;
    this->trainName = trainName;
    this->source = source;
    this->destination = destination;
    this->departureTime = departureTime;
    this->arrivalTime = arrivalTime;
}

void Train::display() const {

    cout << "Train Number   : " << trainNumber << '\n';
    cout << "Train Name     : " << trainName << '\n';
    cout << "Route          : " << source
         << " -> " << destination << '\n';

    cout << "Departure      : " << departureTime << '\n';
    cout << "Arrival        : " << arrivalTime << '\n';

    cout << "Total Seats    : " << getTotalSeats() << '\n';
    cout << "Available Seats: " << getAvailableSeats() << '\n';

    cout << "\nCoaches:\n";

    for (const Coach& coach : coaches) {

        cout << "  Coach "
             << coach.getCoachNumber()
             << " - "
             << coach.getCoachType()
             << " - Available Seats: "
             << coach.getAvailableSeats()
             << '\n';
    }

    cout << "----------------------------------\n";
}

int Train::getTrainNumber() const {
    return trainNumber;
}

string Train::getSource() const {
    return source;
}

string Train::getDestination() const {
    return destination;
}

int Train::getAvailableSeats() const {

    int count = 0;

    for (const Coach& coach : coaches) {
        count += coach.getAvailableSeats();
    }

    return count;
}

void Train::displaySeats() const {

    cout << "\n========== SEAT AVAILABILITY ==========\n";

    for (const Coach& coach : coaches) {

        cout << "\nCoach "
             << coach.getCoachNumber()
             << " (" << coach.getCoachType() << ")\n";

        cout << "Available Seats: "
             << coach.getAvailableSeats()
             << "\n";

        coach.display();
    }
}

Seat* Train::findSeat(int seatNumber) {

    for (Coach& coach : coaches) {

        Seat* seat = coach.findSeat(seatNumber);

        if (seat != nullptr) {
            return seat;
        }
    }

    return nullptr;
}

Coach* Train::findCoach(int coachNumber) {

    for (Coach& coach : coaches) {

        if (coach.getCoachNumber() == coachNumber) {
            return &coach;
        }
    }

    return nullptr;
}

int Train::getTotalSeats() const {

    int count = 0;

    for (const Coach& coach : coaches) {
        count += coach.getTotalSeats();
    }

    return count;
}

void Train::addCoach(const Coach& coach) {
    coaches.push_back(coach);
}

string Train::getTrainName() const {
    return trainName;
}

bool Train::runsDaily() const {
    return true;
}