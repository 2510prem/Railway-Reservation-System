#include "FareCalculator.h"

using namespace std;

double FareCalculator::calculateFare(const string& coachType) {

    if (coachType == "Sleeper") {
        return 500;
    }

    if (coachType == "AC 3 Tier") {
        return 1000;
    }

    if (coachType == "AC 2 Tier") {
        return 1500;
    }

    if (coachType == "First AC") {
        return 2000;
    }

    return 0;
}