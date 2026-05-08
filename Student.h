#ifndef STUDENT_H
#define STUDENT_H

#include "AcademicEntity.h"
#include "Constants.h"
#include <string>

class Student : public AcademicEntity {
protected:
    std::string enrolledCourseIDs[MAX_ENROLLMENTS];
    double finalGrades[MAX_ENROLLMENTS];
    bool gradeSet[MAX_ENROLLMENTS];
    int courseCount;

public:
    Student();
    Student(const std::string& id, const std::string& n, const std::string& e);

    bool isEnrolledIn(const std::string& cid) const;
    bool enrollCourse(const std::string& cid);
    bool unenrollCourse(const std::string& cid);
    void setFinalGrade(const std::string& cid, double grade);
    double getFinalGrade(const std::string& cid) const;

    int getCourseCount() const { return courseCount; }
    std::string getCourseAt(int i) const { return enrolledCourseIDs[i]; }
    double getGradeAt(int i) const { return finalGrades[i]; }
    bool isGradeSet(int i) const { return gradeSet[i]; }

    virtual double calculateGPA() = 0;
    virtual std::string getType() const = 0;
    virtual double getGPA() const = 0;
    virtual void setGPA(double g) = 0;

    void viewTranscript() const;
    void displayProfile() const override;

    
};

class RegularStudent : public Student {
    double gpa;
public:
    RegularStudent();
    RegularStudent(const std::string& id, const std::string& n, const std::string& e, double g = 0);
    double calculateGPA() override;
    std::string getType() const override { return "Regular"; }
    double getGPA() const override { return gpa; }
    void setGPA(double g) override { gpa = g; }
};

class ScholarshipStudent : public Student {
    double gpa;
    double minGPA;
    bool probation;
public:
    ScholarshipStudent();
    ScholarshipStudent(const std::string& id, const std::string& n, const std::string& e,
        double g = 0, double mn = 3.0);
    double calculateGPA() override;
    void checkProbation();
    std::string getType() const override;
    double getGPA() const override { return gpa; }
    void setGPA(double g) override;
    double getMinGPA() const { return minGPA; }
    bool isOnProbation() const { return probation; }
};

class ExchangeStudent : public Student {
public:
    ExchangeStudent();
    ExchangeStudent(const std::string& id, const std::string& n, const std::string& e);
    double calculateGPA() override { return -1; }
    std::string getType() const override { return "Exchange"; }
    double getGPA() const override { return -1; }
    void setGPA(double) override {}
};

#endif