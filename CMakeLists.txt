cmake_minimum_required(VERSION 3.20)

project(RailwayReservation)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

## include_directories(include)

add_executable(RailwayReservation
    main.cpp

    src/Train.cpp
    src/Passenger.cpp
    src/Booking.cpp
    src/Coach.cpp
    src/Seat.cpp
    src/RailwaySystem.cpp
    src/FareCalculator.cpp
    src/FileManager.cpp
    src/Ticket.cpp
)

target_include_directories(RailwayReservation PRIVATE include)