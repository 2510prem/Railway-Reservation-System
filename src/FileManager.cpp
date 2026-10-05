#include "FileManager.h"

#include <fstream>
#include <sstream>

using namespace std;

bool FileManager::initializeFile(const string& filename) {

    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        return false;
    }

    file.close();

    return true;
}

bool FileManager::saveBooking(const Booking& booking,
                              const string& filename) {

    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        return false;
    }

    const Passenger& passenger = booking.getPassenger();

    file << booking.getPNR() << '|'
         << booking.getTrainNumber() << '|'
         << booking.getCoachNumber() << '|'
         << passenger.getName() << '|'
         << passenger.getAge() << '|'
         << passenger.getGender() << '|'
         << passenger.getPhone() << '|'
         << booking.getSeatNumber() << '|'
         << booking.getFare() << '|'
         << booking.getJourneyDate() << '|'
         << booking.getStatus()
         << '\n';

    file.close();

    return true;
}

vector<Booking> FileManager::loadBookings(
    const string& filename) {

    vector<Booking> loadedBookings;

    ifstream file(filename);

    if (!file.is_open()) {
        return loadedBookings;
    }

    string line;

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        stringstream ss(line);

        string pnr;
        string trainNumber;
        string coachNumber;
        string name;
        string age;
        string gender;
        string phone;
        string seatNumber;
        string fare;
        string journeyDate;
        string status;

        getline(ss, pnr, '|');
        getline(ss, trainNumber, '|');
        getline(ss, coachNumber, '|');
        getline(ss, name, '|');
        getline(ss, age, '|');
        getline(ss, gender, '|');
        getline(ss, phone, '|');
        getline(ss, seatNumber, '|');
        getline(ss, fare, '|');
        getline(ss, journeyDate, '|');
        getline(ss, status, '|');

        Passenger passenger(
            name,
            stoi(age),
            gender[0],
            phone
        );

        Booking booking(
            pnr,
            stoi(trainNumber),
            stoi(coachNumber),
            passenger,
            stoi(seatNumber),
            stod(fare),
            journeyDate
        );

        if (status == "Cancelled") {
            booking.cancel();
        }

        loadedBookings.push_back(booking);
    }

    file.close();

    return loadedBookings;
}

bool FileManager::saveAllBookings(
    const vector<Booking>& bookings,
    const string& filename) {

    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    for (const Booking& booking : bookings) {

        const Passenger& passenger = booking.getPassenger();

        file << booking.getPNR() << '|'
             << booking.getTrainNumber() << '|'
             << booking.getCoachNumber() << '|'
             << passenger.getName() << '|'
             << passenger.getAge() << '|'
             << passenger.getGender() << '|'
             << passenger.getPhone() << '|'
             << booking.getSeatNumber() << '|'
             << booking.getFare() << '|'
             << booking.getJourneyDate() << '|'
             << booking.getStatus()
             << '\n';
    }

    file.close();

    return true;
}