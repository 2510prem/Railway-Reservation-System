#include "RailwaySystem.h"
#include "FareCalculator.h"
#include "FileManager.h"

#include <iostream>
#include <cctype>
#include <ctime>

using namespace std;

RailwaySystem::RailwaySystem() {

    nextPNR = 1001;

    trains.emplace_back(
        12301,
        "Rajdhani Express",
        "New Delhi",
        "Mumbai",
        "16:55",
        "08:35"
    );

    trains.back().addCoach(
        Coach(1, "AC 2 Tier", 6)
    );

    trains.back().addCoach(
        Coach(2, "First AC", 6)
    );

    trains.emplace_back(
        12302,
        "Shatabdi Express",
        "New Delhi",
        "Bhopal",
        "06:00",
        "14:00"
    );

    trains.back().addCoach(
        Coach(1, "AC 3 Tier", 6)
    );

    trains.back().addCoach(
        Coach(2, "AC 3 Tier", 6)
    );

    trains.emplace_back(
        12303,
        "Intercity Express",
        "Dhanbad",
        "Ranchi",
        "07:30",
        "11:00"
    );

    trains.back().addCoach(
        Coach(1, "Sleeper", 6)
    );

    trains.back().addCoach(
        Coach(2, "Sleeper", 6)
    );


    bookings = FileManager::loadBookings(
        "data/bookings.dat"
    );


    // Restore next PNR number
    updateNextPNR();
}


void RailwaySystem::addTrain(const Train& train) {

    trains.push_back(train);
}


void RailwaySystem::displayAllTrains() const {

    cout << "\n========== AVAILABLE TRAINS ==========\n\n";

    for (const Train& train : trains) {

        train.display();
    }
}



void RailwaySystem::searchTrain() {

    int choice;

    cout << "\n========== SEARCH TRAIN ==========\n";

    cout << "1. Search by Train Number\n";
    cout << "2. Search by Train Name\n";
    cout << "3. Search by Route\n";

    cout << "\nEnter choice: ";
    cin >> choice;


    if (choice == 1) {

        int trainNumber;

        cout << "\nEnter train number: ";
        cin >> trainNumber;

        Train* train = findTrain(trainNumber);

        if (train == nullptr) {

            cout << "\nTrain not found.\n";
            return;
        }

        train->display();
    }

    else if (choice == 2) {

        string trainName;

        cout << "\nEnter train name: ";

        cin.ignore();
        getline(cin, trainName);

        bool found = false;

        for (Train& train : trains) {

            if (train.getTrainName() == trainName) {

                train.display();

                found = true;
            }
        }

        if (!found) {

            cout << "\nTrain not found.\n";
        }
    }

    else if (choice == 3) {

        string source;
        string destination;

        cout << "\nEnter source: ";
        cin >> source;

        cout << "Enter destination: ";
        cin >> destination;

        bool found = false;

        for (Train& train : trains) {

            if (train.getSource() == source &&
                train.getDestination() == destination) {

                train.display();

                found = true;
            }
        }

        if (!found) {

            cout << "\nNo trains found for this route.\n";
        }
    }


    else {

        cout << "\nInvalid choice.\n";
    }
}

void RailwaySystem::displayAllBookings() const {

    if (bookings.empty()) {

        cout << "\nNo bookings found.\n";
        return;
    }

    cout << "\n========== ALL BOOKINGS ==========\n";

    for (const Booking& booking : bookings) {

        booking.display();
    }
}


