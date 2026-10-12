#ifndef WAITINGQUEUE_H
#define WAITINGQUEUE_H

#include <list>
#include "Exceptions.h"

// [Templates: generic class]
// A class template is a class with a "blank" type. T is a placeholder
// that is filled in when the class is used:
//
//     WaitingQueue<WaitingEntry> queue;   // T becomes WaitingEntry
//     WaitingQueue<int> numbers;          // T becomes int
//
// The compiler writes a separate version of the class for each type.
// The whole template lives in the header file, because the compiler
// needs to see the full code wherever the template is used.
//
// WaitingQueue is a "first come, first served" line: items are added
// at the back and taken from the front.
template <typename T>
class WaitingQueue {
private:
    // [STL: list]
    // std::list is a doubly linked list. Adding at the back and
    // removing from the front are both fast, which is exactly what a
    // queue needs. (A vector would have to shift every element when
    // the first one is removed.)
    std::list<T> items;

public:
    // Adds an item at the back of the queue.
    void add(const T& item) {
        items.push_back(item);
    }

    // [Templates: class templates with exception handling]
    // Removes and returns the item at the front of the queue.
    // If the queue is empty there is nothing to return, so the
    // function throws an exception instead.
    T takeNext() {

        if (items.empty()) {
            throw NotFoundException("The waiting list is empty.");
        }

        T first = items.front();

        items.pop_front();

        return first;
    }

    bool isEmpty() const {
        return items.empty();
    }

    int size() const {
        return static_cast<int>(items.size());
    }
};

#endif
