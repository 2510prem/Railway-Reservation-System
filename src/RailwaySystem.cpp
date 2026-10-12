#include "RailwaySystem.h"
#include "Displayable.h"
#include "Exceptions.h"
#include "FileManager.h"
#include "InputHelper.h"
#include "Ticket.h"

#include <array>
#include <cctype>
#include <cmath>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;


// Constants and helper functions used only inside this file.
namespace {

const string TRAINS_FILE = "data/trains.dat";
const string BOOKINGS_FILE = "data/bookings.dat";

// Most tickets allowed in one booking
const int MAX_TICKETS = 5;

// How many days before the journey a ticket can be booked
const int MAX_ADVANCE_DAYS = 120;

// [STL: array]
// std::array is an array whose size is fixed and known while
// compiling. Unlike a vector it can never grow or shrink, which
// suits lists that never change: there are always 12 months and
// always these 3 berth types.
const array<int, 12> DAYS_IN_MONTH = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

const array<string, 3> BERTH_TYPES = {
    "Lower", "Middle", "Upper"
};


// [OOP: Abstraction through polymorphism]
// This function accepts ANY object whose class implements the
// Displayable interface: a Train, a Booking, a Coach, a Ticket...
// It does not know which one it was given and does not need to.
// item.display() is a virtual call, so the display() of the real
// class runs.
void show(const Displayable& item) {

    item.display();
}


// [Templates: generic function]
// Displays every item of a vector, whatever type T the items are.
// It is called below with vector<Train> and with vector<Booking>;
// the compiler creates one version of the function for each.
template <typename T>
void showAll(const vector<T>& items) {

    for (const T& item : items) {
        show(item);
    }
}


// Key used in the journey index: one train on one date.
string makeJourneyKey(int trainNumber, const string& journeyDate) {

    return to_string(trainNumber) + "|" + journeyDate;
}


// Key used for the waiting lists: one coach of one train on one date.
string makeWaitingKey(int trainNumber,
                      const string& journeyDate,
                      int coachNumber) {

    return makeJourneyKey(trainNumber, journeyDate)
         + "|" + to_string(coachNumber);
}


// Asks for the details of one passenger and returns the Passenger.
// Keeps asking until the details are valid.
Passenger readPassenger() {

    while (true) {

        cout << "\nEnter passenger name: ";
        string name = readLine();

        cout << "Enter age: ";
        int age = readValue<int>();

        cout << "Enter gender (M/F/O): ";
        char gender = readValue<char>();

        gender = static_cast<char>(
            toupper(static_cast<unsigned char>(gender))
        );

        cout << "Enter phone number: ";
        string phone = readValue<string>();

        // [Exceptions in constructors]
        // The Passenger constructor checks the details and throws
        // InvalidInputException if something is wrong. The exception
        // is caught here, the reason is shown, and the loop asks for
        // the details again.
        try {

            return Passenger(
                name,
                age,
                gender,
                phone
            );
        }
        catch (const InvalidInputException& error) {

            cout << "\n" << error.what() << '\n';
            cout << "Please enter the details of this passenger again.\n";
        }
    }
}

}


RailwaySystem::RailwaySystem() {

    nextPNR = 1001;

    // [Exceptions: try and catch]
    // loadTrains() throws FileException when the file cannot be
    // opened. The program can still start without trains, so the
    // problem is reported and execution continues.
    try {

        trains = FileManager::loadTrains(TRAINS_FILE);

        if (trains.empty()) {

            cout << "\nWarning: " << TRAINS_FILE
                 << " contains no valid trains.\n";
        }
    }
    catch (const FileException& error) {

        cout << "\nWarning: " << error.what() << '\n';
        cout << "No trains are available.\n";
    }


    bookings = FileManager::loadBookings(
        BOOKINGS_FILE,
        unreadableLines
    );


    // Restore next PNR number
    updateNextPNR();

    rebuildIndexes();
}


