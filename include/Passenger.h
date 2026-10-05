#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

class Passenger {
private:
    std::string name;
    int age;
    char gender;
    std::string phone;

public:
    Passenger(std::string name,
              int age,
              char gender,
              std::string phone);

    void display() const;

    std::string getName() const;
    int getAge() const;
    char getGender() const;
    std::string getPhone() const;
};

#endif