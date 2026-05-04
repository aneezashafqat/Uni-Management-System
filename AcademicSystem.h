#ifndef ACADEMICSYSTEM_H
#define ACADEMICSYSTEM_H

#include "Constants.h"
#include "DatabaseManager.h"
#include "Scheduler.h"
#include "Venue.h"
#include "Section.h"

class Student;
class Teacher;
class Course;

class AcademicSystem {
private:
    Student* students[MAX_STUDENTS];
    Teacher* teachers[MAX_TEACHERS];
    Course* courses[MAX_COURSES];
    Venue venues[MAX_VENUES];
    Section sections[MAX_SECTIONS];

    int studentCount;
    int teacherCount;
    int courseCount;
    int venueCount;
    int sectionCount;

    DatabaseManager db;
    Scheduler scheduler;

    // Helper methods
    Student* findStudent(const std::string& id) const;
    Teacher* findTeacher(const std::string& id) const;
    Course* findCourse(const std::string& id) const;
    bool idExistsStudent(const std::string& id) const;
    bool idExistsTeacher(const std::string& id) const;
    bool idExistsCourse(const std::string& id) const;
    void updateStudentGPA(Student* s);
    void saveAll();

public:
    AcademicSystem();
    ~AcademicSystem();
    void loadData();

    // Student Management
    void menuStudents();
    void addStudent();
    void listStudents();
    void searchStudent();
    void updateStudent();
    void deleteStudent();
    void viewTranscript();

    // Teacher Management
    void menuTeachers();
    void addTeacher();
    void listTeachers();
    void updateTeacher();
    void deleteTeacher();
    void viewTeacherFeedback();

    // Course Management
    void menuCourses();
    void addCourse();
    void listCourses();
    void deleteCourse();
    void viewCourseEnrollments();

    // Venue Management
    void menuVenues();
    void addVenue();
    void listVenues();

    // Section Management
    void menuSections();
    void createSection();
    void listSections();

    // Registration
    void menuRegistration();
    void registerStudent();
    void dropCourse();

    // Grading
    void menuGrading();
    void enterMarks();
    void viewGrades();

    // Feedback
    void menuFeedback();
    void giveFeedback();

    // Other
    void runScheduler();
    void viewWeightages();
    void run();
};

#endif