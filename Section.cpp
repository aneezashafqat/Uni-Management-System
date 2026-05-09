#include "Section.h"
#include <iostream>
#include <iomanip>

using namespace std;

Section::Section() : studentCount(0) {}

Section::Section(const string& sid, const string& cid, const string& tid,
    const string& vid, const string& ts)
    : sectionID(sid), courseID(cid), teacherID(tid), venueID(vid), timeSlot(ts), studentCount(0) {
}

void Section::addStudent(const string& sid) {
    if (studentCount < MAX_COURSE_ENROLL) {
        enrolledStudentIDs[studentCount++] = sid;
    }
}

bool Section::hasStudent(const string& sid) const {
    for (int i = 0; i < studentCount; i++) {
        if (enrolledStudentIDs[i] == sid) return true;
    }
    return false;
}

void Section::display() const {
    cout << left << setw(12) << sectionID << setw(10) << courseID
        << setw(10) << teacherID << setw(12) << venueID
        << setw(18) << timeSlot << studentCount << " students\n";
}