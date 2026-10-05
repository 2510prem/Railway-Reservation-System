#ifndef FARECALCULATOR_H
#define FARECALCULATOR_H

#include <string>

class FareCalculator {
public:
    static double calculateFare(const std::string& coachType);
};

#endif