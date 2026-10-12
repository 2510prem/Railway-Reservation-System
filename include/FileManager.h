#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Booking.h"
#include "Train.h"

// Reads and writes the data files.
// All the functions are static, so they are called on the class
// itself: FileManager::loadTrains("data/trains.dat").
class FileManager {
public:
    // Reads the trains file. Lines that are not valid are skipped
    // with a warning.
    // Throws FileException if the file cannot be opened.
    static std::vector<Train> loadTrains(
        const std::string& filename
    );

    // Reads the bookings file. A missing file is not an error: it
    // just means that there are no bookings yet.
    //
    // Lines that cannot be understood are skipped with a warning and
    // returned in unreadableLines, so that they can be written back
    // to the file later instead of being lost.
    static std::vector<Booking> loadBookings(
        const std::string& filename,
        std::vector<std::string>& unreadableLines
    );

    // Adds one booking at the end of the file.
    // Throws FileException if the booking could not be written.
    static void saveBooking(
        const Booking& booking,
        const std::string& filename
    );

    // Rewrites the whole file: every booking, followed by the
    // unreadable lines that were found when the file was loaded.
    // Throws FileException if the file could not be written.
    static void saveAllBookings(
        const std::vector<Booking>& bookings,
        const std::vector<std::string>& unreadableLines,
        const std::string& filename
    );
};

#endif
