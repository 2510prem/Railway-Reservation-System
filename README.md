# 🚆 Railway Reservation System

A **CLI-based Railway Reservation System** developed in **C++17** using
**Object-Oriented Programming (OOP)** principles and **CMake**.

The project is designed as an incremental, modular railway booking
application with train management, passenger details, coach/seat
management, booking, PNR generation, cancellation, fare calculation,
journey-date-aware availability, and persistent booking storage.

------------------------------------------------------------------------

## 📌 Project Overview

The Railway Reservation System allows users to:

-   View available trains
-   Search trains by:
    -   Train number
    -   Train name
    -   Source and destination
-   View seat availability
-   Book railway tickets
-   Book **1--5 passengers in a single transaction**
-   Assign different coaches and seats to passengers
-   Generate a **single PNR for a group booking**
-   Calculate fares according to coach type
-   Search bookings using PNR
-   View all bookings
-   Cancel an entire booking using its PNR
-   Store bookings in a file so that data persists after restarting the
    application

The application runs entirely through the **command line**.

------------------------------------------------------------------------

## 🛠️ Tech Stack

  Technology     Purpose
  -------------- -----------------------------------
  **C++17**      Core programming language
  **OOP**        System architecture
  **CMake**      Build system
  **STL**        `vector`, `string`, streams, etc.
  **File I/O**   Persistent booking storage
  **VS Code**    Development environment

------------------------------------------------------------------------

## 🧱 Project Structure

``` text
RailwayReservation/
│
├── main.cpp
│
├── include/
│   ├── Train.h
│   ├── Passenger.h
│   ├── Booking.h
│   ├── Ticket.h
│   ├── Coach.h
│   ├── Seat.h
│   ├── RailwaySystem.h
│   ├── FareCalculator.h
│   └── FileManager.h
│
├── src/
│   ├── Train.cpp
│   ├── Passenger.cpp
│   ├── Booking.cpp
│   ├── Ticket.cpp
│   ├── Coach.cpp
│   ├── Seat.cpp
│   ├── RailwaySystem.cpp
│   ├── FareCalculator.cpp
│   └── FileManager.cpp
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

The project follows a modular OOP design.

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

                    FareCalculator
```

### Main responsibilities

**RailwaySystem** - Controls the application flow - Handles menus and
user interaction - Manages trains and bookings

**Train** - Stores train information - Contains coaches - Provides
train-level operations

**Coach** - Stores coach number and coach type - Contains seats

**Seat** - Represents individual seats

**Passenger** - Stores passenger information

**Ticket** - Connects a passenger with a coach, seat, and fare

**Booking** - Represents a complete reservation - Contains one PNR - Can
contain up to 5 tickets

**FareCalculator** - Calculates fare based on coach type

**FileManager** - Saves and loads booking data

------------------------------------------------------------------------

## 🎟️ Multi-Passenger Booking

A major feature of the system is **group booking**.

A single booking can contain a maximum of **5 tickets**.

``` text
Booking
│
├── PNR
├── Train Number
├── Journey Date
├── Status
│
└── Tickets
    ├── Ticket 1
    │   ├── Passenger
    │   ├── Coach
    │   ├── Seat
    │   └── Fare
    │
    ├── Ticket 2
    ├── Ticket 3
    ├── Ticket 4
    └── Ticket 5
```

For example:

``` text
PNR1001

Passenger 1 → Coach 1 → Seat 2
Passenger 2 → Coach 1 → Seat 3
Passenger 3 → Coach 2 → Seat 1
```

All three passengers belong to the same booking and therefore share the
same PNR.

------------------------------------------------------------------------

## 🔢 PNR System

Each booking receives a unique PNR.

Example:

``` text
PNR1001
PNR1002
PNR1003
```

The system restores the next PNR number from previously saved bookings
when the application starts.

This prevents PNR numbers from restarting after every program execution.

------------------------------------------------------------------------

## 💺 Seat Availability

Seat availability is **journey-date aware**.

A seat booked on:

``` text
10/10/2026
```

does not automatically become unavailable for:

``` text
11/10/2026
```

The system checks existing confirmed bookings for:

``` text
Train Number
+
Coach Number
+
Seat Number
+
Journey Date
```

A cancelled booking does not block the seat.

------------------------------------------------------------------------

## 💰 Fare Calculation

Fare is calculated using the coach type through the `FareCalculator`
class.

Different coaches can therefore have different fares.

For example, the system supports coach types such as:

``` text
First AC
AC 2 Tier
AC 3 Tier
Sleeper
```

The exact fare values are defined by the implementation of
`FareCalculator`.

------------------------------------------------------------------------

## 💾 Data Persistence

Bookings are stored in:

``` text
data/bookings.dat
```