void RailwaySystem::displayAllTrains() const {

    cout << "\n========== AVAILABLE TRAINS ==========\n\n";

    showAll(trains);
}



void RailwaySystem::searchTrain() {

    cout << "\n========== SEARCH TRAIN ==========\n";

    cout << "1. Search by Train Number\n";
    cout << "2. Search by Train Name\n";
    cout << "3. Search by Route\n";

    cout << "\nEnter choice: ";
    int choice = readIntInRange(1, 3);


    if (choice == 1) {

        cout << "\nEnter train number: ";
        int trainNumber = readValue<int>();

        Train* train = findTrain(trainNumber);

        if (train == nullptr) {
            throw NotFoundException("Train not found.");
        }

        cout << '\n';
        show(*train);
    }

    else if (choice == 2) {

        cout << "\nEnter train name: ";
        string trainName = readLine();

        // [OOP: Method overloading - static polymorphism]
        // Same function name as in choice 1, but called with a
        // string. The compiler picks findTrain(const string&).
        Train* train = findTrain(trainName);

        if (train == nullptr) {
            throw NotFoundException("Train not found.");
        }

        cout << '\n';
        show(*train);
    }

    else {

        cout << "\nEnter source: ";
        string source = readLine();

        cout << "Enter destination: ";
        string destination = readLine();

        bool found = false;

        cout << '\n';

        for (const Train& train : trains) {

            if (train.getSource() == source &&
                train.getDestination() == destination) {

                show(train);

                found = true;
            }
        }

        if (!found) {

            cout << "No trains found for this route.\n";
        }
    }
}


void RailwaySystem::displayAllBookings() const {

    if (bookings.empty()) {

        cout << "\nNo bookings found.\n";
        return;
    }

    cout << "\n========== ALL BOOKINGS ==========\n";

    showAll(bookings);
}


void RailwaySystem::showAvailability(
    const Train& train,
    const string& journeyDate
) const {

    // Nothing has been selected yet when the whole train is shown
    vector<Ticket> noSelection;

    cout << "\n========== SEAT AVAILABILITY ==========\n";

    cout << "Train        : "
         << train.getTrainNumber()
         << " - "
         << train.getTrainName()
         << '\n';

    cout << "Journey Date : "
         << journeyDate
         << '\n';


    for (const Coach* coach : train.getCoaches()) {

        int freeSeats = countFreeSeats(
            train.getTrainNumber(),
            *coach,
            journeyDate,
            noSelection
        );

        cout << "\nCoach "
             << coach->getCoachNumber()
             << " - "
             << coach->getCoachType()
             << " ("
             << freeSeats
             << " of "
             << coach->getTotalSeats()
             << " seats free)\n";


        for (int seatNumber = 1;
             seatNumber <= coach->getTotalSeats();
             seatNumber++) {

            // [OOP: Operator overloading]
            // (*coach)[seatNumber] uses Coach::operator[].
            const Seat& seat = (*coach)[seatNumber];

            bool booked = isSeatBooked(
                train.getTrainNumber(),
                coach->getCoachNumber(),
                seatNumber,
                journeyDate
            );

            cout << "  Seat "
                 << seatNumber
                 << " ("
                 << seat.getSeatType()
                 << ") - "
                 << (booked ? "Booked" : "Available")
                 << '\n';
        }


        int waiting = countWaiting(
            train.getTrainNumber(),
            journeyDate,
            coach->getCoachNumber()
        );

        if (waiting > 0) {

            cout << "  Waiting list: "
                 << waiting
                 << " passenger(s)\n";
        }
    }


    cout << "=======================================\n";
}