void RailwaySystem::bookTicket() {

    int trainNumber;

    cout << "\nEnter train number: ";
    cin >> trainNumber;


    Train* train = findTrain(trainNumber);

    if (train == nullptr) {

        cout << "\nTrain not found.\n";
        return;
    }


    string journeyDate;

    cout << "\nEnter journey date (DD/MM/YYYY): ";
    cin >> journeyDate;

    if (!isValidDate(journeyDate)) {

        cout << "\nInvalid journey date.\n";
        return;
    }


    if (!train->runsDaily()) {

        cout << "\nTrain does not operate on this date.\n";
        return;
    }


    train->displaySeats();


    int coachNumber;
    int seatNumber;

    cout << "\nEnter coach number: ";
    cin >> coachNumber;

    Coach* coach = train->findCoach(coachNumber);

    if (coach == nullptr) {

        cout << "\nCoach not found.\n";
        return;
    }


    cout << "\nCoach selected: "
         << coach->getCoachNumber()
         << '\n';

    coach->display();



    cout << "\nEnter seat number: ";
    cin >> seatNumber;

    Seat* seat = coach->findSeat(seatNumber);

    if (seat == nullptr) {

        cout << "\nSeat not found.\n";
        return;
    }

    if (isSeatBooked(
            trainNumber,
            coachNumber,
            seatNumber,
            journeyDate)) {

        cout << "\nThis seat is already booked for this journey date.\n";
        return;
    }


    double fare = FareCalculator::calculateFare(
        coach->getCoachType()
    );


    string name;
    int age;
    char gender;
    string phone;


    cout << "\nEnter passenger name: ";

    cin.ignore();
    getline(cin, name);


    cout << "Enter age: ";
    cin >> age;

    if (age < 1 || age > 120) {

        cout << "\nInvalid age.\n";
        return;
    }


    cout << "Enter gender (M/F/O): ";
    cin >> gender;

    if (gender != 'M' &&
        gender != 'F' &&
        gender != 'O') {

        cout << "\nInvalid gender.\n";
        return;
    }


    cout << "Enter phone number: ";
    cin >> phone;

    if (phone.length() != 10) {

        cout << "\nInvalid phone number.\n";
        return;
    }


    for (char ch : phone) {

        if (!isdigit(ch)) {

            cout << "\nInvalid phone number.\n";
            return;
        }
    }


    Passenger passenger(
        name,
        age,
        gender,
        phone
    );


    string pnr;

    do {

        pnr = "PNR" + to_string(nextPNR);

        nextPNR++;

    } while (isPNRExists(pnr));


    Booking booking(
        pnr,
        trainNumber,
        coachNumber,
        passenger,
        seat->getSeatNumber(),
        fare,
        journeyDate
    );


    bookings.push_back(booking);


    if (!FileManager::saveBooking(
            booking,
            "data/bookings.dat")) {

        cout << "\nWarning: Booking could not be saved to file.\n";
    }


    cout << "\nTicket booked successfully!\n";

    booking.display();
}


void RailwaySystem::searchBooking() const {

    string pnr;

    cout << "\nEnter PNR: ";
    cin >> pnr;


    for (const Booking& booking : bookings) {

        if (booking.getPNR() == pnr) {

            cout << "\n========== Booking Found ==========\n";

            booking.display();

            cout << "===================================\n";

            return;
        }
    }


    cout << "\nBooking not found.\n";
}



void RailwaySystem::cancelBooking() {

    string pnr;

    cout << "\nEnter PNR to cancel: ";
    cin >> pnr;


    for (Booking& booking : bookings) {

        if (booking.getPNR() == pnr) {


            if (booking.getStatus() == "Cancelled") {

                cout << "\nBooking is already cancelled.\n";
                return;
            }

            booking.cancel();

            if (!FileManager::saveAllBookings(
                    bookings,
                    "data/bookings.dat")) {

                cout << "\nWarning: Booking cancellation could not be saved.\n";
                return;
            }


            cout << "\nBooking cancelled successfully.\n";

            booking.display();

            return;
        }
    }


    cout << "\nBooking not found.\n";
}


void RailwaySystem::displaySeats() {

    int trainNumber;

    cout << "\nEnter train number: ";
    cin >> trainNumber;


    Train* train = findTrain(trainNumber);

    if (train == nullptr) {

        cout << "\nTrain not found.\n";
        return;
    }


    train->displaySeats();
}


