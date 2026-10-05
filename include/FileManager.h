#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <string>
#include <vector>
#include "Booking.h"

class FileManager {
public:
    static bool initializeFile(const std::string& filename);

    static bool saveBooking(
        const Booking& booking,
        const std::string& filename
    );

    static std::vector<Booking> loadBookings(
        const std::string& filename
    );

    static bool saveAllBookings(
        const std::vector<Booking>& bookings,
        const std::string& filename
    );
};

#endif