const Coach* RailwaySystem::chooseCoach(
    const Train& train,
    const string& journeyDate,
    const vector<Ticket>& selectedTickets,
    bool& joinWaitingList
) const {

    joinWaitingList = false;

    while (true) {

        cout << "\nEnter coach number: ";
        int coachNumber = readValue<int>();

        const Coach* coach = train.findCoach(coachNumber);

        if (coach == nullptr) {

            cout << "\nCoach not found. Please try again.\n";
            continue;
        }

        int freeSeats = countFreeSeats(
            train.getTrainNumber(),
            *coach,
            journeyDate,
            selectedTickets
        );

        if (freeSeats > 0) {
            return coach;
        }


        // The coach is full: offer the waiting list
        int waiting = countWaiting(
            train.getTrainNumber(),
            journeyDate,
            coachNumber
        );

        // Count the passengers of this booking who already chose
        // to wait for this coach
        for (const Ticket& ticket : selectedTickets) {

            if (ticket.isWaiting() &&
                ticket.getCoachNumber() == coachNumber) {

                waiting++;
            }
        }

        cout << "\nCoach " << coachNumber
             << " is full for this date.\n";

        cout << "Passengers already waiting: " << waiting << '\n';

        cout << "\n1. Join the waiting list\n";
        cout << "2. Choose another coach\n";

        cout << "\nEnter choice: ";

        if (readIntInRange(1, 2) == 1) {

            joinWaitingList = true;
            return coach;
        }
    }
}


int RailwaySystem::chooseSeat(
    int trainNumber,
    const Coach& coach,
    const string& journeyDate,
    const vector<Ticket>& selectedTickets
) const {

    int coachNumber = coach.getCoachNumber();

    cout << "\nSeat selection\n";
    cout << "1. Choose a seat myself\n";
    cout << "2. Automatic allocation (with berth preference)\n";

    cout << "\nEnter choice: ";
    int choice = readIntInRange(1, 2);


    // ---------- The passenger chooses the seat ----------

    if (choice == 1) {

        while (true) {

            cout << "\nEnter seat number: ";
            int seatNumber = readValue<int>();

            // [Exceptions: multiple catch statements]
            // Two different things can go wrong here, and each has
            // its own exception type and its own catch block. The
            // first catch block whose type matches is the one that
            // runs. In both cases the loop asks for the seat again.
            try {

                // [Exceptions and operator overloading]
                // Coach::operator[] throws NotFoundException when
                // the seat number does not exist.
                const Seat& seat = coach[seatNumber];

                if (isSeatBooked(trainNumber,
                                 coachNumber,
                                 seatNumber,
                                 journeyDate)) {

                    throw SeatUnavailableException(
                        "Seat " + to_string(seatNumber) +
                        " in Coach " + to_string(coachNumber) +
                        " is already booked for this date."
                    );
                }

                if (!isSeatFree(trainNumber,
                                coachNumber,
                                seatNumber,
                                journeyDate,
                                selectedTickets)) {

                    throw SeatUnavailableException(
                        "You have already selected this seat "
                        "for another passenger."
                    );
                }

                cout << "\nSelected: Seat " << seatNumber
                     << " (" << seat.getSeatType() << ")\n";

                return seatNumber;
            }
            catch (const NotFoundException& error) {

                cout << "\n" << error.what()
                     << " Please try again.\n";
            }
            catch (const SeatUnavailableException& error) {

                cout << "\n" << error.what()
                     << " Please choose another seat.\n";
            }
        }
    }


    // ---------- Automatic allocation ----------

    cout << "\nBerth preference\n";

    for (size_t i = 0; i < BERTH_TYPES.size(); i++) {

        cout << (i + 1) << ". " << BERTH_TYPES[i] << '\n';
    }

    cout << "4. No preference\n";

    cout << "\nEnter choice: ";
    int preference = readIntInRange(1, 4);

    // "" means any berth type
    string berthType;

    if (preference <= 3) {
        berthType = BERTH_TYPES[preference - 1];
    }

    int seatNumber = findFreeSeat(
        trainNumber,
        coach,
        journeyDate,
        berthType,
        selectedTickets
    );

    if (seatNumber == 0) {

        // No free berth of the preferred type. The coach is known
        // to have at least one free seat, so take any of them.
        cout << "\nNo " << berthType
             << " berth is free in this coach."
             << " Another berth has been allotted.\n";

        seatNumber = findFreeSeat(
            trainNumber,
            coach,
            journeyDate,
            "",
            selectedTickets
        );
    }

    cout << "\nAllotted: Seat " << seatNumber
         << " (" << coach[seatNumber].getSeatType() << ")\n";

    return seatNumber;
}