void RailwaySystem::run() {

    int choice;

    do {

        cout << "\n====================================\n";
        cout << "       RAILWAY RESERVATION SYSTEM\n";
        cout << "====================================\n";

        cout << "1. View All Trains\n";
        cout << "2. Search Train\n";
        cout << "3. Book Ticket\n";
        cout << "4. View All Bookings\n";
        cout << "5. Search Booking\n";
        cout << "6. Cancel Booking\n";
        cout << "7. View Seat Availability\n";
        cout << "8. Exit\n";

        cout << "\nEnter your choice: ";
        cin >> choice;


        switch (choice) {

            case 1:

                displayAllTrains();
                break;


            case 2:

                searchTrain();
                break;


            case 3:

                bookTicket();
                break;


            case 4:

                displayAllBookings();
                break;


            case 5:

                searchBooking();
                break;


            case 6:

                cancelBooking();
                break;


            case 7:

                displaySeats();
                break;


            case 8:

                cout << "\nExiting the system. Goodbye!\n";
                break;


            default:

                cout << "\nInvalid choice. Please try again.\n";
        }

    } while (choice != 8);
}


Train* RailwaySystem::findTrain(int trainNumber) {

    for (Train& train : trains) {

        if (train.getTrainNumber() == trainNumber) {

            return &train;
        }
    }


    return nullptr;
}


bool RailwaySystem::isValidDate(const string& date) const {


    if (date.length() != 10) {

        return false;
    }


    if (date[2] != '/' ||
        date[5] != '/') {

        return false;
    }

    for (int i = 0; i < 10; i++) {

        if (i == 2 || i == 5) {

            continue;
        }


        if (!isdigit(date[i])) {

            return false;
        }
    }


    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));


    if (year < 2026 ||
        year > 2100) {

        return false;
    }

    if (month < 1 ||
        month > 12) {

        return false;
    }

    int daysInMonth;


    switch (month) {

        case 2:

            if ((year % 400 == 0) ||
                (year % 4 == 0 && year % 100 != 0)) {

                daysInMonth = 29;
            }

            else {

                daysInMonth = 28;
            }

            break;


        case 4:
        case 6:
        case 9:
        case 11:

            daysInMonth = 30;
            break;


        default:

            daysInMonth = 31;
    }

    if (day < 1 ||
        day > daysInMonth) {

        return false;
    }


    time_t now = time(nullptr);

    tm* today = localtime(&now);


    int currentDay = today->tm_mday;
    int currentMonth = today->tm_mon + 1;
    int currentYear = today->tm_year + 1900;

    if (year < currentYear) {

        return false;
    }


    if (year == currentYear &&
        month < currentMonth) {

        return false;
    }


    if (year == currentYear &&
        month == currentMonth &&
        day < currentDay) {

        return false;
    }


    return true;
}


bool RailwaySystem::isSeatBooked(
    int trainNumber,
    int coachNumber,
    int seatNumber,
    const string& journeyDate
) const {

    for (const Booking& booking : bookings) {

        if (booking.getTrainNumber() == trainNumber &&
            booking.getCoachNumber() == coachNumber &&
            booking.getSeatNumber() == seatNumber &&
            booking.getJourneyDate() == journeyDate &&
            booking.getStatus() != "Cancelled") {

            return true;
        }
    }


    return false;
}


bool RailwaySystem::isPNRExists(
    const string& pnr
) const {

    for (const Booking& booking : bookings) {

        if (booking.getPNR() == pnr) {

            return true;
        }
    }


    return false;
}


void RailwaySystem::updateNextPNR() {

    for (const Booking& booking : bookings) {

        string pnr = booking.getPNR();

        if (pnr.rfind("PNR", 0) != 0) {

            continue;
        }


        int number = stoi(
            pnr.substr(3)
        );


        if (number >= nextPNR) {

            nextPNR = number + 1;
        }
    }
}