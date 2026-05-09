#include "Assessment.h"

using namespace std;

Assessment::Assessment() : weightage(0), scoreCount(0) {}
Assessment::Assessment(const string& aid, const string& n, double w)
    : assessID(aid), name(n), weightage(w), scoreCount(0) {
}
Assessment::~Assessment() {}

void Assessment::setScore(const string& studentID, double raw, double max) {
    for (int i = 0; i < scoreCount; i++) {
        if (scoreStudentIDs[i] == studentID) {
            rawScores[i] = raw;
            maxScores[i] = max;
            return;
        }
    }
    if (scoreCount < MAX_GRADES) {
        scoreStudentIDs[scoreCount] = studentID;
        rawScores[scoreCount] = raw;
        maxScores[scoreCount] = max;
        scoreCount++;
    }
}

double Assessment::getWeightedScore(const string& studentID) const {
    for (int i = 0; i < scoreCount; i++) {
        if (scoreStudentIDs[i] == studentID) {
            if (maxScores[i] <= 0) return 0;
            return (rawScores[i] / maxScores[i]) * weightage;
        }
    }
    return 0;
}

bool Assessment::hasScore(const string& studentID) const {
    for (int i = 0; i < scoreCount; i++)
        if (scoreStudentIDs[i] == studentID) return true;
    return false;
}

double Assessment::getRaw(const string& sid) const {
    for (int i = 0; i < scoreCount; i++)
        if (scoreStudentIDs[i] == sid) return rawScores[i];
    return -1;
}

double Assessment::getMax(const string& sid) const {
    for (int i = 0; i < scoreCount; i++)
        if (scoreStudentIDs[i] == sid) return maxScores[i];
    return 100;
}

Exam::Exam() {}
Exam::Exam(const string& aid, const string& n, double w) : Assessment(aid, n, w) {}

Quiz::Quiz() {}
Quiz::Quiz(const string& aid, const string& n, double w) : Assessment(aid, n, w) {}

Assignment::Assignment() {}
Assignment::Assignment(const string& aid, const string& n, double w) : Assessment(aid, n, w) {}