void RailwaySystem::bookTicket() {

    cout << "\nEnter train number: ";
    int trainNumber = readValue<int>();

    Train* train = findTrain(trainNumber);

    if (train == nullptr) {

        // [Exceptions: throwing errors]
        // There is nothing more this function can do, so it throws.
        // The exception is caught in run(), which shows the message
        // and returns to the menu.
        throw NotFoundException("Train not found.");
    }

    cout << "\nEnter journey date (DD/MM/YYYY): ";
    string journeyDate = readValue<string>();

    // Throws InvalidInputException if the date is not acceptable
    validateJourneyDate(journeyDate);

    if (!train->runsDaily()) {
        cout << "\nTrain does not operate on this date.\n";
        return;
    }

    cout << "\nEnter number of tickets (1-" << MAX_TICKETS << "): ";
    int numberOfTickets = readIntInRange(1, MAX_TICKETS);

    showAvailability(*train, journeyDate);

    vector<Ticket> selectedTickets;

    for (int i = 1; i <= numberOfTickets; i++) {

        cout << "\n========================================\n";
        cout << "          PASSENGER " << i << '\n';
        cout << "========================================\n";

        bool joinWaitingList = false;

        const Coach* coach = chooseCoach(
            *train,
            journeyDate,
            selectedTickets,
            joinWaitingList
        );

        // A waiting passenger has no seat yet (seat number 0)
        int seatNumber = 0;
        string status = "Waiting";

        if (!joinWaitingList) {

            seatNumber = chooseSeat(
                trainNumber,
                *coach,
                journeyDate,
                selectedTickets
            );

            status = "Confirmed";
        }

        Passenger passenger = readPassenger();

        // [OOP: Subtype polymorphism]
        // "coach" is a Coach*, but the object it points to is a
        // SleeperCoach, AC3TierCoach, AC2TierCoach or FirstACCoach.
        // getFare() is virtual, so the fare of that real type is
        // returned. No "if coach type is ..." checks are needed.
        double fare = coach->getFare();

        selectedTickets.push_back(
            Ticket(
                passenger,
                coach->getCoachNumber(),
                seatNumber,
                fare,
                status
            )
        );
    }


    string pnr;

    do {

        pnr = "PNR" + to_string(nextPNR);
        nextPNR++;

    } while (isPNRExists(pnr));


    Booking booking(
        pnr,
        trainNumber,
        journeyDate
    );

    int waitingCount = 0;

    for (const Ticket& ticket : selectedTickets) {

        booking.addTicket(ticket);

        if (ticket.isWaiting()) {
            waitingCount++;
        }
    }

    bookings.push_back(booking);

    rebuildIndexes();


    // [Exceptions: re-throwing]
    // If the booking cannot be saved, it must not stay in memory
    // either: the user would see a booking that is gone after a
    // restart. The catch block undoes the booking and then uses
    // "throw;" (with nothing after it) to pass the SAME exception on
    // to the next catch block up, in run(), which reports it.
    try {

        FileManager::saveBooking(
            booking,
            BOOKINGS_FILE
        );
    }
    catch (const FileException&) {

        bookings.pop_back();

        rebuildIndexes();

        throw;
    }


    cout << "\n========================================\n";
    cout << "       TICKETS BOOKED SUCCESSFULLY!\n";
    cout << "========================================\n";

    show(booking);

    if (waitingCount > 0) {

        cout << "\nNote: " << waitingCount
             << " passenger(s) are on the waiting list.\n"
             << "A seat is allotted automatically when a confirmed\n"
             << "ticket in that coach is cancelled.\n";
    }
}


