#include "FileManager.h"
#include "CoachTypes.h"
#include "Exceptions.h"

#include <cstdio>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;


// Helper functions used only inside this file.
namespace {

// Splits "a|b|c" into {"a", "b", "c"}.
vector<string> splitFields(const string& line) {

    vector<string> fields;
    string field;

    stringstream ss(line);

    while (getline(ss, field, '|')) {
        fields.push_back(field);
    }

    return fields;
}


// [Exceptions: catching and throwing errors]
// stoi() throws std::invalid_argument when the text is not a number.
// That message ("stoi") would mean nothing to the user, so the
// exception is caught here and a FileException with a clear message
// is thrown in its place.
int toInt(const string& text, const string& fieldName) {

    try {

        return stoi(text);
    }
    catch (const exception&) {

        throw FileException(
            fieldName + " is not a valid number: \"" + text + "\""
        );
    }
}


double toDouble(const string& text, const string& fieldName) {

    try {

        return stod(text);
    }
    catch (const exception&) {

        throw FileException(
            fieldName + " is not a valid number: \"" + text + "\""
        );
    }
}


// Builds a Train from one line of trains.dat.
// Throws FileException if the line is not valid.
//
// Format:
// TrainNumber|Name|Source|Destination|Departure|Arrival|CoachCount|
// CoachNumber|CoachType|Seats|...
Train parseTrain(const string& line) {

    vector<string> fields = splitFields(line);

    if (fields.size() < 7) {
        throw FileException("Too few fields.");
    }

    int trainNumber = toInt(fields[0], "Train number");
    int coachCount = toInt(fields[6], "Coach count");

    if (coachCount < 1 ||
        static_cast<int>(fields.size()) != 7 + coachCount * 3) {

        throw FileException("Wrong number of coach fields.");
    }

    Train train(
        trainNumber,
        fields[1],
        fields[2],
        fields[3],
        fields[4],
        fields[5]
    );

    int index = 7;

    for (int i = 0; i < coachCount; i++) {

        int coachNumber = toInt(fields[index++], "Coach number");

        string coachType = fields[index++];

        int numberOfSeats = toInt(fields[index++], "Seat count");

        // If any of the "throw" statements below runs, this function
        // ends at once. The local object "train" is then destroyed
        // automatically, and its destructor deletes the coaches added
        // so far. No memory is leaked.

        if (numberOfSeats < 1) {
            throw FileException("A coach must have at least 1 seat.");
        }

        if (train.findCoach(coachNumber) != nullptr) {
            throw FileException("Duplicate coach number.");
        }

        // [OOP: Inheritance and subtyping]
        // createCoach() returns a Coach* that points to the
        // sub class matching the coach type in the file.
        Coach* coach = createCoach(
            coachType,
            coachNumber,
            numberOfSeats
        );

        if (coach == nullptr) {

            throw FileException(
                "Unknown coach type: \"" + coachType + "\""
            );
        }

        // The train now owns the coach and will delete it.
        train.addCoach(coach);
    }

    return train;
}


// Builds a Ticket from 7 or 8 fields, starting at fields[index]:
// Name|Age|Gender|Phone|Coach|Seat|Fare|Status
//
// Older files have no Status field. In that case hasStatus is false
// and defaultStatus is used.
Ticket parseTicket(const vector<string>& fields,
                   int index,
                   bool hasStatus,
                   const string& defaultStatus) {

    string name = fields[index];
    int age = toInt(fields[index + 1], "Age");

    if (fields[index + 2].length() != 1) {
        throw FileException("Gender must be a single letter.");
    }

    char gender = fields[index + 2][0];

    string phone = fields[index + 3];

    int coachNumber = toInt(fields[index + 4], "Coach number");
    int seatNumber = toInt(fields[index + 5], "Seat number");

    double fare = toDouble(fields[index + 6], "Fare");

    string status = defaultStatus;

    if (hasStatus) {
        status = fields[index + 7];
    }

    if (status != "Confirmed" &&
        status != "Waiting" &&
        status != "Cancelled") {

        throw FileException(
            "Unknown ticket status: \"" + status + "\""
        );
    }

    // [Exceptions in constructors]
    // The Passenger constructor throws InvalidInputException if the
    // details in the file are not valid (for example age 0).
    Passenger passenger(
        name,
        age,
        gender,
        phone
    );

    return Ticket(
        passenger,
        coachNumber,
        seatNumber,
        fare,
        status
    );
}


// Builds a Booking from one line of bookings.dat.
// Throws an exception if the line is not valid.
//
// Three formats are understood:
//
// Current:
//   PNR|Train|Date|Status|TicketCount|  then 8 fields per ticket
// Older (tickets had no status of their own):
//   PNR|Train|Date|Status|TicketCount|  then 7 fields per ticket
// Oldest (one passenger per booking, always 11 fields):
//   PNR|Train|Coach|Name|Age|Gender|Phone|Seat|Fare|Date|Status
Booking parseBooking(const string& line) {

    vector<string> fields = splitFields(line);

    if (fields.size() < 5) {
        throw FileException("Too few fields.");
    }

    if (fields[0].empty()) {
        throw FileException("The PNR is missing.");
    }


    // Oldest format
    if (fields.size() == 11) {

        string status = "Confirmed";

        if (fields[10] == "Cancelled") {
            status = "Cancelled";
        }

        // Put the ticket fields in the order parseTicket() expects
        vector<string> ticketFields = {
            fields[3],
            fields[4],
            fields[5],
            fields[6],
            fields[2],
            fields[7],
            fields[8]
        };

        Booking booking(
            fields[0],
            toInt(fields[1], "Train number"),
            fields[9]
        );

        booking.addTicket(
            parseTicket(ticketFields, 0, false, status)
        );

        return booking;
    }


    int ticketCount = toInt(fields[4], "Ticket count");

    if (ticketCount < 1 || ticketCount > 5) {
        throw FileException("The ticket count must be from 1 to 5.");
    }

    int fieldCount = static_cast<int>(fields.size());

    bool hasStatus;

    if (fieldCount == 5 + ticketCount * 8) {
        hasStatus = true;
    }

    else if (fieldCount == 5 + ticketCount * 7) {
        hasStatus = false;
    }

    else {
        throw FileException("Wrong number of ticket fields.");
    }

    int fieldsPerTicket = hasStatus ? 8 : 7;

    // Used only for the older format, where the booking status
    // applied to all of its tickets.
    string defaultStatus = "Confirmed";

    if (fields[3] == "Cancelled") {
        defaultStatus = "Cancelled";
    }

    Booking booking(
        fields[0],
        toInt(fields[1], "Train number"),
        fields[2]
    );

    for (int i = 0; i < ticketCount; i++) {

        booking.addTicket(
            parseTicket(
                fields,
                5 + i * fieldsPerTicket,
                hasStatus,
                defaultStatus
            )
        );
    }

    return booking;
}

}


