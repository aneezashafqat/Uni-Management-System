#include "Course.h"
#include "Student.h"
#include "Assessment.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

using namespace std;

Course::Course() : enrollmentCount(0), assessmentCount(0) {
    for (int i = 0; i < MAX_COURSE_ENROLL; i++) enrolledStudents[i] = NULL;
    for (int i = 0; i < MAX_ASSESSMENTS; i++) assessments[i] = NULL;
}

Course::Course(const string& cid, const string& t, const string& tid, const string& typ)
    : courseID(cid), title(t), teacherID(tid), type(typ), enrollmentCount(0), assessmentCount(0) {
    for (int i = 0; i < MAX_COURSE_ENROLL; i++) enrolledStudents[i] = NULL;
    for (int i = 0; i < MAX_ASSESSMENTS; i++) assessments[i] = NULL;
}

Course::~Course() {
    for (int i = 0; i < assessmentCount; i++) {
        delete assessments[i];
        assessments[i] = NULL;
    }
}

bool Course::isStudentEnrolled(const string& sid) const {
    for (int i = 0; i < enrollmentCount; i++)
        if (enrolledStudents[i] && enrolledStudents[i]->getID() == sid) return true;
    return false;
}

bool Course::addStudent(Student* s) {
    if (isStudentEnrolled(s->getID())) {
        cout << "ERROR: Student already enrolled.\n";
        return false;
    }
    if (enrollmentCount >= MAX_COURSE_ENROLL) {
        cout << "ERROR: Course is full.\n";
        return false;
    }
    enrolledStudents[enrollmentCount++] = s;
    s->enrollCourse(courseID);
    return true;
}

bool Course::removeStudent(const string& sid) {
    for (int i = 0; i < enrollmentCount; i++) {
        if (enrolledStudents[i] && enrolledStudents[i]->getID() == sid) {
            enrolledStudents[i]->unenrollCourse(courseID);
            for (int j = i; j < enrollmentCount - 1; j++)
                enrolledStudents[j] = enrolledStudents[j + 1];
            enrollmentCount--;
            return true;
        }
    }
    return false;
}

void Course::addAssessment(Assessment* a) {
    if (assessmentCount < MAX_ASSESSMENTS) assessments[assessmentCount++] = a;
}

double Course::calculateFinalGrade(const string& studentID) const {
    double total = 0, totalWeight = 0;
    bool anyScore = false;
    for (int i = 0; i < assessmentCount; i++) {
        if (assessments[i]->hasScore(studentID)) {
            total += assessments[i]->getWeightedScore(studentID);
            totalWeight += assessments[i]->getWeightage();
            anyScore = true;
        }
    }
    if (!anyScore) return -1;
    if (totalWeight <= 0) return 0;
    return (total / totalWeight) * 100.0;
}

void Course::display() const {
    cout << left << setw(10) << courseID << setw(30) << title
        << setw(12) << type << setw(12) << teacherID
        << enrollmentCount << " students\n";
}

void Course::showAssessments() const {
    cout << "\nAssessments for " << title << " (" << type << "):\n";
    printLine('-', 50);
    for (int i = 0; i < assessmentCount; i++) {
        cout << "  [" << (i + 1) << "] " << assessments[i]->getType()
            << " — " << assessments[i]->getName()
            << "  Weight: " << assessments[i]->getWeightage() << "%\n";
    }
}