void RailwaySystem::searchBooking() const {

    cout << "\nEnter PNR: ";
    string pnr = readValue<string>();

    // Throws NotFoundException if there is no such booking
    int bookingIndex = findBookingIndex(pnr);

    cout << "\n========== Booking Found ==========\n";

    show(bookings[bookingIndex]);

    cout << "===================================\n";
}



void RailwaySystem::cancelBooking() {

    cout << "\nEnter PNR to cancel: ";
    string pnr = readValue<string>();

    int bookingIndex = findBookingIndex(pnr);

    if (bookings[bookingIndex].getStatus() == "Cancelled") {

        cout << "\nBooking is already cancelled.\n";
        return;
    }

    show(bookings[bookingIndex]);

    cout << "\n1. Cancel the entire booking\n";
    cout << "2. Cancel one passenger\n";
    cout << "3. Go back\n";

    cout << "\nEnter choice: ";
    int choice = readIntInRange(1, 3);

    if (choice == 3) {
        return;
    }


    // Work out which tickets to cancel: all of them,
    // or only the one the user picks.
    int totalTickets = bookings[bookingIndex].getTotalPassengers();

    int firstTicket = 0;
    int lastTicket = totalTickets - 1;

    if (choice == 2) {

        cout << "\nEnter passenger number (1-" << totalTickets << "): ";

        firstTicket = readIntInRange(1, totalTickets) - 1;
        lastTicket = firstTicket;

        if (bookings[bookingIndex].getTickets()[firstTicket].isCancelled()) {

            cout << "\nThis passenger is already cancelled.\n";
            return;
        }
    }


    // Keep a copy of all bookings, in case the change cannot be saved
    vector<Booking> backup = bookings;

    int trainNumber = bookings[bookingIndex].getTrainNumber();
    string journeyDate = bookings[bookingIndex].getJourneyDate();


    // Cancel the tickets, remembering the seats that become free
    vector<Ticket> freedSeats;

    for (int i = firstTicket; i <= lastTicket; i++) {

        const Ticket& ticket = bookings[bookingIndex].getTickets()[i];

        if (ticket.isCancelled()) {
            continue;
        }

        if (ticket.isConfirmed()) {
            freedSeats.push_back(ticket);
        }

        bookings[bookingIndex].cancelTicket(i);
    }

    // The cancelled tickets must leave the waiting lists
    // before anybody is promoted
    rebuildIndexes();


    // Give each freed seat to the first waiting passenger, if any
    vector<string> promotions;

    for (const Ticket& freed : freedSeats) {

        string message = promoteFromWaitingList(
            trainNumber,
            journeyDate,
            freed.getCoachNumber(),
            freed.getSeatNumber()
        );

        if (!message.empty()) {
            promotions.push_back(message);
        }
    }

    rebuildIndexes();


    // [Exceptions: re-throwing]
    // If saving fails, the cancellation is undone so that memory and
    // file still agree, and the exception is passed on to run().
    try {

        FileManager::saveAllBookings(
            bookings,
            unreadableLines,
            BOOKINGS_FILE
        );
    }
    catch (const FileException&) {

        bookings = backup;

        rebuildIndexes();

        throw;
    }


    if (choice == 1) {
        cout << "\nBooking cancelled successfully.\n";
    }

    else {
        cout << "\nPassenger cancelled successfully.\n";
    }

    show(bookings[bookingIndex]);

    for (const string& message : promotions) {
        cout << "\nWaiting list update: " << message << '\n';
    }
}


