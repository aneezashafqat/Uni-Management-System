#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <string>
#include "Constants.h"

class Section;
class Venue;
class Course;

class Scheduler {
    struct Entry {
        std::string sectionID;
        std::string venueID;
        std::string timeSlot;
        bool conflict;
    };
    Entry entries[MAX_SECTIONS];
    int entryCount;

    static const int NUM_SLOTS = 6;
    static const char* TIME_SLOTS[NUM_SLOTS];

    bool isSlotTaken(const std::string& venueID, const std::string& slot) const;

public:
    Scheduler();
    std::string assign(Section sections[], int secCount, Venue venues[],
        int venCount, Course* courses[], int curCount);
};

#endif