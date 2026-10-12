#ifndef TRAIN_H
#define TRAIN_H

#include <string>
#include <vector>
#include "Coach.h"
#include "Displayable.h"

class Train : public Displayable {
private:
    int trainNumber;
    std::string trainName;
    std::string source;
    std::string destination;

    std::string departureTime;
    std::string arrivalTime;

    // [OOP: Subtype polymorphism]
    // The train stores POINTERS to Coach, not Coach objects.
    // Each pointer can point to any sub class of Coach, so one
    // vector holds sleeper, AC and first class coaches together.
    // Calling a virtual function through these pointers runs the
    // version that belongs to the real type of each coach.
    //
    // (A vector<Coach> could not do this: Coach is abstract, and
    // storing a sub class object by value would cut off its
    // sub class part - "object slicing".)
    std::vector<Coach*> coaches;

public:
    Train(int trainNumber,
          std::string trainName,
          std::string source,
          std::string destination,
          std::string departureTime,
          std::string arrivalTime);

    // The coaches are created with "new", so the Train has to manage
    // that memory itself. A class that owns raw pointers needs these
    // three functions:
    //
    //   destructor          - deletes the coaches when the train is destroyed
    //   copy constructor    - gives a copied train its own copies of the coaches
    //   assignment operator - the same, for "trainA = trainB"
    //
    // Without them, copying a train would copy only the pointers. Two
    // trains would then share the same coaches and both would try to
    // delete them.
    //
    // [Exceptions in destructors]
    // A destructor must never let an exception escape. Destructors
    // also run while another exception is being handled (to clean up
    // local objects), and a second exception at that moment ends the
    // program. ~Train() only deletes memory, which cannot throw.
    ~Train();
    Train(const Train& other);
    Train& operator=(const Train& other);

    // [OOP: Inheritance and subtyping]
    // The parameter type is Coach*, but a pointer to a SleeperCoach or
    // any other sub class object can be passed here, because a
    // sub class object IS A Coach.
    //
    // The coach must have been created with "new". The train takes
    // ownership of it and deletes it in its destructor.
    void addCoach(Coach* coach);

    void display() const override;

    int getTrainNumber() const;
    std::string getTrainName() const;
    std::string getSource() const;
    std::string getDestination() const;

    int getTotalSeats() const;

    // Returns nullptr when the train has no coach with this number.
    const Coach* findCoach(int coachNumber) const;

    bool runsDaily() const;

    const std::vector<Coach*>& getCoaches() const;
};

#endif
