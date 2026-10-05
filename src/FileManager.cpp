#include "FileManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


bool FileManager::initializeFile(const string& filename) {

    ifstream file(filename);

    if (file.good()) {
        return true;
    }

    ofstream newFile(filename);

    return newFile.good();
}


bool FileManager::saveBooking(
    const Booking& booking,
    const string& filename
) {

    ofstream file(filename, ios::app);

    if (!file.is_open()) {
        return false;
    }


    file << booking.getPNR() << '|';
    file << booking.getTrainNumber() << '|';
    file << booking.getJourneyDate() << '|';
    file << booking.getStatus() << '|';
    file << booking.getTotalPassengers();

    for (const Ticket& ticket : booking.getTickets()) {

        const Passenger& passenger = ticket.getPassenger();

        file << '|'
             << passenger.getName() << '|'
             << passenger.getAge() << '|'
             << passenger.getGender() << '|'
             << passenger.getPhone() << '|'
             << ticket.getCoachNumber() << '|'
             << ticket.getSeatNumber() << '|'
             << ticket.getFare();
    }

    file << '\n';

    return true;
}


vector<Booking> FileManager::loadBookings(
    const string& filename
) {

    vector<Booking> bookings;

    ifstream file(filename);

    if (!file.is_open()) {
        return bookings;
    }

    string line;

    while (getline(file, line)) {

        if (line.empty()) {
            continue;
        }

        vector<string> fields;
        string field;

        stringstream ss(line);

        while (getline(ss, field, '|')) {
            fields.push_back(field);
        }


        if (fields.size() == 11) {

            try {

                string pnr = fields[0];

                int trainNumber = stoi(fields[1]);
                int coachNumber = stoi(fields[2]);

                string name = fields[3];
                int age = stoi(fields[4]);

                char gender = fields[5][0];

                string phone = fields[6];

                int seatNumber = stoi(fields[7]);
                double fare = stod(fields[8]);

                string journeyDate = fields[9];
                string status = fields[10];

                Passenger passenger(
                    name,
                    age,
                    gender,
                    phone
                );

                Ticket ticket(
                    passenger,
                    coachNumber,
                    seatNumber,
                    fare
                );

                Booking booking(
                    pnr,
                    trainNumber,
                    journeyDate
                );

                booking.addTicket(ticket);

                if (status == "Cancelled") {
                    booking.cancel();
                }

                bookings.push_back(booking);

            }
            catch (...) {

                cerr << "Warning: Invalid old booking record skipped.\n";
            }

            continue;
        }


        if (fields.size() < 5) {
            cerr << "Warning: Invalid booking record skipped.\n";
            continue;
        }

        try {

            string pnr = fields[0];

            int trainNumber = stoi(fields[1]);

            string journeyDate = fields[2];

            string status = fields[3];

            int ticketCount = stoi(fields[4]);


            int expectedFields = 5 + ticketCount * 7;

            if (ticketCount < 1 ||
                ticketCount > 5 ||
                static_cast<int>(fields.size()) != expectedFields) {

                cerr << "Warning: Invalid ticket count in booking "
                     << pnr << ".\n";

                continue;
            }


            Booking booking(
                pnr,
                trainNumber,
                journeyDate
            );


            int index = 5;

            for (int i = 0; i < ticketCount; i++) {

                string name = fields[index++];
                int age = stoi(fields[index++]);

                char gender = fields[index++][0];

                string phone = fields[index++];

                int coachNumber = stoi(fields[index++]);
                int seatNumber = stoi(fields[index++]);

                double fare = stod(fields[index++]);


                Passenger passenger(
                    name,
                    age,
                    gender,
                    phone
                );


                Ticket ticket(
                    passenger,
                    coachNumber,
                    seatNumber,
                    fare
                );


                booking.addTicket(ticket);
            }


            if (status == "Cancelled") {
                booking.cancel();
            }


            bookings.push_back(booking);

        }
        catch (...) {

            cerr << "Warning: Invalid booking record skipped.\n";
        }
    }

    return bookings;
}


bool FileManager::saveAllBookings(
    const vector<Booking>& bookings,
    const string& filename
) {

    ofstream file(filename);

    if (!file.is_open()) {
        return false;
    }


    for (const Booking& booking : bookings) {

        file << booking.getPNR() << '|';
        file << booking.getTrainNumber() << '|';
        file << booking.getJourneyDate() << '|';
        file << booking.getStatus() << '|';
        file << booking.getTotalPassengers();


        for (const Ticket& ticket : booking.getTickets()) {

            const Passenger& passenger =
                ticket.getPassenger();

            file << '|'
                 << passenger.getName() << '|'
                 << passenger.getAge() << '|'
                 << passenger.getGender() << '|'
                 << passenger.getPhone() << '|'
                 << ticket.getCoachNumber() << '|'
                 << ticket.getSeatNumber() << '|'
                 << ticket.getFare();
        }

        file << '\n';
    }


    return true;
}