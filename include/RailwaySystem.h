#ifndef RAILWAYSYSTEM_H
#define RAILWAYSYSTEM_H

#include <map>
#include <string>
#include <vector>

#include "Train.h"
#include "Booking.h"
#include "WaitingQueue.h"

// One place in a waiting list: which ticket of which booking is
// waiting. Both numbers are positions (starting at 0) - the booking
// in the bookings vector, and the ticket inside that booking.
struct WaitingEntry {
    int bookingIndex;
    int ticketIndex;
};

// The main class of the program. It owns the trains and the bookings
// and contains the menu and every operation the user can perform.
class RailwaySystem {
private:
    // [STL: vector]
    // The main storage. Trains and bookings are kept in the order
    // they were loaded or created.
    std::vector<Train> trains;
    std::vector<Booking> bookings;

    // Lines of bookings.dat that could not be understood. They are
    // kept so that they can be written back unchanged.
    std::vector<std::string> unreadableLines;

    // The three containers below are INDEXES: they hold no new
    // information, only quick ways into the bookings vector. They are
    // rebuilt by rebuildIndexes() whenever the bookings change.

    // [STL: map]
    // std::map stores key -> value pairs, sorted by key, with ONE
    // value per key. Here: PNR -> position of that booking in the
    // bookings vector. Finding a booking by PNR becomes a quick
    // lookup instead of a search through every booking.
    std::map<std::string, int> pnrIndex;

    // [STL: multimap]
    // std::multimap is like map, but the same key may appear MANY
    // times. Here: "train|date" -> position of a booking. One train
    // on one date has many bookings, so a plain map would not do.
    // It is used to find which seats are taken on a journey.
    std::multimap<std::string, int> journeyIndex;

    // [STL: map] + [Templates: generic class]
    // "train|date|coach" -> the waiting list for that coach on that
    // date. WaitingQueue<WaitingEntry> is our class template with
    // T = WaitingEntry.
    std::map<std::string, WaitingQueue<WaitingEntry>> waitingLists;

    int nextPNR;
    void updateNextPNR();

    void rebuildIndexes();

    // Throws InvalidInputException if the date is not acceptable.
    void validateJourneyDate(const std::string& date) const;

    // Throws NotFoundException if there is no booking with this PNR.
    int findBookingIndex(const std::string& pnr) const;

    bool isPNRExists(const std::string& pnr) const;

    // True if a confirmed ticket holds this seat on this date.
    bool isSeatBooked(int trainNumber,
                      int coachNumber,
                      int seatNumber,
                      const std::string& journeyDate) const;

    // True if the seat is neither booked nor already chosen for an
    // earlier passenger of the booking that is being made.
    bool isSeatFree(int trainNumber,
                    int coachNumber,
                    int seatNumber,
                    const std::string& journeyDate,
                    const std::vector<Ticket>& selectedTickets) const;

    int countFreeSeats(int trainNumber,
                       const Coach& coach,
                       const std::string& journeyDate,
                       const std::vector<Ticket>& selectedTickets) const;

    // Returns the first free seat with the given berth type
    // ("Lower", "Middle" or "Upper"; "" means any type).
    // Returns 0 if there is none.
    int findFreeSeat(int trainNumber,
                     const Coach& coach,
                     const std::string& journeyDate,
                     const std::string& berthType,
                     const std::vector<Ticket>& selectedTickets) const;

    int countWaiting(int trainNumber,
                     const std::string& journeyDate,
                     int coachNumber) const;

    void showAvailability(const Train& train,
                          const std::string& journeyDate) const;

    // Steps of bookTicket()
    const Coach* chooseCoach(const Train& train,
                             const std::string& journeyDate,
                             const std::vector<Ticket>& selectedTickets,
                             bool& joinWaitingList) const;

    int chooseSeat(int trainNumber,
                   const Coach& coach,
                   const std::string& journeyDate,
                   const std::vector<Ticket>& selectedTickets) const;

    // Gives a freed seat to the first passenger on the waiting list.
    // Returns a message describing the change, or "" if nobody was
    // waiting.
    std::string promoteFromWaitingList(int trainNumber,
                                       const std::string& journeyDate,
                                       int coachNumber,
                                       int seatNumber);

public:
    RailwaySystem();

    void displayAllTrains() const;
    void searchTrain();

    // [OOP: Method overloading - static polymorphism]
    // Two functions with the same name but different parameters.
    // The compiler chooses between them by looking at the argument:
    //   findTrain(12301)              -> the int version
    //   findTrain("Rajdhani Express") -> the string version
    // The choice is made while compiling, which is why this is
    // called static (compile-time) polymorphism.
    // Both return nullptr when no train matches.
    Train* findTrain(int trainNumber);
    Train* findTrain(const std::string& trainName);

    void bookTicket();
    void displayAllBookings() const;
    void searchBooking() const;
    void cancelBooking();
    void displaySeats();

    void run();
};

#endif
