# 🚆 Railway Reservation System

A **command-line Railway Reservation System** written in **C++17** with
**Object-Oriented Programming (OOP)** and built with **CMake**.

It covers train search, group booking with PNR generation, manual and
automatic seat allocation, a waiting list, full and partial
cancellation, date-aware seat availability and file-based storage.

The source code is written to be read: every C++ concept it uses is
marked with a short explanatory comment (see
[Concept Map](#-concept-map)).

------------------------------------------------------------------------

## 📌 Features

-   View all trains
-   Search trains by number, by name, or by source and destination
-   View seat availability for a train on a journey date
-   Book **1 to 5 passengers** in one booking with a **single PNR**
-   Choose a seat yourself, or let the system allot one using a
    **berth preference** (Lower / Middle / Upper)
-   Join a **waiting list** when a coach is full; a seat is allotted
    automatically when someone cancels
-   Search a booking by PNR and view all bookings
-   Cancel an **entire booking** or **one passenger** of it
-   Book up to **120 days** before the journey
-   Trains and bookings are stored in files and survive a restart

------------------------------------------------------------------------

## 🛠️ Tech Stack

  Technology     Purpose
  -------------- ---------------------------------------------
  **C++17**      Programming language
  **OOP**        Program design
  **STL**        `vector`, `list`, `array`, `map`, `multimap`
  **CMake**      Build system
  **File I/O**   Storage of trains and bookings
  **VS Code**    Development environment

------------------------------------------------------------------------

## 🧱 Project Structure

``` text
RailwayReservation/
│
├── main.cpp
│
├── include/
│   ├── RailwaySystem.h      menu and all user operations
│   ├── FileManager.h        reading and writing the data files
│   │
│   ├── Train.h
│   ├── Coach.h              abstract base class of all coaches
│   ├── CoachTypes.h         the four concrete coach classes
│   ├── Seat.h
│   │
│   ├── Booking.h
│   ├── Ticket.h
│   ├── Passenger.h
│   │
│   ├── Displayable.h        interface: can be shown on screen
│   ├── Serializable.h       interface: can be saved as text
│   │
│   ├── Exceptions.h         the exception classes
│   ├── WaitingQueue.h       class template (waiting list)
│   └── InputHelper.h        function template (safe input)
│
├── src/
│   ├── RailwaySystem.cpp
│   ├── FileManager.cpp
│   ├── Train.cpp
│   ├── Coach.cpp
│   ├── CoachTypes.cpp
│   ├── Seat.cpp
│   ├── Booking.cpp
│   ├── Ticket.cpp
│   └── Passenger.cpp
│
├── data/
│   ├── trains.dat
│   └── bookings.dat
│
├── CMakeLists.txt
└── README.md
```

------------------------------------------------------------------------

## 🏗️ System Architecture

``` text
                    ┌─────────────────────┐
                    │       main.cpp      │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   RailwaySystem     │
                    └──────────┬──────────┘
                               │
             ┌─────────────────┼─────────────────┐
             │                 │                 │
             ▼                 ▼                 ▼
          Train             Booking         FileManager
             │                 │
             ▼                 ▼
           Coach             Ticket
             │                 │
             ▼                 ▼
           Seat            Passenger
```

### Main responsibilities

**RailwaySystem** - Shows the menu - Performs every user operation -
Owns the trains, the bookings and the waiting lists

**Train** - Stores train information - Owns its coaches

**Coach** - Abstract base class for all coaches - Stores the coach
number - Contains seats

**SleeperCoach, AC3TierCoach, AC2TierCoach, FirstACCoach** - The
concrete coach classes - Each provides its own type name, fare and
berth layout

**Seat** - One berth: its number and type (Lower / Middle / Upper)

**Passenger** - Stores and validates passenger details

**Ticket** - Connects a passenger with a coach, seat, fare and status

**Booking** - One reservation - Has one PNR - Contains 1 to 5 tickets

**FileManager** - Loads train data - Loads and saves bookings

**WaitingQueue** - A first-come, first-served queue (class template)

------------------------------------------------------------------------

## 🚂 Trains

Trains are loaded at startup from:

``` text
data/trains.dat
```

Each line describes one train and its coaches:

``` text
TrainNumber|Name|Source|Destination|Departure|Arrival|CoachCount|
CoachNumber|CoachType|Seats|...
```

Example:

``` text
12301|Rajdhani Express|New Delhi|Mumbai|16:55|08:35|2|1|AC 2 Tier|6|2|First AC|6
```

Lines starting with `#` are ignored. To add a train, add a line to the
file; no code change is needed. A line that is not valid is skipped
with a warning that names the line and the reason.

### Coach types and fares

Each kind of coach is its own class and knows its own fare and berth
layout.

  Coach type   Class            Fare   Berths
  ------------ ---------------- ------ -----------------------
  Sleeper      `SleeperCoach`   500    Lower, Middle, Upper
  AC 3 Tier    `AC3TierCoach`   1000   Lower, Middle, Upper
  AC 2 Tier    `AC2TierCoach`   1500   Lower, Upper
  First AC     `FirstACCoach`   2000   Lower, Upper

------------------------------------------------------------------------

## 🎟️ Booking

A booking has one PNR, one train, one journey date and 1 to 5 tickets.

``` text
Start
  │
  ▼
Enter train number
  │
  ▼
Enter journey date ........ today up to 120 days ahead
  │
  ▼
Enter number of tickets ... 1 to 5
  │
  ▼
Seat availability is shown
  │
  ▼
For each passenger:
  ├── choose a coach
  │     └── coach full? → join the waiting list, or pick another coach
  ├── choose a seat
  │     ├── 1. Choose a seat myself
  │     └── 2. Automatic allocation with berth preference
  └── enter name, age, gender, phone
  │
  ▼
PNR is generated → booking is saved → booking is displayed
```

If an entry is not valid (unknown coach, seat already taken, wrong age
or phone number), the program explains why and asks again. The
passengers already entered are not lost.

### Automatic seat allocation

The passenger picks a berth preference: Lower, Middle, Upper or no
preference. The system allots the first free seat of that type in the
chosen coach. If no berth of that type is free, it allots another free
berth and says so.

### Waiting list

When every seat of a coach is taken for the journey date, the passenger
can join the waiting list for that coach. A waiting ticket has no seat
yet and the status `Waiting`.

When a confirmed ticket in that coach is cancelled, the freed seat goes
to the passenger who has been waiting the longest, and that ticket
becomes `Confirmed`.

### PNR

Each booking receives a unique PNR (`PNR1001`, `PNR1002`, ...). The
next number is restored from the saved bookings at startup, so numbers
continue after a restart.

------------------------------------------------------------------------

## 💺 Seat Availability

Availability depends on the **journey date**: a seat booked for
`10/11/2026` is still free on `11/11/2026`.

A seat is taken when a `Confirmed` ticket exists for the same train,
journey date, coach and seat. Cancelled and waiting tickets do not
occupy a seat.

``` text
========== SEAT AVAILABILITY ==========
Train        : 12306 - Tejas Express
Journey Date : 25/12/2026

Coach 2 - First AC (0 of 4 seats free)
  Seat 1 (Lower) - Booked
  Seat 2 (Upper) - Booked
  Seat 3 (Lower) - Booked
  Seat 4 (Upper) - Booked
  Waiting list: 2 passenger(s)
=======================================
```

------------------------------------------------------------------------

## 🔒 Cancellation

Enter the PNR, then choose:

``` text
1. Cancel the entire booking
2. Cancel one passenger
3. Go back
```

-   Cancelled tickets stay in the file with the status `Cancelled`, so
    the booking history is kept.
-   Each seat that becomes free is given to the first passenger on the
    waiting list of that coach, if there is one.
-   If the change cannot be saved to the file, it is undone and an
    error is shown.

The status of a booking follows from its tickets:

  Booking status   Meaning
  ---------------- --------------------------------------------
  `Confirmed`      at least one passenger has a seat
  `Waiting`        nobody has a seat yet; all are waiting
  `Cancelled`      every ticket is cancelled

------------------------------------------------------------------------

## 💾 Data Persistence

Bookings are stored in:

``` text
data/bookings.dat
```

One line per booking:

``` text
PNR|Train|Date|Status|TicketCount|
Name|Age|Gender|Phone|Coach|Seat|Fare|TicketStatus|...
```

Example:

``` text
PNR1004|12306|25/12/2026|Confirmed|2|Asha Rao|30|F|9876543210|2|1|2000|Confirmed|Ravi Rao|35|M|9876543211|2|0|2000|Waiting
```

-   The two older formats (tickets without their own status, and the
    original one-passenger format) are still read. They are converted
    to the current format the next time the file is rewritten.
-   A line that cannot be understood is skipped with a warning. It is
    kept and written back unchanged, so it is never lost.
-   When the whole file is rewritten, the new content is first written
    to a temporary file and then put in place of the old one.

------------------------------------------------------------------------

## ▶️ Building the Project

### Prerequisites

-   A C++17 compiler
-   CMake
-   VS Code (recommended)

### Configure and build

From the project root:

``` bash
cmake -S . -B build
cmake --build build
```

### Run

Run the program **from the project root**, because it looks for the
`data` folder in the current directory.

``` powershell
.\build\RailwayReservation.exe
```

With a Visual Studio generator the program is at
`.\build\Debug\RailwayReservation.exe` instead.

------------------------------------------------------------------------

## 🖥️ Application Menu

``` text
====================================
       RAILWAY RESERVATION SYSTEM
====================================
1. View All Trains
2. Search Train
3. Book Ticket
4. View All Bookings
5. Search Booking
6. Cancel Booking
7. View Seat Availability
8. Exit
```

------------------------------------------------------------------------

## 🧪 Validation

  Input              Rule
  ------------------ ----------------------------------------------
  Menu choices       must be a number in the offered range
  Journey date       `DD/MM/YYYY`, a real date, not in the past,
                     at most 120 days ahead
  Tickets            1 to 5 per booking
  Coach              must exist in the train
  Seat               must exist, and be free on the journey date
  Name               not empty, must not contain `|`
  Age                1 to 120
  Gender             M, F or O
  Phone number       exactly 10 digits

Typing letters where a number is expected does not break the program;
it asks again.

------------------------------------------------------------------------

## 📘 Concept Map

In the source code every concept is marked with a comment that starts
with a tag in square brackets, followed by a plain explanation:

``` cpp
// [OOP: Multiple inheritance]
// [Exceptions: re-throwing]
// [Templates: generic class]
// [STL: multimap]
```

Searching the project for `[OOP`, `[Exceptions`, `[Templates` or `[STL`
lists all of them.

### Objects, classes and encapsulation

| Concept | Where to look |
|---|---|
| Objects and classes | [main.cpp](main.cpp), [include/Passenger.h](include/Passenger.h) |
| Encapsulation, data and method binding | [include/Passenger.h](include/Passenger.h) |
| Access specification: `private`, `public` | [include/Passenger.h](include/Passenger.h) |
| Access specification: `protected` | [include/Coach.h](include/Coach.h) |

**Comparison of C with C++.** In C this program would use `struct`s for
the data and separate global functions that take a pointer to the
struct. Any function could change any field. In C++ the data and the
functions that work on it are bound together in a class, the data is
hidden behind `private`, and features such as constructors, references,
`std::string`, `std::vector`, function overloading, inheritance,
exceptions and templates replace manual memory handling, `char` arrays
and error codes.

**Executing and debugging.** Build and run the program as described in
*Building the Project*. To debug in VS Code, set a breakpoint by
clicking next to a line number and press `F5`; the program stops at
that line so that variables can be inspected and the code stepped
through line by line.

### Inheritance

| Concept | Where to look |
|---|---|
| Sub and super classes, single inheritance | [include/Coach.h](include/Coach.h), [include/CoachTypes.h](include/CoachTypes.h) |
| Sub class constructors calling the super class | [src/CoachTypes.cpp](src/CoachTypes.cpp) |
| Code reuse | `Coach::addSeats` in [src/Coach.cpp](src/Coach.cpp) |
| Inheritance and subtyping | `Train::addCoach` in [include/Train.h](include/Train.h), `createCoach` in [src/CoachTypes.cpp](src/CoachTypes.cpp) |
| Interfaces | [include/Displayable.h](include/Displayable.h), [include/Serializable.h](include/Serializable.h) |
| Multiple inheritance | [include/Ticket.h](include/Ticket.h), [include/Booking.h](include/Booking.h) |

``` text
            Displayable            Serializable
   ┌────┬──────┼──────┬──────┬────────┐  │
   │    │      │      │      │        │  │
 Train Seat Passenger Coach  Ticket ──┼──┤
                        │    Booking ─┴──┘
        ┌───────────┬───┴───────┬────────────┐
   SleeperCoach AC3TierCoach AC2TierCoach FirstACCoach
```

### Polymorphism

| Concept | Where to look |
|---|---|
| Method overloading (static polymorphism) | `findTrain(int)` and `findTrain(const string&)` in [include/RailwaySystem.h](include/RailwaySystem.h) |
| Method overriding (subtype polymorphism) | [include/CoachTypes.h](include/CoachTypes.h), [src/CoachTypes.cpp](src/CoachTypes.cpp) |
| Subtype polymorphism through base class pointers | `vector<Coach*>` in [include/Train.h](include/Train.h), `coach->getFare()` in `bookTicket` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| Virtual versus non-virtual functions | [include/Coach.h](include/Coach.h) |
| Abstract class, pure virtual functions | [include/Coach.h](include/Coach.h) |
| Abstraction through polymorphism | `show(const Displayable&)` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp), `Coach::display` in [src/Coach.cpp](src/Coach.cpp) |
| Virtual destructor | [include/Coach.h](include/Coach.h), `Train::~Train` in [src/Train.cpp](src/Train.cpp) |
| Operator overloading | `Coach::operator[]` in [include/Coach.h](include/Coach.h) |

Because `Train` stores its coaches as raw pointers created with `new`,
it also has a destructor, a copy constructor and an assignment operator
(see [include/Train.h](include/Train.h)).

### Exception handling

| Concept | Where to look |
|---|---|
| The try-catch-throw paradigm | [include/Exceptions.h](include/Exceptions.h), `run` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| Throwing errors | `bookTicket` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp), `loadTrains` in [src/FileManager.cpp](src/FileManager.cpp) |
| Catching one error and throwing a clearer one | `toInt` in [src/FileManager.cpp](src/FileManager.cpp) |
| Multiple catch statements | `chooseSeat` and `run` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| Re-throwing exceptions (`throw;`) | `bookTicket` and `cancelBooking` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| Exceptions in constructors | `Passenger::Passenger` in [src/Passenger.cpp](src/Passenger.cpp), `readPassenger` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| Exceptions in destructors | [include/Train.h](include/Train.h), `parseTrain` in [src/FileManager.cpp](src/FileManager.cpp) |
| Exceptions and operator overloading | `Coach::operator[]` in [src/Coach.cpp](src/Coach.cpp) |
| Exceptions and inheritance | [include/Exceptions.h](include/Exceptions.h), `run` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp), `loadBookings` in [src/FileManager.cpp](src/FileManager.cpp) |
| Class templates with exception handling | `WaitingQueue::takeNext` in [include/WaitingQueue.h](include/WaitingQueue.h), `promoteFromWaitingList` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |

