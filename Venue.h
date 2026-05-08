#ifndef VENUE_H
#define VENUE_H

#include <string>

class Venue {
    std::string roomID;
    int capacity;
    bool hasComputers;
public:
    Venue();
    Venue(const std::string& rid, int cap, bool hc);

    std::string getRoomID() const { return roomID; }
    int getCapacity() const { return capacity; }
    bool getHasComputers() const { return hasComputers; }

    void display() const;
};

#endif