The application loads existing bookings when it starts and saves new
bookings to the file.

The current persistence design supports the new multi-ticket structure:

``` text
PNR|Train|Date|Status|TicketCount|
Name|Age|Gender|Phone|Coach|Seat|Fare|...
```

Example:

``` text
PNR1001|12301|10/10/2026|Confirmed|3|Prem Kumar|21|M|9876543210|1|2|1500|Rahul Sharma|22|M|9876543211|1|3|1500|Ankit Singh|20|M|9876543212|2|1|2000
```

The loader also supports the previous single-ticket booking format so
that existing saved bookings can continue to be loaded.

------------------------------------------------------------------------

## 🚂 Currently Configured Trains

The system currently contains example trains such as:

  Train   Name                Route                Departure   Arrival
  ------- ------------------- -------------------- ----------- ---------
  12301   Rajdhani Express    New Delhi → Mumbai   16:55       08:35
  12302   Shatabdi Express    New Delhi → Bhopal   06:00       14:00
  12303   Intercity Express   Dhanbad → Ranchi     07:30       11:00

Each train has its own coach configuration.

------------------------------------------------------------------------

## ▶️ Building the Project

### Prerequisites

Install:

-   A C++17-compatible compiler
-   CMake
-   VS Code (recommended)

### Configure the project

From the project root:

``` bash
cmake -S . -B build
```

### Build

``` bash
cmake --build build
```

### Run on Windows

Depending on the CMake generator:

``` powershell
.\build\Debug\RailwayReservation.exe
```

------------------------------------------------------------------------

## 🖥️ Application Menu

The application provides a command-line menu similar to:

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
7. Display Seats
8. Exit
```

------------------------------------------------------------------------

## 🔄 Typical Booking Flow

``` text
Start Application
       │
       ▼
Select "Book Ticket"
       │
       ▼
Enter Train Number
       │
       ▼
Enter Journey Date
       │
       ▼
Enter Number of Tickets
       │
       ├── 1
       ├── 2
       ├── 3
       ├── 4
       └── 5
       │
       ▼
Select Coach & Seat
       │
       ▼
Enter Passenger Details
       │
       ▼
Calculate Fare
       │
       ▼
Generate PNR
       │
       ▼
Create Booking
       │
       ▼
Save to bookings.dat
       │
       ▼
Display Booking
```

------------------------------------------------------------------------

## 🧪 Validation

The system performs validation for important passenger and booking
inputs, including:

-   Journey date
-   Past journey dates
-   Passenger age
-   Passenger gender
-   Phone number
-   Train existence
-   Coach existence
-   Seat existence
-   Duplicate seat selection
-   Existing seat bookings
-   Maximum group size of 5 passengers
-   Duplicate PNR prevention

------------------------------------------------------------------------

## 🔒 Cancellation

Cancellation is performed using the PNR.

Since a PNR represents one complete reservation transaction, cancelling
the PNR currently cancels the **entire group booking**.

Example:

``` text
PNR1001
 ├── Passenger 1
 ├── Passenger 2
 └── Passenger 3

Cancel PNR1001
       ↓
Entire booking → Cancelled
```

Cancelled bookings remain stored in the data file with their status
changed to:

``` text
Cancelled
```

This allows the system to preserve booking history while preventing the
cancelled seats from being treated as occupied.

------------------------------------------------------------------------

## 🎯 Design Goals

The project is being developed incrementally with focus on:

-   Object-oriented design
-   Separation of responsibilities
-   Reusable classes
-   Data persistence
-   Input validation
-   Realistic railway booking logic
-   Maintainable C++ code
-   C++17 compatibility

------------------------------------------------------------------------

## 🚀 Future Improvements

Possible future enhancements include:

-   Dynamic train management
-   User/admin login
-   Waiting list and RAC
-   Partial cancellation of group bookings
-   Ticket modification
-   Automatic seat allocation
-   Better seat layouts
-   Different quotas
-   Payment simulation
-   Booking history by passenger
-   More advanced file/database storage
-   Database integration
-   Improved CLI interface
-   Unit testing

------------------------------------------------------------------------

## 📚 Learning Objectives

This project provides practical experience with:

-   Classes and objects
-   Encapsulation
-   Composition
-   STL containers
-   Constructors
-   References and `const`
-   File handling
-   Exception-safe input handling
-   Modular C++ development
-   CMake
-   Multi-class project organization
-   Basic system design

------------------------------------------------------------------------

## 👨‍💻 Author

**Prem Kumar Singh**

Developed as a C++17 project for learning and demonstrating
object-oriented software design, data structures, file handling, and
system-level project organization.

------------------------------------------------------------------------

## 📄 License

This project is intended for educational and learning purposes.