``` text
std::exception
      │
RailwayException
      ├── InvalidInputException      the user typed something not acceptable
      ├── NotFoundException          train, booking or seat does not exist
      ├── SeatUnavailableException   the seat cannot be given
      ├── FileException              a data file could not be read or written
      └── InputClosedException       the keyboard input has ended
```

### Templates

| Concept | Where to look |
|---|---|
| Generic function | `readValue<T>` in [include/InputHelper.h](include/InputHelper.h), `showAll<T>` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| Generic class | `WaitingQueue<T>` in [include/WaitingQueue.h](include/WaitingQueue.h) |

### STL containers

| Container | Used for | Where to look |
|---|---|---|
| `vector` | trains, bookings, tickets, seats, coaches | [include/Booking.h](include/Booking.h), [include/RailwaySystem.h](include/RailwaySystem.h) |
| `list` | the items of a waiting queue | [include/WaitingQueue.h](include/WaitingQueue.h) |
| `array` | days in each month, berth types | top of [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| `map` | PNR → booking, coach → waiting queue | [include/RailwaySystem.h](include/RailwaySystem.h), `findBookingIndex` and `rebuildIndexes` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |
| `multimap` | train and date → all bookings of that journey | [include/RailwaySystem.h](include/RailwaySystem.h), `isSeatBooked` in [src/RailwaySystem.cpp](src/RailwaySystem.cpp) |

------------------------------------------------------------------------

## 🚀 Possible Future Improvements

-   RAC (two passengers sharing one berth until a seat is free)
-   Trains that run only on certain days of the week
-   Fare concessions and distance-based fares
-   Search bookings by phone number or journey date
-   An admin menu to add and remove trains from inside the program
-   Refund calculation on cancellation
-   Unit tests
-   Database storage

------------------------------------------------------------------------

## 👨‍💻 Author

**Prem Kumar Singh**

Developed as a C++17 project for learning and demonstrating
object-oriented software design, exception handling, templates, the
STL, file handling and project organization.

------------------------------------------------------------------------

## 📄 License

This project is intended for educational and learning purposes.