string RailwaySystem::promoteFromWaitingList(
    int trainNumber,
    const string& journeyDate,
    int coachNumber,
    int seatNumber
) {

    string key = makeWaitingKey(
        trainNumber,
        journeyDate,
        coachNumber
    );

    WaitingEntry next;

    // [Templates: class templates with exception handling]
    // waitingLists[key] is a WaitingQueue<WaitingEntry>. Its
    // takeNext() function throws NotFoundException when the queue is
    // empty, which simply means that nobody is waiting for this coach.
    try {

        next = waitingLists[key].takeNext();
    }
    catch (const NotFoundException&) {

        return "";
    }

    Booking& booking = bookings[next.bookingIndex];

    booking.confirmTicket(next.ticketIndex, seatNumber);

    const Ticket& ticket = booking.getTickets()[next.ticketIndex];

    return ticket.getPassenger().getName()
         + " (" + booking.getPNR() + ")"
         + " now has Coach " + to_string(coachNumber)
         + ", Seat " + to_string(seatNumber) + ".";
}


void RailwaySystem::displaySeats() {

    cout << "\nEnter train number: ";
    int trainNumber = readValue<int>();

    Train* train = findTrain(trainNumber);

    if (train == nullptr) {
        throw NotFoundException("Train not found.");
    }

    cout << "Enter journey date (DD/MM/YYYY): ";
    string journeyDate = readValue<string>();

    validateJourneyDate(journeyDate);

    showAvailability(*train, journeyDate);
}


void RailwaySystem::run() {

    bool running = true;

    while (running) {

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

        // [Exceptions: the try-catch-throw paradigm]
        // Every menu action runs inside this try block. If anything
        // below throws - however deep inside other functions - the
        // action stops and control jumps to the matching catch block.
        // The program then carries on with the next loop round
        // instead of crashing.
        try {

            int choice = readValue<int>();

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

                    running = false;
                    break;


                default:

                    cout << "\nInvalid choice. Please try again.\n";
            }
        }

        // [Exceptions: multiple catch statements]
        // [Exceptions and inheritance]
        // The catch blocks are tried from top to bottom, and the
        // first one that matches is used. A catch block for a base
        // class also matches all of its sub classes, so the ORDER
        // MATTERS: the specific exceptions must come first and the
        // base classes last. If "catch (const RailwayException&)"
        // were written first, it would catch everything and the
        // blocks below it would never run.
        catch (const InputClosedException&) {

            cout << "\nInput closed. Exiting the system.\n";

            running = false;
        }
        catch (const InvalidInputException& error) {

            cout << "\nInvalid input: " << error.what() << '\n';
        }
        catch (const FileException& error) {

            cout << "\nFile error: " << error.what() << '\n';
            cout << "Nothing was changed.\n";
        }
        catch (const RailwayException& error) {

            // Any other exception of this project,
            // for example NotFoundException
            cout << "\n" << error.what() << '\n';
        }
        catch (const exception& error) {

            // Any exception from the standard library
            cout << "\nUnexpected error: " << error.what() << '\n';
        }
    }
}


Train* RailwaySystem::findTrain(int trainNumber) {

    for (Train& train : trains) {

        if (train.getTrainNumber() == trainNumber) {

            return &train;
        }
    }


    return nullptr;
}


Train* RailwaySystem::findTrain(const string& trainName) {

    for (Train& train : trains) {

        if (train.getTrainName() == trainName) {

            return &train;
        }
    }


    return nullptr;
}


