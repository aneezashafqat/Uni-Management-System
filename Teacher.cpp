#include "Teacher.h"
#include "Utils.h"
#include <iostream>
#include <iomanip>

using namespace std;

Teacher::Teacher() : feedbackCount(0), avgFeedback(0), courseCount(0) {}
Teacher::Teacher(const string& id, const string& n, const string& e)
    : AcademicEntity(id, n, e), feedbackCount(0), avgFeedback(0), courseCount(0) {
}

void Teacher::addFeedback(int rating, const string& comment) {
    if (feedbackCount < MAX_FEEDBACK) {
        feedbackRatings[feedbackCount] = rating;
        feedbackComments[feedbackCount] = comment;
        feedbackCount++;
        recalcAvg();
    }
}

void Teacher::recalcAvg() {
    if (feedbackCount == 0) {
        avgFeedback = 0;
        return;
    }
    double sum = 0;
    for (int i = 0; i < feedbackCount; i++) sum += feedbackRatings[i];
    avgFeedback = sum / feedbackCount;
}

void Teacher::showFeedbacks() const {
    printHeader("FEEDBACK — " + name);
    if (feedbackCount == 0) {
        cout << "No feedback yet.\n";
        return;
    }
    for (int i = 0; i < feedbackCount; i++) {
        cout << "[" << (i + 1) << "] Rating: " << feedbackRatings[i] << "/5";
        if (!feedbackComments[i].empty())
            cout << "  Comment: " << feedbackComments[i];
        cout << endl;
    }
    cout << "\nAverage Score: " << fixed << setprecision(2) << avgFeedback << "/5\n";
}

void Teacher::addCourse(const string& cid) {
    for (int i = 0; i < courseCount; i++)
        if (assignedCourseIDs[i] == cid) return;
    if (courseCount < MAX_COURSES) assignedCourseIDs[courseCount++] = cid;
}

void Teacher::displayProfile() const {
    cout << left << setw(10) << ID << setw(25) << name << setw(30) << email
        << "Avg Feedback: " << fixed << setprecision(2) << avgFeedback << "/5"
        << " (" << feedbackCount << " reviews)" << endl;
}