#ifndef SECTION_H
#define SECTION_H

#include "Constants.h"
#include <string>

class Section {
    std::string sectionID;
    std::string courseID;
    std::string teacherID;
    std::string venueID;
    std::string timeSlot;
    std::string enrolledStudentIDs[MAX_COURSE_ENROLL];
    int studentCount;

public:
    Section();
    Section(const std::string& sid, const std::string& cid, const std::string& tid,
        const std::string& vid, const std::string& ts);

    std::string getSectionID() const { return sectionID; }
    std::string getCourseID() const { return courseID; }
    std::string getTeacherID() const { return teacherID; }
    std::string getVenueID() const { return venueID; }
    std::string getTimeSlot() const { return timeSlot; }

    void setTimeSlot(const std::string& ts) { timeSlot = ts; }
    void setVenueID(const std::string& v) { venueID = v; }

    int getStudentCount() const { return studentCount; }
    void setStudentCount(int n) { studentCount = n; }

    void addStudent(const std::string& sid);
    bool hasStudent(const std::string& sid) const;
    void display() const;

    
};

#endif