void RailwaySystem::validateJourneyDate(const string& date) const {

    // ---------- Format: DD/MM/YYYY ----------

    bool formatValid = date.length() == 10 &&
                       date[2] == '/' &&
                       date[5] == '/';

    for (int i = 0; formatValid && i < 10; i++) {

        if (i == 2 || i == 5) {
            continue;
        }

        if (!isdigit(static_cast<unsigned char>(date[i]))) {
            formatValid = false;
        }
    }

    if (!formatValid) {

        throw InvalidInputException(
            "The journey date must be in the format DD/MM/YYYY."
        );
    }


    // ---------- A real calendar date ----------

    int day = stoi(date.substr(0, 2));
    int month = stoi(date.substr(3, 2));
    int year = stoi(date.substr(6, 4));

    if (month < 1 || month > 12) {

        throw InvalidInputException(
            "The journey date is not a real date."
        );
    }

    int daysInMonth = DAYS_IN_MONTH[month - 1];

    // February has 29 days in a leap year
    bool leapYear = (year % 400 == 0) ||
                    (year % 4 == 0 && year % 100 != 0);

    if (month == 2 && leapYear) {
        daysInMonth = 29;
    }

    if (day < 1 || day > daysInMonth) {

        throw InvalidInputException(
            "The journey date is not a real date."
        );
    }


    // ---------- Not in the past, not too far ahead ----------

    // Convert both dates to "seconds since 1970" at 12 noon,
    // then turn the difference into days.
    time_t now = time(nullptr);

    tm today = *localtime(&now);

    today.tm_hour = 12;
    today.tm_min = 0;
    today.tm_sec = 0;

    tm journey = {};

    journey.tm_mday = day;
    journey.tm_mon = month - 1;
    journey.tm_year = year - 1900;
    journey.tm_hour = 12;
    journey.tm_isdst = -1;

    time_t todayTime = mktime(&today);
    time_t journeyTime = mktime(&journey);

    // mktime() returns -1 for a year it cannot handle
    if (journeyTime == static_cast<time_t>(-1)) {

        throw InvalidInputException(
            "The journey date is not a real date."
        );
    }

    int daysAhead = static_cast<int>(
        round(difftime(journeyTime, todayTime) / 86400.0)
    );

    if (daysAhead < 0) {

        throw InvalidInputException(
            "The journey date is in the past."
        );
    }

    if (daysAhead > MAX_ADVANCE_DAYS) {

        throw InvalidInputException(
            "Tickets can be booked at most " +
            to_string(MAX_ADVANCE_DAYS) +
            " days before the journey."
        );
    }
}


bool RailwaySystem::isSeatBooked(
    int trainNumber,
    int coachNumber,
    int seatNumber,
    const string& journeyDate
) const {

    // [STL: multimap]
    // equal_range() returns all the entries that have this key, as a
    // pair of iterators: "first" points to the first matching entry
    // and "second" points just past the last one. Only the bookings
    // of this train on this date are looked at, not every booking.
    //
    // An iterator works like a pointer to an entry. For a map or
    // multimap, it->first is the key and it->second is the value
    // (here: the position of a booking).
    auto range = journeyIndex.equal_range(
        makeJourneyKey(trainNumber, journeyDate)
    );

    for (auto it = range.first; it != range.second; ++it) {

        const Booking& booking = bookings[it->second];

        for (const Ticket& ticket : booking.getTickets()) {

            if (ticket.isConfirmed() &&
                ticket.getCoachNumber() == coachNumber &&
                ticket.getSeatNumber() == seatNumber) {

                return true;
            }
        }
    }

    return false;
}


bool RailwaySystem::isSeatFree(
    int trainNumber,
    int coachNumber,
    int seatNumber,
    const string& journeyDate,
    const vector<Ticket>& selectedTickets
) const {

    if (isSeatBooked(trainNumber,
                     coachNumber,
                     seatNumber,
                     journeyDate)) {

        return false;
    }

    // Seats chosen for earlier passengers of the same booking
    for (const Ticket& ticket : selectedTickets) {

        if (ticket.isConfirmed() &&
            ticket.getCoachNumber() == coachNumber &&
            ticket.getSeatNumber() == seatNumber) {

            return false;
        }
    }

    return true;
}


int RailwaySystem::countFreeSeats(
    int trainNumber,
    const Coach& coach,
    const string& journeyDate,
    const vector<Ticket>& selectedTickets
) const {

    int count = 0;

    for (int seatNumber = 1;
         seatNumber <= coach.getTotalSeats();
         seatNumber++) {

        if (isSeatFree(trainNumber,
                       coach.getCoachNumber(),
                       seatNumber,
                       journeyDate,
                       selectedTickets)) {

            count++;
        }
    }

    return count;
}


