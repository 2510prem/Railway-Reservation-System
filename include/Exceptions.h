#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>
#include <string>

// [Exceptions: the try-catch-throw paradigm]
// When a function meets a problem it cannot solve itself, it THROWS
// an exception object. The function stops at once, and control jumps
// to the nearest CATCH block that accepts that type of exception.
// Code that may throw is placed inside a TRY block.
//
//     throw InvalidInputException("Invalid age.");    // report
//
//     try { ... }                                     // watch
//     catch (const InvalidInputException& error) {    // handle
//         cout << error.what();
//     }
//
// This file defines the exception classes used in this project.

// [Exceptions and inheritance]
// RailwayException is the base class of all our exceptions. It
// inherits from std::exception, the base class of the standard
// library exceptions.
//
// A catch block for a base class also catches every sub class.
// So "catch (const RailwayException&)" catches any exception below,
// and "catch (const std::exception&)" catches those AND the standard
// ones such as std::invalid_argument.
class RailwayException : public std::exception {
private:
    std::string message;

public:
    explicit RailwayException(const std::string& message)
        : message(message) {
    }

    // Overrides std::exception::what(). Returns the error message.
    // "noexcept" promises that this function never throws.
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// The user typed something that is not acceptable
// (bad date, bad age, bad phone number...).
class InvalidInputException : public RailwayException {
public:
    explicit InvalidInputException(const std::string& message)
        : RailwayException(message) {
    }
};

// Something that was asked for does not exist
// (train, booking, seat, or an entry in an empty waiting list).
class NotFoundException : public RailwayException {
public:
    explicit NotFoundException(const std::string& message)
        : RailwayException(message) {
    }
};

// The seat exists but cannot be given to this passenger.
class SeatUnavailableException : public RailwayException {
public:
    explicit SeatUnavailableException(const std::string& message)
        : RailwayException(message) {
    }
};

// A data file could not be opened, read or written,
// or a line in it is not in the expected format.
class FileException : public RailwayException {
public:
    explicit FileException(const std::string& message)
        : RailwayException(message) {
    }
};

// The keyboard input has ended (for example Ctrl+Z was pressed),
// so the program cannot ask for anything more.
class InputClosedException : public RailwayException {
public:
    InputClosedException()
        : RailwayException("Input closed.") {
    }
};

#endif
