#include "Scheduler.h"
#include "Section.h"
#include "Venue.h"
#include "Course.h"
#include <iostream>
#include <string>

using namespace std;

const char* Scheduler::TIME_SLOTS[6] = {
    "Mon 09:00-12:00", "Mon 13:00-15:00",
    "Tue 09:00-12:00", "Tue 13:00-15:00",
    "Wed 09:00-12:00", "Wed 13:00-15:00"
};

Scheduler::Scheduler() : entryCount(0) {}

bool Scheduler::isSlotTaken(const string& venueID, const string& slot) const {
    for (int i = 0; i < entryCount; i++) {
        if (entries[i].venueID == venueID && entries[i].timeSlot == slot && !entries[i].conflict)
            return true;
    }
    return false;
}

string Scheduler::assign(Section sections[], int secCount, Venue venues[],
    int venCount, Course* courses[], int curCount) {
    entryCount = 0;
    string report = "========== EXAM SCHEDULE ==========\n";

    for (int i = 0; i < secCount; i++) {
        string cid = sections[i].getCourseID();
        Course* course = NULL;
        for (int j = 0; j < curCount; j++) {
            if (courses[j] && courses[j]->getCourseID() == cid) {
                course = courses[j];
                break;
            }
        }
        if (!course) continue;

        if (course->getExamDuration() == 0) {
            string row = "Section " + sections[i].getSectionID() + " (Lab) — No exam required.\n";
            report += row;
            cout << row;
            continue;
        }

        int need = sections[i].getStudentCount();
        bool needPC = course->requiresComputers();
        bool assigned = false;

        for (int v = 0; v < venCount && !assigned; v++) {
            if (venues[v].getCapacity() < need) continue;
            if (needPC && !venues[v].getHasComputers()) continue;

            for (int sl = 0; sl < NUM_SLOTS && !assigned; sl++) {
                string slot = TIME_SLOTS[sl];
                if (!isSlotTaken(venues[v].getRoomID(), slot)) {
                    entries[entryCount].sectionID = sections[i].getSectionID();
                    entries[entryCount].venueID = venues[v].getRoomID();
                    entries[entryCount].timeSlot = slot;
                    entries[entryCount].conflict = false;
                    entryCount++;
                    sections[i].setTimeSlot(slot);
                    sections[i].setVenueID(venues[v].getRoomID());
                    assigned = true;

                    string row = "Section " + sections[i].getSectionID() +
                        " -> Venue: " + venues[v].getRoomID() +
                        " | Slot: " + slot +
                        " | Students: " + to_string(need) +
                        "/" + to_string(venues[v].getCapacity()) + "\n";
                    report += row;
                    cout << row;
                }
            }
        }

        if (!assigned) {
            string row = "[CONFLICT] Section " + sections[i].getSectionID() +
                " could not be assigned a venue/slot!\n";
            report += row;
            cout << row;
        }
    }
    report += "====================================\n";
    return report;
}