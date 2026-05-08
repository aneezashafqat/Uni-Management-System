#include "Student.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

using namespace std;

Student::Student() : courseCount(0) {
    for (int i = 0; i < MAX_ENROLLMENTS; i++) {
        enrolledCourseIDs[i] = "";
        finalGrades[i] = 0;
        gradeSet[i] = false;
    }
}

Student::Student(const string& id, const string& n, const string& e)
    : AcademicEntity(id, n, e), courseCount(0) {
    for (int i = 0; i < MAX_ENROLLMENTS; i++) {
        enrolledCourseIDs[i] = "";
        finalGrades[i] = 0;
        gradeSet[i] = false;
    }
}

bool Student::isEnrolledIn(const string& cid) const {
    for (int i = 0; i < courseCount; i++)
        if (enrolledCourseIDs[i] == cid) return true;
    return false;
}

bool Student::enrollCourse(const string& cid) {
    if (isEnrolledIn(cid) || courseCount >= MAX_ENROLLMENTS) return false;
    enrolledCourseIDs[courseCount++] = cid;
    return true;
}

bool Student::unenrollCourse(const string& cid) {
    for (int i = 0; i < courseCount; i++) {
        if (enrolledCourseIDs[i] == cid) {
            for (int j = i; j < courseCount - 1; j++) {
                enrolledCourseIDs[j] = enrolledCourseIDs[j + 1];
                finalGrades[j] = finalGrades[j + 1];
                gradeSet[j] = gradeSet[j + 1];
            }
            courseCount--;
            return true;
        }
    }
    return false;
}

void Student::setFinalGrade(const string& cid, double grade) {
    for (int i = 0; i < courseCount; i++) {
        if (enrolledCourseIDs[i] == cid) {
            finalGrades[i] = grade;
            gradeSet[i] = true;
            return;
        }
    }
}

double Student::getFinalGrade(const string& cid) const {
    for (int i = 0; i < courseCount; i++)
        if (enrolledCourseIDs[i] == cid) return finalGrades[i];
    return -1;
}

void Student::viewTranscript() const {
    printHeader("TRANSCRIPT — " + name + " (" + ID + ")");
    cout << "Type: " << getType() << "\nEmail: " << email << endl;
    printLine();
    if (courseCount == 0) {
        cout << "No courses enrolled." << endl;
    }
    else {
        cout << left << setw(12) << "Course" << setw(10) << "Grade%"
            << setw(8) << "Letter" << "GPA pts" << endl;
        printLine('-', 45);
        for (int i = 0; i < courseCount; i++) {
            cout << left << setw(12) << enrolledCourseIDs[i];
            if (!gradeSet[i]) {
                cout << setw(10) << "N/A" << setw(8) << "N/A" << "N/A";
            }
            else if (getType() == "Exchange") {
                cout << setw(10) << fixed << setprecision(1) << finalGrades[i]
                    << setw(8) << (finalGrades[i] >= 50 ? "PASS" : "FAIL") << "N/A";
            }
            else {
                cout << setw(10) << fixed << setprecision(1) << finalGrades[i]
                    << setw(8) << letterGrade(finalGrades[i])
                    << fixed << setprecision(2) << percentToGPA(finalGrades[i]);
            }
            cout << endl;
        }
    }
    printLine();
}

void Student::displayProfile() const {
    cout << left << setw(10) << ID << setw(25) << name << setw(30) << email
        << setw(20) << getType();
    if (getGPA() >= 0 && getType() != "Exchange")
        cout << "GPA: " << fixed << setprecision(2) << getGPA();
    cout << endl;
}

RegularStudent::RegularStudent() : gpa(0) {}
RegularStudent::RegularStudent(const string& id, const string& n, const string& e, double g)
    : Student(id, n, e), gpa(g) {
}

double RegularStudent::calculateGPA() {
    double total = 0; int cnt = 0;
    for (int i = 0; i < courseCount; i++) {
        if (gradeSet[i]) {
            total += percentToGPA(finalGrades[i]);
            cnt++;
        }
    }
    gpa = (cnt > 0) ? total / cnt : 0;
    return gpa;
}

ScholarshipStudent::ScholarshipStudent() : gpa(0), minGPA(3.0), probation(false) {}
ScholarshipStudent::ScholarshipStudent(const string& id, const string& n, const string& e,
    double g, double mn)
    : Student(id, n, e), gpa(g), minGPA(mn), probation(false) {
    if (g > 0 && g < mn) probation = true;
}

double ScholarshipStudent::calculateGPA() {
    double total = 0; int cnt = 0;
    for (int i = 0; i < courseCount; i++) {
        if (gradeSet[i]) {
            total += percentToGPA(finalGrades[i]);
            cnt++;
        }
    }
    gpa = (cnt > 0) ? total / cnt : 0;
    probation = (gpa < minGPA && gpa > 0);
    return gpa;
}

void ScholarshipStudent::checkProbation() {
    probation = (gpa > 0 && gpa < minGPA);
    if (probation) {
        cout << "\n[!] ALERT: " << name << " is on PROBATION (GPA "
            << fixed << setprecision(2) << gpa << " < " << minGPA << ")\n";
    }
}

string ScholarshipStudent::getType() const {
    return probation ? "Scholarship(Probation)" : "Scholarship";
}

void ScholarshipStudent::setGPA(double g) {
    gpa = g;
    probation = (g > 0 && g < minGPA);
}

ExchangeStudent::ExchangeStudent() {}
ExchangeStudent::ExchangeStudent(const string& id, const string& n, const string& e)
    : Student(id, n, e) {
}