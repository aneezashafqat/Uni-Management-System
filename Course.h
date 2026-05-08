#ifndef COURSE_H
#define COURSE_H

#include "Constants.h"
#include <string>

class Student;
class Assessment;

class Course {
protected:
    std::string courseID;
    std::string title;
    std::string teacherID;
    std::string type;
    Student* enrolledStudents[MAX_COURSE_ENROLL];
    int enrollmentCount;
    Assessment* assessments[MAX_ASSESSMENTS];
    int assessmentCount;

public:
    Course();
    Course(const std::string& cid, const std::string& t, const std::string& tid, const std::string& typ);
    virtual ~Course();

    std::string getCourseID() const { return courseID; }
    std::string getTitle() const { return title; }
    std::string getTeacherID() const { return teacherID; }
    std::string getType() const { return type; }
    int getEnrollmentCount() const { return enrollmentCount; }

    bool isStudentEnrolled(const std::string& sid) const;
    bool addStudent(Student* s);
    bool removeStudent(const std::string& sid);
    Student* getStudentAt(int i) const { return enrolledStudents[i]; }

    void addAssessment(Assessment* a);
    int getAssessmentCount() const { return assessmentCount; }
    Assessment* getAssessmentAt(int i) const { return assessments[i]; }

    double calculateFinalGrade(const std::string& studentID) const;
    void display() const;
    void showAssessments() const;

    virtual int getExamDuration() const = 0;
    virtual bool requiresComputers() const = 0;
};

class CoreCourse : public Course {
public:
    CoreCourse() {}
    CoreCourse(const std::string& c, const std::string& t, const std::string& tid)
        : Course(c, t, tid, "Core") {
    }
    int getExamDuration() const override { return 3; }
    bool requiresComputers() const override { return false; }
};

class ElectiveCourse : public Course {
public:
    ElectiveCourse() {}
    ElectiveCourse(const std::string& c, const std::string& t, const std::string& tid)
        : Course(c, t, tid, "Elective") {
    }
    int getExamDuration() const override { return 2; }
    bool requiresComputers() const override { return false; }
};

class LabCourse : public Course {
public:
    LabCourse() {}
    LabCourse(const std::string& c, const std::string& t, const std::string& tid)
        : Course(c, t, tid, "Lab") {
    }
    int getExamDuration() const override { return 0; }
    bool requiresComputers() const override { return true; }
};

#endif