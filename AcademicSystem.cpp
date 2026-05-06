#include "AcademicSystem.h"  //eventually will make these classes
#include "Student.h"

/* 
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

//       STUDENT MANAGEMENT 

void AcademicSystem::menuStudents() {
    int ch;
    do {  //display options 
        printHeader("STUDENT MANAGEMENT");
        cout << "  1. Add Student\n"
            << "  2. List All Students\n"
            << "  3. Search Student\n"
            << "  4. Update Student\n"
            << "  5. Delete Student\n"
            << "  6. View Transcript\n"
            << "  0. Back\n";
        printLine();
        ch = safeIntInput("Choice: ", 0, 6);
        switch (ch) {
        case 1: addStudent(); break;
        case 2: listStudents(); break;
        case 3: searchStudent(); break;
        case 4: updateStudent(); break;
        case 5: deleteStudent(); break;
        case 6: viewTranscript(); break;
        }
    } while (ch != 0);
}

//adding students and checking limits 
void AcademicSystem::addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << "ERROR: Maximum students reached (" << MAX_STUDENTS << ")!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW STUDENT");

    string id, nm, em, tp;

// makinf sure no duplicates 
    while (true) {
        cout << "Enter Student ID (numbers only): ";
        cin >> id;
        id = trim(id);

        if (id.empty()) {
            cout << "ERROR: ID cannot be empty!\n";
            continue;
        }

        if (id[0] == '-') {
            cout << "ERROR: ID cannot be negative!\n";
            continue;
        }

        bool allDigits = true;
        for (size_t i = 0; i < id.length(); i++) {
            if (!isdigit(id[i])) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << "ERROR: ID must contain ONLY numbers (0-9)!\n";
            cout << "       No letters, spaces, or special characters allowed.\n";
            continue;
        }

        if (idExistsStudent(id)) {
            cout << "ERROR: ID '" << id << "' already exists!\n";
            continue;
        }

        break;
    }

    cin.ignore();

    // NAME check + validation 
    while (true) {
        cout << "Enter Name (letters and spaces only): ";
        getline(cin, nm);
        nm = trim(nm);

        if (nm.empty()) {
            cout << "ERROR: Name cannot be empty!\n";
            continue;
        }

        bool validName = true;
        for (size_t i = 0; i < nm.length(); i++) {
            char c = nm[i];
            if (!isalpha(c) && c != ' ' && c != '.' && c != '-' && c != '\'') {
                validName = false;
                break;
            }
        }

        if (!validName) {
            cout << "ERROR: Name can only contain LETTERS, spaces, dots (.), hyphens (-), and apostrophes (')\n";
            cout << "       No numbers or special characters allowed!\n";
            continue;
        }

        break;
    }

    // email formatting 
    while (true) {
        cout << "Enter Email (name@domain.com): ";
        getline(cin, em);
        em = trim(em);

        if (em.empty()) {
            cout << "ERROR: Email cannot be empty!\n";
            continue;
        }

        bool hasAt = false;
        bool hasDot = false;
        int atPos = -1;
        bool emailValid = true;

        for (size_t i = 0; i < em.length(); i++) {
            if (em[i] == '@') {
                if (hasAt) {
                    cout << "ERROR: Email cannot have multiple '@' symbols!\n";
                    emailValid = false;
                    break;
                }
                hasAt = true;
                atPos = i;
            }
            if (hasAt && em[i] == '.' && i > (size_t)atPos) {
                hasDot = true;
            }
        }

        if (!emailValid) continue;

        if (!hasAt) {
            cout << "ERROR: Email must contain '@' symbol!\n";
            cout << "       Example: username@domain.com\n";
            continue;
        }

        if (!hasDot) {
            cout << "ERROR: Email must have a domain extension (e.g., .com, .edu)!\n";
            continue;
        }

        if (atPos == 0) {
            cout << "ERROR: Email cannot start with '@'!\n";
            continue;
        }

        if (atPos == (int)em.length() - 1) {
            cout << "ERROR: Email cannot end with '@'!\n";
            continue;
        }

        break;
    }

    // student type validation 
    while (true) {
        cout << "Enter Type (Regular/Scholarship/Exchange): ";
        cin >> tp;
        tp = trim(tp);

        if (tp == "Regular" || tp == "regular" || tp == "REGULAR") {
            tp = "Regular";
            break;
        }
        else if (tp == "Scholarship" || tp == "scholarship" || tp == "SCHOLARSHIP") {
            tp = "Scholarship";
            break;
        }
        else if (tp == "Exchange" || tp == "exchange" || tp == "EXCHANGE") {
            tp = "Exchange";
            break;
        }
        else {
            cout << "ERROR: Invalid type! Please enter Regular, Scholarship, or Exchange\n";
        }
    }

        // creating appropriate student based on type
    Student* s = NULL;

    if (tp == "Regular") {
        double g = safeDoubleInput("Enter GPA (0.0 - 4.0): ", 0.0, 4.0);
        s = new RegularStudent(id, nm, em, g);
        cout << "\n[SUCCESS] Regular Student added successfully!\n";
    }
    else if (tp == "Scholarship") {
        double g = safeDoubleInput("Enter GPA (0.0 - 4.0): ", 0.0, 4.0);
        double mn = safeDoubleInput("Enter Minimum GPA required (0.0 - 4.0): ", 0.0, 4.0);
        s = new ScholarshipStudent(id, nm, em, g, mn);
        cout << "\n[SUCCESS] Scholarship Student added successfully!\n";
    }
    else {
        s = new ExchangeStudent(id, nm, em);
        cout << "\n[SUCCESS] Exchange Student added successfully!\n";
    }

        // adding + save
    students[studentCount++] = s;

    //display for confirmation and review 
    cout << "\n========================================\n";
    cout << "Student Details:\n";
    cout << "  ID    : " << id << "\n";
    cout << "  Name  : " << nm << "\n";
    cout << "  Email : " << em << "\n";
    cout << "  Type  : " << tp << "\n";
    cout << "========================================\n";

    saveAll();
    pauseScreen();
}
// listing all students 
void AcademicSystem::listStudents() {
    printHeader("ALL STUDENTS");
    if (studentCount == 0) {
        cout << "No students found.\n";
        pauseScreen();
        return;
    }
    //pritning headers
    cout << left << setw(10) << "ID" << setw(25) << "Name"
        << setw(30) << "Email" << setw(20) << "Type" << "GPA\n";
    printLine();
    //printing students 
    for (int i = 0; i < studentCount; i++) {
        students[i]->displayProfile();
    }
    pauseScreen();
}

//searching thru ID
void AcademicSystem::searchStudent() {
    printHeader("SEARCH STUDENT");
    string id;
    cout << "Enter Student ID: ";
    cin >> id;
    Student* s = findStudent(id);
    if (!s) {
        cout << "ERROR: Student not found.\n";
    }
    else {
        s->displayProfile();
    }
    pauseScreen();
}

//updating info on an students that exists 
void AcademicSystem::updateStudent() {
    printHeader("UPDATE STUDENT");
    string id;
    cout << "Enter Student ID: ";
    cin >> id;
    Student* s = findStudent(id);
    if (!s) {
        cout << "ERROR: Student not found.\n";
        pauseScreen();
        return;
    }
//current info to recheck and ease of the user 
    cout << "\nCurrent Information:\n";
    s->displayProfile();
    cout << "\n";
//checking what needs to be updated 
    string nm, em;
    cin.ignore();
    cout << "New Name (press Enter to keep current): ";
    getline(cin, nm);
    cout << "New Email (press Enter to keep current): ";
    getline(cin, em);
//string manipulation
    if (!trim(nm).empty()) s->setName(nm);
    if (!trim(em).empty()) s->setEmail(em);

    cout << "\n[SUCCESS] Student updated successfully!\n";
    saveAll();
    pauseScreen();
}

// deleting a student
//fidning the index by comparing the ID
void AcademicSystem::deleteStudent() {
    printHeader("DELETE STUDENT");
    string id;
    cout << "Enter Student ID: ";
    cin >> id;

    int index = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i] && students[i]->getID() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "ERROR: Student not found!\n";
        pauseScreen();
        return;
    }

    Student* s = students[index];
    cout << "\nStudent to delete:\n";
    cout << "  ID: " << s->getID() << "\n";
    cout << "  Name: " << s->getName() << "\n";
    cout << "  Type: " << s->getType() << "\n";
//Confiarmation 
    cout << "\nAre you sure you want to delete this student? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled.\n";
        pauseScreen();
        return;
    }

    // removing from all enrolled cources 
    for (int j = 0; j < courseCount; j++) {
        if (courses[j]) courses[j]->removeStudent(id);
    }
//fixing the array after deleting 
    delete students[index];
    for (int j = index; j < studentCount - 1; j++) {
        students[j] = students[j + 1];
    }
    studentCount--;
    students[studentCount] = NULL;

    cout << "\n[SUCCESS] Student deleted successfully!\n";
    saveAll();
    pauseScreen();
}

//all records reviewal 
void AcademicSystem::viewTranscript() {
    string id;
    cout << "Enter Student ID: ";
    cin >> id;
    Student* s = findStudent(id);
    if (!s) {
        cout << "ERROR: Student not found.\n";
    }
    else {
        s->viewTranscript();
    }
    pauseScreen();
}


// teacher management system 
void AcademicSystem::menuTeachers() {
    int ch;
    do {
        printHeader("TEACHER MANAGEMENT");
        cout << "  1. Add Teacher\n"
            << "  2. List All Teachers\n"
            << "  3. Update Teacher\n"
            << "  4. Delete Teacher\n"
            << "  5. View Feedback\n"
            << "  0. Back\n";
        printLine();
        ch = safeIntInput("Choice: ", 0, 5);
        switch (ch) {
        case 1: addTeacher(); break;
        case 2: listTeachers(); break;
        case 3: updateTeacher(); break;
        case 4: deleteTeacher(); break;
        case 5: viewTeacherFeedback(); break;
        }
    } while (ch != 0);
}

void AcademicSystem::addTeacher() {
    if (teacherCount >= MAX_TEACHERS) {
        cout << "ERROR: Maximum teachers reached (" << MAX_TEACHERS << ")!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW TEACHER");

    string id, nm, em;

    while (true) {
        cout << "Enter Teacher ID (numbers only): ";
        cin >> id;
        id = trim(id);

        if (id.empty()) {
            cout << "ERROR: ID cannot be empty!\n";
            continue;
        }

        if (id[0] == '-') {
            cout << "ERROR: ID cannot be negative!\n";
            continue;
        }

        bool allDigits = true;
        for (size_t i = 0; i < id.length(); i++) {
            if (!isdigit(id[i])) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << "ERROR: ID must contain ONLY numbers (0-9)!\n";
            continue;
        }

        if (idExistsTeacher(id)) {
            cout << "ERROR: ID '" << id << "' already exists!\n";
            continue;
        }

        break;
    }

    cin.ignore();

    while (true) {
        cout << "Enter Name (letters and spaces only): ";
        getline(cin, nm);
        nm = trim(nm);

        if (nm.empty()) {
            cout << "ERROR: Name cannot be empty!\n";
            continue;
        }

        bool validName = true;
        for (size_t i = 0; i < nm.length(); i++) {
            char c = nm[i];
            if (!isalpha(c) && c != ' ' && c != '.' && c != '-' && c != '\'') {
                validName = false;
                break;
            }
        }

        if (!validName) {
            cout << "ERROR: Name can only contain LETTERS, spaces, dots, hyphens, and apostrophes\n";
            continue;
        }

        break;
    }

    while (true) {
        cout << "Enter Email (name@domain.com): ";
        getline(cin, em);
        em = trim(em);

        if (em.empty()) {
            cout << "ERROR: Email cannot be empty!\n";
            continue;
        }

        bool hasAt = false;
        bool hasDot = false;
        int atPos = -1;
        bool emailValid = true;

        for (size_t i = 0; i < em.length(); i++) {
            if (em[i] == '@') {
                if (hasAt) {
                    cout << "ERROR: Email cannot have multiple '@' symbols!\n";
                    emailValid = false;
                    break;
                }
                hasAt = true;
                atPos = i;
            }
            if (hasAt && em[i] == '.' && i > (size_t)atPos) {
                hasDot = true;
            }
        }

        if (!emailValid) continue;

        if (!hasAt) {
            cout << "ERROR: Email must contain '@' symbol!\n";
            continue;
        }

        if (!hasDot) {
            cout << "ERROR: Email must have a domain extension (e.g., .com)!\n";
            continue;
        }

        if (atPos == 0 || atPos == (int)em.length() - 1) {
            cout << "ERROR: '@' cannot be first or last character!\n";
            continue;
        }

        break;
    }

    teachers[teacherCount++] = new Teacher(id, nm, em);

    cout << "\n[SUCCESS] Teacher added successfully!\n";
    cout << "========================================\n";
    cout << "Teacher Details:\n";
    cout << "  ID    : " << id << "\n";
    cout << "  Name  : " << nm << "\n";
    cout << "  Email : " << em << "\n";
    cout << "========================================\n";

    saveAll();
    pauseScreen();
}

void AcademicSystem::listTeachers() {
    printHeader("ALL TEACHERS");
    if (teacherCount == 0) {
        cout << "No teachers found.\n";
        pauseScreen();
        return;
    }
    cout << left << setw(10) << "ID" << setw(25) << "Name"
        << setw(30) << "Email" << "Avg Feedback\n";
    printLine();
    for (int i = 0; i < teacherCount; i++) {
        teachers[i]->displayProfile();
    }
    pauseScreen();
}

void AcademicSystem::updateTeacher() {
    printHeader("UPDATE TEACHER");
    string id;
    cout << "Enter Teacher ID: ";
    cin >> id;
    Teacher* t = findTeacher(id);
    if (!t) {
        cout << "ERROR: Teacher not found.\n";
        pauseScreen();
        return;
    }

    cout << "\nCurrent Information:\n";
    t->displayProfile();
    cout << "\n";

    string nm, em;
    cin.ignore();
    cout << "New Name (press Enter to keep current): ";
    getline(cin, nm);
    cout << "New Email (press Enter to keep current): ";
    getline(cin, em);

    if (!trim(nm).empty()) t->setName(nm);
    if (!trim(em).empty()) t->setEmail(em);

    cout << "\n[SUCCESS] Teacher updated successfully!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::deleteTeacher() {
    printHeader("DELETE TEACHER");
    string id;
    cout << "Enter Teacher ID: ";
    cin >> id;

    int index = -1;
    for (int i = 0; i < teacherCount; i++) {
        if (teachers[i] && teachers[i]->getID() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "ERROR: Teacher not found!\n";
        pauseScreen();
        return;
    }

    Teacher* t = teachers[index];
    cout << "\nTeacher to delete:\n";
    cout << "  ID: " << t->getID() << "\n";
    cout << "  Name: " << t->getName() << "\n";

    cout << "\nAre you sure you want to delete this teacher? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled.\n";
        pauseScreen();
        return;
    }

    delete teachers[index];
    for (int j = index; j < teacherCount - 1; j++) {
        teachers[j] = teachers[j + 1];
    }
    teacherCount--;
    teachers[teacherCount] = NULL;

    cout << "\n[SUCCESS] Teacher deleted successfully!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::viewTeacherFeedback() {
    string id;
    cout << "Enter Teacher ID: ";
    cin >> id;
    Teacher* t = findTeacher(id);
    if (!t) {
        cout << "ERROR: Teacher not found.\n";
    }
    else {
        t->showFeedbacks();
    }
    pauseScreen();
}


//course management 
void AcademicSystem::menuCourses() {
    int ch;
    do {
        printHeader("COURSE MANAGEMENT");
        cout << "  1. Add Course\n"
            << "  2. List All Courses\n"
            << "  3. Delete Course\n"
            << "  4. View Course Enrollments\n"
            << "  0. Back\n";
        printLine();
        ch = safeIntInput("Choice: ", 0, 4);
        switch (ch) {
        case 1: addCourse(); break;
        case 2: listCourses(); break;
        case 3: deleteCourse(); break;
        case 4: viewCourseEnrollments(); break;
        }
    } while (ch != 0);
}

void AcademicSystem::addCourse() {
    if (courseCount >= MAX_COURSES) {
        cout << "ERROR: Maximum courses reached (" << MAX_COURSES << ")!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW COURSE");

    string id, title, tid, type;

    while (true) {
        cout << "Enter Course ID (numbers only): ";
        cin >> id;
        id = trim(id);

        if (id.empty()) {
            cout << "ERROR: Course ID cannot be empty!\n";
            continue;
        }

        if (id[0] == '-') {
            cout << "ERROR: Course ID cannot be negative!\n";
            continue;
        }

        bool allDigits = true;
        for (size_t i = 0; i < id.length(); i++) {
            if (!isdigit(id[i])) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << "ERROR: Course ID must contain ONLY numbers (0-9)!\n";
            continue;
        }

        if (idExistsCourse(id)) {
            cout << "ERROR: Course ID '" << id << "' already exists!\n";
            continue;
        }

        break;
    }

    cin.ignore();

    while (true) {
        cout << "Enter Course Title: ";
        getline(cin, title);
        title = trim(title);

        if (title.empty()) {
            cout << "ERROR: Title cannot be empty!\n";
            continue;
        }

        bool validTitle = true;
        for (size_t i = 0; i < title.length(); i++) {
            char c = title[i];
            if (!isalnum(c) && c != ' ' && c != '.' && c != '-' && c != '&') {
                validTitle = false;
                break;
            }
        }

        if (!validTitle) {
            cout << "ERROR: Title can only contain letters, numbers, spaces, dots, hyphens, and ampersands\n";
            continue;
        }

        break;
    }

    while (true) {
        cout << "Enter Teacher ID: ";
        getline(cin, tid);
        tid = trim(tid);

        if (tid.empty()) {
            cout << "ERROR: Teacher ID cannot be empty!\n";
            continue;
        }

        if (!findTeacher(tid)) {
            cout << "ERROR: Teacher with ID '" << tid << "' not found!\n";
            cout << "       Please add the teacher first.\n";
            continue;
        }

        break;
    }

    while (true) {
        cout << "Enter Type (Core/Elective/Lab): ";
        cin >> type;
        type = trim(type);

        if (type == "Core" || type == "core" || type == "CORE") {
            type = "Core";
            break;
        }
        else if (type == "Elective" || type == "elective" || type == "ELECTIVE") {
            type = "Elective";
            break;
        }
        else if (type == "Lab" || type == "lab" || type == "LAB") {
            type = "Lab";
            break;
        }
        else {
            cout << "ERROR: Invalid type! Please enter Core, Elective, or Lab\n";
        }
    }

    Course* c = NULL;
    if (type == "Core") {
        c = new CoreCourse(id, title, tid);
    }
    else if (type == "Elective") {
        c = new ElectiveCourse(id, title, tid);
    }
    else {
        c = new LabCourse(id, title, tid);
    }

    double ew = WeightageConfig::getExamW(type);
    double aw = WeightageConfig::getAssignW(type);
    double qw = WeightageConfig::getQuizW(type);

    if (ew > 0) c->addAssessment(new Exam(id + "_EX", "Final Exam", ew));
    if (aw > 0) c->addAssessment(new Assignment(id + "_AS", "Assignment", aw));
    if (qw > 0) c->addAssessment(new Quiz(id + "_QZ", "Quiz", qw));

    courses[courseCount++] = c;
    findTeacher(tid)->addCourse(id);

    cout << "\n[SUCCESS] Course added successfully!\n";
    cout << "========================================\n";
    cout << "Course Details:\n";
    cout << "  ID      : " << id << "\n";
    cout << "  Title   : " << title << "\n";
    cout << "  Teacher : " << tid << "\n";
    cout << "  Type    : " << type << "\n";
    cout << "========================================\n";

    saveAll();
    pauseScreen();
}

void AcademicSystem::listCourses() {
    printHeader("ALL COURSES");
    if (courseCount == 0) {
        cout << "No courses found.\n";
        pauseScreen();
        return;
    }
    cout << left << setw(10) << "ID" << setw(30) << "Title"
        << setw(12) << "Type" << setw(12) << "Teacher" << "Students\n";
    printLine();
    for (int i = 0; i < courseCount; i++) {
        courses[i]->display();
    }
    pauseScreen();
}

void AcademicSystem::deleteCourse() {
    printHeader("DELETE COURSE");
    string id;
    cout << "Enter Course ID: ";
    cin >> id;

    int index = -1;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i] && courses[i]->getCourseID() == id) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        cout << "ERROR: Course not found!\n";
        pauseScreen();
        return;
    }

    Course* c = courses[index];
    cout << "\nCourse to delete:\n";
    cout << "  ID: " << c->getCourseID() << "\n";
    cout << "  Title: " << c->getTitle() << "\n";
    cout << "  Enrolled Students: " << c->getEnrollmentCount() << "\n";

    if (c->getEnrollmentCount() > 0) {
        cout << "\nWARNING: This course has " << c->getEnrollmentCount()
            << " enrolled students!\n";
    }

    cout << "\nAre you sure you want to delete this course? (y/n): ";
    char confirm;
    cin >> confirm;
    if (tolower(confirm) != 'y') {
        cout << "Deletion cancelled.\n";
        pauseScreen();
        return;
    }

    for (int i = 0; i < c->getEnrollmentCount(); i++) {
        Student* s = c->getStudentAt(i);
        if (s) s->unenrollCourse(c->getCourseID());
    }

    // Remove associated sections
    for (int i = 0; i < sectionCount; i++) {
        if (sections[i].getCourseID() == id) {
            for (int j = i; j < sectionCount - 1; j++) {
                sections[j] = sections[j + 1];
            }
            sectionCount--;
            i--;
        }
    }

    delete courses[index];
    for (int j = index; j < courseCount - 1; j++) {
        courses[j] = courses[j + 1];
    }
    courseCount--;
    courses[courseCount] = NULL;

    cout << "\n[SUCCESS] Course deleted successfully!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::viewCourseEnrollments() {
    string id;
    cout << "Enter Course ID: ";
    cin >> id;
    Course* c = findCourse(id);
    if (!c) {
        cout << "ERROR: Course not found.\n";
        pauseScreen();
        return;
    }

    printHeader("ENROLLMENTS - " + c->getTitle());
    cout << "Type: " << c->getType()
        << " | Teacher: " << c->getTeacherID()
        << " | Enrolled: " << c->getEnrollmentCount() << "\n";
    printLine();

    if (c->getEnrollmentCount() == 0) {
        cout << "No students enrolled.\n";
    }
    else {
        for (int i = 0; i < c->getEnrollmentCount(); i++) {
            Student* s = c->getStudentAt(i);
            if (s) cout << "  " << s->getID() << "  " << s->getName() << "\n";
        }
    }
    pauseScreen();
} 

void AcademicSystem::menuVenues() {
    int ch;
    do {
        printHeader("VENUE MANAGEMENT");
        cout << "  1. Add Venue\n"
            << "  2. List All Venues\n"
            << "  0. Back\n";
        printLine();
        ch = safeIntInput("Choice: ", 0, 2);
        switch (ch) {
        case 1: addVenue(); break;
        case 2: listVenues(); break;
        }
    } while (ch != 0);
}

void AcademicSystem::addVenue() {
    if (venueCount >= MAX_VENUES) {
        cout << "ERROR: Maximum venues reached (" << MAX_VENUES << ")!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW VENUE");

    string rid;
    int cap, hc;

    while (true) {
        cout << "Enter Room ID (e.g., A101, LAB1, 101): ";
        cin >> rid;
        rid = trim(rid);

        if (rid.empty()) {
            cout << "ERROR: Room ID cannot be empty!\n";
            continue;
        }

        bool duplicate = false;
        for (int i = 0; i < venueCount; i++) {
            if (venues[i].getRoomID() == rid) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "ERROR: Room ID '" << rid << "' already exists!\n";
            continue;
        }

        break;
    }

    cap = safeIntInput("Enter Capacity (1-1000): ", 1, 1000);

    hc = safeIntInput("Has Computers? (1=Yes, 0=No): ", 0, 1);

    venues[venueCount++] = Venue(rid, cap, hc == 1);

    cout << "\n[SUCCESS] Venue added successfully!\n";
    cout << "========================================\n";
    cout << "Venue Details:\n";
    cout << "  Room ID    : " << rid << "\n";
    cout << "  Capacity   : " << cap << "\n";
    cout << "  Computers  : " << (hc == 1 ? "Yes" : "No") << "\n";
    cout << "========================================\n";

    saveAll();
    pauseScreen();
}

void AcademicSystem::listVenues() {
    printHeader("ALL VENUES");
    if (venueCount == 0) {
        cout << "No venues found.\n";
        pauseScreen();
        return;
    }
    cout << left << setw(12) << "Room ID" << setw(12) << "Capacity" << "Computers\n";
    printLine();
    for (int i = 0; i < venueCount; i++) {
        venues[i].display();
    }
    pauseScreen();
}

//section system 
void AcademicSystem::menuSections() {
    int ch;
    do {
        printHeader("SECTION MANAGEMENT");
        cout << "  1. Create Section\n"
            << "  2. List All Sections\n"
            << "  0. Back\n";
        printLine();
        ch = safeIntInput("Choice: ", 0, 2);
        switch (ch) {
        case 1: createSection(); break;
        case 2: listSections(); break;
        }
    } while (ch != 0);
}

void AcademicSystem::createSection() {
    if (sectionCount >= MAX_SECTIONS) {
        cout << "ERROR: Maximum sections reached (" << MAX_SECTIONS << ")!\n";
        pauseScreen();
        return;
    }

    printHeader("CREATE NEW SECTION");

    string sid, cid, tid, vid, ts;

    while (true) {
        cout << "Enter Section ID (numbers only): ";
        cin >> sid;
        sid = trim(sid);

        if (sid.empty()) {
            cout << "ERROR: Section ID cannot be empty!\n";
            continue;
        }

        if (sid[0] == '-') {
            cout << "ERROR: Section ID cannot be negative!\n";
            continue;
        }

        bool allDigits = true;
        for (size_t i = 0; i < sid.length(); i++) {
            if (!isdigit(sid[i])) {
                allDigits = false;
                break;
            }
        }

        if (!allDigits) {
            cout << "ERROR: Section ID must contain ONLY numbers (0-9)!\n";
            continue;
        }

        bool duplicate = false;
        for (int i = 0; i < sectionCount; i++) {
            if (sections[i].getSectionID() == sid) {
                duplicate = true;
                break;
            }
        }
        if (duplicate) {
            cout << "ERROR: Section ID '" << sid << "' already exists!\n";
            continue;
        }

        break;
    }

    while (true) {
        cout << "Enter Course ID: ";
        cin >> cid;
        cid = trim(cid);

        if (!findCourse(cid)) {
            cout << "ERROR: Course not found! Please enter a valid Course ID.\n";
            continue;
        }
        break;
    }

    while (true) {
        cout << "Enter Teacher ID: ";
        cin >> tid;
        tid = trim(tid);

        if (!findTeacher(tid)) {
            cout << "ERROR: Teacher not found! Please enter a valid Teacher ID.\n";
            continue;
        }
        break;
    }

    if (venueCount == 0) {
        cout << "\n========================================\n";
        cout << "ERROR: No venues available in the system!\n";
        cout << "You must create a venue before creating a section.\n";
        cout << "========================================\n\n";

        cout << "Would you like to create a venue now? (y/n): ";
        char createVenue;
        cin >> createVenue;

        if (tolower(createVenue) == 'y') {
            addVenue();  
            cout << "\nVenue created. Do you want to continue creating this section? (y/n): ";
            char continueSection;
            cin >> continueSection;
            if (tolower(continueSection) != 'y') {
                cout << "Section creation cancelled.\n";
                pauseScreen();
                return;
            }
        }
        else {
            cout << "Section creation cancelled. Please add a venue first from Venue Management menu.\n";
            pauseScreen();
            return;
        }
    }

    while (true) {
        cout << "\nAvailable Venues:\n";
        cout << left << setw(12) << "Venue ID" << setw(12) << "Capacity" << "Computers\n";
        printLine('-', 35);
        for (int i = 0; i < venueCount; i++) {
            venues[i].display();
        }
        printLine('-', 35);

        cout << "\nEnter Venue ID from the list above: ";
        cin >> vid;
        vid = trim(vid);

        bool venueFound = false;
        for (int i = 0; i < venueCount; i++) {
            if (venues[i].getRoomID() == vid) {
                venueFound = true;
                break;
            }
        }

        if (!venueFound) {
            cout << "ERROR: Venue ID '" << vid << "' not found!\n";
            cout << "Please enter a Venue ID from the list above.\n";
            continue;
        }
        break;
    }

    cin.ignore();

    while (true) {
        cout << "Enter Time Slot (e.g., Mon 09:00-12:00) or press Enter for TBD: ";
        getline(cin, ts);
        ts = trim(ts);

        if (ts.empty()) {
            ts = "TBD";
            break;
        }

        if (ts.length() >= 10) {
            break;
        }
        else {
            cout << "ERROR: Invalid time slot format. Use format like 'Mon 09:00-12:00'\n";
        }
    }

    sections[sectionCount++] = Section(sid, cid, tid, vid, ts);

    cout << "\n[SUCCESS] Section created successfully!\n";
    cout << "========================================\n";
    cout << "Section Details:\n";
    cout << "  Section ID : " << sid << "\n";
    cout << "  Course ID  : " << cid << "\n";
    cout << "  Teacher ID : " << tid << "\n";
    cout << "  Venue ID   : " << vid << "\n";
    cout << "  Time Slot  : " << ts << "\n";
    cout << "========================================\n";

    saveAll();
    pauseScreen();
}

void AcademicSystem::listSections() {
    printHeader("ALL SECTIONS");
    if (sectionCount == 0) {
        cout << "No sections found.\n";
        pauseScreen();
        return;
    }
    cout << left << setw(12) << "Section ID" << setw(10) << "Course"
        << setw(12) << "Teacher" << setw(12) << "Venue"
        << setw(18) << "Time Slot" << "Students\n";
    printLine();
    for (int i = 0; i < sectionCount; i++) {
        sections[i].display();
    }
    pauseScreen();
}
