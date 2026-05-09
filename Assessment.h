#ifndef ASSESSMENT_H
#define ASSESSMENT_H

#include "Constants.h"
#include <string>

class Assessment {
protected:
    std::string assessID;
    std::string name;
    double weightage;
    std::string scoreStudentIDs[MAX_GRADES];
    double rawScores[MAX_GRADES];
    double maxScores[MAX_GRADES];
    int scoreCount;

public:
    Assessment();
    Assessment(const std::string& aid, const std::string& n, double w);
    virtual ~Assessment();

    virtual std::string getType() const = 0;
    std::string getAssessID() const { return assessID; }
    std::string getName() const { return name; }
    double getWeightage() const { return weightage; }

    void setScore(const std::string& studentID, double raw, double max);
    double getWeightedScore(const std::string& studentID) const;
    bool hasScore(const std::string& studentID) const;
    double getRaw(const std::string& sid) const;
    double getMax(const std::string& sid) const;
    int getScoreCount() const { return scoreCount; }
    std::string getScoreStudentID(int i) const { return scoreStudentIDs[i]; }

    
};

class Exam : public Assessment {
public:
    Exam();
    Exam(const std::string& aid, const std::string& n, double w);
    std::string getType() const override { return "Exam"; }
};

class Quiz : public Assessment {
public:
    Quiz();
    Quiz(const std::string& aid, const std::string& n, double w);
    std::string getType() const override { return "Quiz"; }
};

class Assignment : public Assessment {
public:
    Assignment();
    Assignment(const std::string& aid, const std::string& n, double w);
    std::string getType() const override { return "Assignment"; }
};

#endif