#ifndef DATABASE_MANAGER_H
#define DATABASE_MANAGER_H

#include <string>

class Student;
class Teacher;
class Course;
class Venue;
class Section;

class DatabaseManager {
    std::string dir;
    std::string path(const std::string& fn) const;

public:
    explicit DatabaseManager(const std::string& d = "data/");

    void saveStudents(Student* arr[], int count);
    int loadStudents(Student* arr[]);
    void saveTeachers(Teacher* arr[], int count);
    int loadTeachers(Teacher* arr[]);
    void saveCourses(Course* arr[], int count);
    int loadCourses(Course* arr[]);
    void saveVenues(Venue arr[], int count);
    int loadVenues(Venue arr[]);
    void saveSections(Section arr[], int count);
    int loadSections(Section arr[]);
    void saveAssessmentScores(Course* courses[], int courseCount);
    void loadAssessmentScores(Course* courses[], int courseCount);
    void saveSchedule(const std::string& scheduleText);
};

#endif