#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
#include "Displayable.h"

// [OOP: Objects and Classes]
// A CLASS is a blueprint; an OBJECT is one thing built from it.
// "Passenger" is the class. Each person who books a ticket is a
// separate Passenger object with its own name, age, gender and phone.
//
// [OOP: Data and method binding]
// In C, data (a struct) and the functions that work on it are
// written separately. In C++ a class binds them together: the data
// members and the member functions below live in one unit.
class Passenger : public Displayable {

// [OOP: Encapsulation and access specification - private]
// The data members are private, so code outside this class cannot
// read or change them directly. "passenger.age = -5;" does not
// compile. This hiding of data is called encapsulation.
private:
    std::string name;
    int age;
    char gender;
    std::string phone;

// [OOP: Access specification - public]
// Public members are the only way the rest of the program can use
// a Passenger. The getters give read-only access to the private data.
// (The third specifier, "protected", is shown in Coach.h.)
public:
    // Constructor: runs automatically when a Passenger object is created.
    // Throws InvalidInputException if any of the details is not valid.
    Passenger(std::string name,
              int age,
              char gender,
              std::string phone);

    void display() const override;

    std::string getName() const;
    int getAge() const;
    char getGender() const;
    std::string getPhone() const;
};

#endif