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

// Destructor: frees every coach that was created with "new".
// [OOP: Virtual destructor]
// "delete coach" is called on a Coach*. Because the Coach destructor
// is virtual, the destructor of the real sub class runs.
Train::~Train() {

    for (Coach* coach : coaches) {
        delete coach;
    }
}

// Copy constructor: builds a new train as a copy of another one.
// [OOP: Subtype polymorphism]
// coach->clone() is a virtual call. The train does not know (or need
// to know) which sub class each coach is; every coach copies itself.
Train::Train(const Train& other)
    : Displayable(other),
      trainNumber(other.trainNumber),
      trainName(other.trainName),
      source(other.source),
      destination(other.destination),
      departureTime(other.departureTime),
      arrivalTime(other.arrivalTime) {

    for (const Coach* coach : other.coaches) {
        coaches.push_back(coach->clone());
    }
}

// Assignment operator: makes an existing train a copy of another one.
Train& Train::operator=(const Train& other) {

    // Guard against "train = train;"
    if (this != &other) {

        // Release the coaches this train currently owns
        for (Coach* coach : coaches) {
            delete coach;
        }

        coaches.clear();

        trainNumber = other.trainNumber;
        trainName = other.trainName;
        source = other.source;
        destination = other.destination;
        departureTime = other.departureTime;
        arrivalTime = other.arrivalTime;

        for (const Coach* coach : other.coaches) {
            coaches.push_back(coach->clone());
        }
    }

    return *this;
}

void Train::display() const {

    cout << "Train Number   : " << trainNumber << '\n';
    cout << "Train Name     : " << trainName << '\n';
    cout << "Route          : " << source
         << " -> " << destination << '\n';

    cout << "Departure      : " << departureTime << '\n';
    cout << "Arrival        : " << arrivalTime << '\n';

    cout << "Total Seats    : " << getTotalSeats() << '\n';

    cout << "\nCoaches:\n";

    for (const Coach* coach : coaches) {

        cout << "  Coach "
             << coach->getCoachNumber()
             << " - "
             << coach->getCoachType()
             << " - Seats: "
             << coach->getTotalSeats()
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

const Coach* Train::findCoach(int coachNumber) const {

    for (const Coach* coach : coaches) {

        if (coach->getCoachNumber() == coachNumber) {
            return coach;
        }
    }

    return nullptr;
}

int Train::getTotalSeats() const {

    int count = 0;

    for (const Coach* coach : coaches) {
        count += coach->getTotalSeats();
    }

    return count;
}

void Train::addCoach(Coach* coach) {
    coaches.push_back(coach);
}

string Train::getTrainName() const {
    return trainName;
}

bool Train::runsDaily() const {
    return true;
}

const vector<Coach*>& Train::getCoaches() const {
    return coaches;
}
