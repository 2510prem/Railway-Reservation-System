#include "Passenger.h"
#include "Exceptions.h"
#include <cctype>
#include <iostream>

using namespace std;

// [Exceptions in constructors]
// A constructor has no return value, so it cannot return an error
// code. Throwing an exception is the way for a constructor to say
// "this object cannot be created".
//
// When a constructor throws, the object is never created. It is
// therefore impossible for an invalid Passenger (age -5, empty name)
// to exist anywhere in the program.
Passenger::Passenger(string name,
                     int age,
                     char gender,
                     string phone)
{
    // '|' separates the fields in bookings.dat, so a name
    // containing it would damage the file.
    if (name.empty() ||
        name.find('|') != string::npos) {

        throw InvalidInputException(
            "Invalid passenger name."
        );
    }

    if (age < 1 || age > 120) {

        throw InvalidInputException(
            "Invalid age. Age must be between 1 and 120."
        );
    }

    if (gender != 'M' &&
        gender != 'F' &&
        gender != 'O') {

        throw InvalidInputException(
            "Invalid gender. Enter M, F or O."
        );
    }

    if (phone.length() != 10) {

        throw InvalidInputException(
            "Invalid phone number. It must have exactly 10 digits."
        );
    }

    for (char ch : phone) {

        if (!isdigit(static_cast<unsigned char>(ch))) {

            throw InvalidInputException(
                "Invalid phone number. It must have exactly 10 digits."
            );
        }
    }

    this->name = name;
    this->age = age;
    this->gender = gender;
    this->phone = phone;
}

void Passenger::display() const {

    cout << "Name   : " << name << '\n';
    cout << "Age    : " << age << '\n';
    cout << "Gender : " << gender << '\n';
    cout << "Phone  : " << phone << '\n';
}

string Passenger::getName() const {
    return name;
}

int Passenger::getAge() const {
    return age;
}

char Passenger::getGender() const {
    return gender;
}

string Passenger::getPhone() const {
    return phone;
}