vector<Train> FileManager::loadTrains(
    const string& filename
) {

    ifstream file(filename);

    if (!file.is_open()) {

        // [Exceptions: throwing errors]
        // This function cannot do its job without the file, and it
        // does not know what the program wants to do about that.
        // It reports the problem and lets the caller decide.
        throw FileException("Could not open " + filename + ".");
    }

    vector<Train> trains;

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {

        lineNumber++;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        // Skip empty lines and comment lines
        if (line.empty() || line[0] == '#') {
            continue;
        }

        // [Exceptions and inheritance]
        // parseTrain() can throw FileException. Catching the base
        // class std::exception here covers that and any other
        // standard exception. One bad line is skipped; the rest of
        // the file is still loaded.
        try {

            Train train = parseTrain(line);

            for (const Train& existing : trains) {

                if (existing.getTrainNumber() == train.getTrainNumber()) {
                    throw FileException("Duplicate train number.");
                }
            }

            trains.push_back(train);
        }
        catch (const exception& error) {

            cerr << "Warning: " << filename
                 << " line " << lineNumber
                 << " skipped. " << error.what() << '\n';
        }
    }

    return trains;
}


vector<Booking> FileManager::loadBookings(
    const string& filename,
    vector<string>& unreadableLines
) {

    vector<Booking> bookings;

    ifstream file(filename);

    if (!file.is_open()) {
        return bookings;
    }

    string line;
    int lineNumber = 0;

    while (getline(file, line)) {

        lineNumber++;

        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }

        if (line.empty()) {
            continue;
        }

        // [Exceptions and inheritance]
        // parseBooking() can throw FileException (bad format) or
        // InvalidInputException (bad passenger details). Both are
        // sub classes of std::exception, so one catch block handles
        // them all.
        try {

            bookings.push_back(parseBooking(line));
        }
        catch (const exception& error) {

            cerr << "Warning: " << filename
                 << " line " << lineNumber
                 << " skipped. " << error.what() << '\n';

            // Keep the line so that it is not lost when the
            // file is rewritten.
            unreadableLines.push_back(line);
        }
    }

    return bookings;
}


void FileManager::saveBooking(
    const Booking& booking,
    const string& filename
) {

    // ios::app = add at the end of the file
    ofstream file(filename, ios::app);

    if (!file.is_open()) {

        throw FileException(
            "Could not open " + filename + " for writing."
        );
    }

    // [OOP: Interfaces]
    // Booking implements the Serializable interface, so it
    // produces its own line of text for the file.
    file << booking.serialize() << '\n';

    file.flush();

    if (!file) {
        throw FileException("Could not write to " + filename + ".");
    }
}


void FileManager::saveAllBookings(
    const vector<Booking>& bookings,
    const vector<string>& unreadableLines,
    const string& filename
) {

    // Write everything to a temporary file first. The real file is
    // replaced only after the new one has been written completely,
    // so a failure half way cannot destroy the existing bookings.
    string tempFilename = filename + ".tmp";

    ofstream file(tempFilename);

    if (!file.is_open()) {

        throw FileException(
            "Could not open " + tempFilename + " for writing."
        );
    }

    for (const Booking& booking : bookings) {
        file << booking.serialize() << '\n';
    }

    for (const string& line : unreadableLines) {
        file << line << '\n';
    }

    file.close();

    if (!file) {

        remove(tempFilename.c_str());

        throw FileException(
            "Could not write to " + tempFilename + "."
        );
    }

    // Replace the old file with the new one
    remove(filename.c_str());

    if (rename(tempFilename.c_str(), filename.c_str()) != 0) {

        throw FileException(
            "Could not replace " + filename + "."
        );
    }
}
