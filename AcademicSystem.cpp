#include "AcademicSystem.h"  //eventually will make these classes
/* 
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "Assessment.h"
#include "Utils.h"
#include "WeightageConfig.h" 
*/
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

//constructor nulls everything 
AcademicSystem::AcademicSystem() : studentCount(0), teacherCount(0), courseCount(0),
venueCount(0), sectionCount(0), db("data/") {
    for (int i = 0; i < MAX_STUDENTS; i++) students[i] = NULL;
    for (int i = 0; i < MAX_TEACHERS; i++) teachers[i] = NULL;
    for (int i = 0; i < MAX_COURSES; i++) courses[i] = NULL;
}

//destructor to clean up memory 
AcademicSystem::~AcademicSystem() {
    for (int i = 0; i < studentCount; i++) delete students[i];
    for (int i = 0; i < teacherCount; i++) delete teachers[i];
    for (int i = 0; i < courseCount; i++) delete courses[i];
}

//null if not found and all those checks 
Student* AcademicSystem::findStudent(const string& id) const {
    for (int i = 0; i < studentCount; i++)
        if (students[i] && students[i]->getID() == id) return students[i];
    return NULL;
}

//same function w teacher
Teacher* AcademicSystem::findTeacher(const string& id) const {
    for (int i = 0; i < teacherCount; i++)
        if (teachers[i] && teachers[i]->getID() == id) return teachers[i];
    return NULL;
}

//null if course not found
Course* AcademicSystem::findCourse(const string& id) const {
    for (int i = 0; i < courseCount; i++)
        if (courses[i] && courses[i]->getCourseID() == id) return courses[i];
    return NULL;
}

//checker if ID exsits or not 
bool AcademicSystem::idExistsStudent(const string& id) const {
    return findStudent(id) != NULL;
}

bool AcademicSystem::idExistsTeacher(const string& id) const {
    return findTeacher(id) != NULL;
}

bool AcademicSystem::idExistsCourse(const string& id) const {
    return findCourse(id) != NULL;
}

//updating grades
void AcademicSystem::updateStudentGPA(Student* s) {
    if (!s) return;
    s->calculateGPA();
    ScholarshipStudent* ss = dynamic_cast<ScholarshipStudent*>(s);
    if (ss) ss->checkProbation();
}

//file handling --> saving data to diff files
void AcademicSystem::saveAll() {
    db.saveStudents(students, studentCount);
    db.saveTeachers(teachers, teacherCount);
    db.saveCourses(courses, courseCount);
    db.saveVenues(venues, venueCount);
    db.saveSections(sections, sectionCount);
    db.saveAssessmentScores(courses, courseCount);
}


//loading data from files
void AcademicSystem::loadData() {
    WeightageConfig::loadFromFile("data/weightages.txt");
    studentCount = db.loadStudents(students);
    teacherCount = db.loadTeachers(teachers);
    courseCount = db.loadCourses(courses);
    venueCount = db.loadVenues(venues);
    sectionCount = db.loadSections(sections);
    db.loadAssessmentScores(courses, courseCount);
    //diplaying data 
    cout << "\nData loaded - Students: " << studentCount
        << " | Teachers: " << teacherCount
        << " | Courses: " << courseCount
        << " | Venues: " << venueCount
        << " | Sections: " << sectionCount << endl;
}