int RailwaySystem::findFreeSeat(
    int trainNumber,
    const Coach& coach,
    const string& journeyDate,
    const string& berthType,
    const vector<Ticket>& selectedTickets
) const {

    for (int seatNumber = 1;
         seatNumber <= coach.getTotalSeats();
         seatNumber++) {

        // An empty berthType means "any berth"
        if (!berthType.empty() &&
            coach[seatNumber].getSeatType() != berthType) {

            continue;
        }

        if (isSeatFree(trainNumber,
                       coach.getCoachNumber(),
                       seatNumber,
                       journeyDate,
                       selectedTickets)) {

            return seatNumber;
        }
    }

    return 0;
}


int RailwaySystem::countWaiting(
    int trainNumber,
    const string& journeyDate,
    int coachNumber
) const {

    // [STL: map]
    // find() looks a key up and returns an iterator to the entry.
    // If the key is not in the map it returns end(), a special
    // iterator that means "not found".
    auto it = waitingLists.find(
        makeWaitingKey(trainNumber, journeyDate, coachNumber)
    );

    if (it == waitingLists.end()) {
        return 0;
    }

    return it->second.size();
}


int RailwaySystem::findBookingIndex(const string& pnr) const {

    // [STL: map]
    auto it = pnrIndex.find(pnr);

    if (it == pnrIndex.end()) {
        throw NotFoundException("Booking not found.");
    }

    return it->second;
}


bool RailwaySystem::isPNRExists(
    const string& pnr
) const {

    // count() returns how many entries have this key:
    // for a map that is 0 or 1.
    return pnrIndex.count(pnr) > 0;
}


void RailwaySystem::rebuildIndexes() {

    pnrIndex.clear();
    journeyIndex.clear();
    waitingLists.clear();

    for (size_t i = 0; i < bookings.size(); i++) {

        const Booking& booking = bookings[i];

        int bookingIndex = static_cast<int>(i);

        // [STL: map]
        // map[key] = value adds the entry, or replaces the value
        // if the key is already there.
        pnrIndex[booking.getPNR()] = bookingIndex;

        // [STL: multimap]
        // A multimap has no [] operator, because one key can have
        // many values. Entries are added with insert().
        journeyIndex.insert({
            makeJourneyKey(
                booking.getTrainNumber(),
                booking.getJourneyDate()
            ),
            bookingIndex
        });


        // Put every waiting ticket in the queue of its coach.
        // Bookings are stored in the order they were made, so the
        // passenger who booked first is first in the queue.
        const vector<Ticket>& tickets = booking.getTickets();

        for (size_t t = 0; t < tickets.size(); t++) {

            if (!tickets[t].isWaiting()) {
                continue;
            }

            string key = makeWaitingKey(
                booking.getTrainNumber(),
                booking.getJourneyDate(),
                tickets[t].getCoachNumber()
            );

            WaitingEntry entry;

            entry.bookingIndex = bookingIndex;
            entry.ticketIndex = static_cast<int>(t);

            // waitingLists[key] creates an empty queue for this
            // coach the first time the key is used.
            waitingLists[key].add(entry);
        }
    }
}


void RailwaySystem::updateNextPNR() {

    for (const Booking& booking : bookings) {

        string pnr = booking.getPNR();

        if (pnr.rfind("PNR", 0) != 0) {

            continue;
        }


        int number;

        try {

            number = stoi(
                pnr.substr(3)
            );
        }
        catch (...) {

            // "catch (...)" catches an exception of ANY type.
            // A PNR without a number is simply ignored here.
            continue;
        }


        if (number >= nextPNR) {

            nextPNR = number + 1;
        }
    }
}
