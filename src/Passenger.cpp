#include "Passenger.h"
#include <iostream>

using namespace std;

Passenger::Passenger(string name,
                     int age,
                     char gender,
                     string phone)
{
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