#include "AcademicSystem.h" //added
#include "Student.h" //added
#include "Teacher.h" //added
#include "Course.h" //added
#include "Assessment.h" //left
#include "Utils.h" //added
#include "WeightageConfig.h" //left
#include <iostream>
#include <iomanip>

using namespace std;


AcademicSystem::AcademicSystem() : studentCount(0), teacherCount(0), courseCount(0),
venueCount(0), sectionCount(0), db("data/") {
    for (int i = 0; i < MAX_STUDENTS; i++) students[i] = NULL;
    for (int i = 0; i < MAX_TEACHERS; i++) teachers[i] = NULL;
    for (int i = 0; i < MAX_COURSES; i++) courses[i] = NULL;
}

AcademicSystem::~AcademicSystem() {
    for (int i = 0; i < studentCount; i++) delete students[i];
    for (int i = 0; i < teacherCount; i++) delete teachers[i];
    for (int i = 0; i < courseCount; i++) delete courses[i];
}


Student* AcademicSystem::findStudent(const string& id) const {
    for (int i = 0; i < studentCount; i++)
        if (students[i] && students[i]->getID() == id) return students[i];
    return NULL;
}

Teacher* AcademicSystem::findTeacher(const string& id) const {
    for (int i = 0; i < teacherCount; i++)
        if (teachers[i] && teachers[i]->getID() == id) return teachers[i];
    return NULL;
}

Course* AcademicSystem::findCourse(const string& id) const {
    for (int i = 0; i < courseCount; i++)
        if (courses[i] && courses[i]->getCourseID() == id) return courses[i];
    return NULL;
}

bool AcademicSystem::idExistsStudent(const string& id) const { return findStudent(id) != NULL; }
bool AcademicSystem::idExistsTeacher(const string& id) const { return findTeacher(id) != NULL; }
bool AcademicSystem::idExistsCourse(const string& id) const { return findCourse(id) != NULL; }

void AcademicSystem::updateStudentGPA(Student* s) {
    if (!s) return;
    s->calculateGPA();
    ScholarshipStudent* ss = dynamic_cast<ScholarshipStudent*>(s);
    if (ss) ss->checkProbation();
}

void AcademicSystem::saveAll() {
    db.saveStudents(students, studentCount);
    db.saveTeachers(teachers, teacherCount);
    db.saveCourses(courses, courseCount);
    db.saveVenues(venues, venueCount);
    db.saveSections(sections, sectionCount);
    db.saveAssessmentScores(courses, courseCount);
}

void AcademicSystem::loadData() {
    WeightageConfig::loadFromFile("data/weightages.txt");
    studentCount = db.loadStudents(students);
    teacherCount = db.loadTeachers(teachers);
    courseCount = db.loadCourses(courses);
    venueCount = db.loadVenues(venues);
    sectionCount = db.loadSections(sections);
    db.loadAssessmentScores(courses, courseCount);

    cout << "\nData loaded - Students: " << studentCount
        << " | Teachers: " << teacherCount
        << " | Courses: " << courseCount
        << " | Venues: " << venueCount
        << " | Sections: " << sectionCount << endl;
}


void AcademicSystem::run() {
    loadData();
    int ch;
    do {
        printHeader("FAST ACADEMIC SYSTEM");
        cout << "  1. Student Management\n  2. Teacher Management\n  3. Course Management\n"
            << "  4. Venue Management\n  5. Section Management\n  6. Course Registration\n"
            << "  7. Grading\n  8. Teacher Feedback\n  9. Run Exam Scheduler\n"
            << " 10. View Weightage Config\n  0. Save & Exit\n";
        printLine();
        ch = safeIntInput("Choice: ", 0, 10);
        switch (ch) {
        case 1: menuStudents(); break;
        case 2: menuTeachers(); break;
        case 3: menuCourses(); break;
        case 4: menuVenues(); break;
        case 5: menuSections(); break;
        case 6: menuRegistration(); break;
        case 7: menuGrading(); break;
        case 8: menuFeedback(); break;
        case 9: runScheduler(); break;
        case 10: viewWeightages(); break;
        case 0: saveAll(); cout << "\nGoodbye!\n"; break;
        }
    } while (ch != 0);
}


void AcademicSystem::menuStudents() {
    const string options[] = { "Add Student", "List All Students", "Search Student",
                               "Update Student", "Delete Student", "View Transcript" };
    int ch;
    do {
        ch = showMenu("STUDENT MANAGEMENT", options, 6);
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

void AcademicSystem::addStudent() {
    if (studentCount >= MAX_STUDENTS) {
        cout << " Maximum students reached!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW STUDENT");

    string id = getValidatedID("Enter Student ID: ");

    if (idExistsStudent(id)) {
        cout << " ID already exists!\n";
        pauseScreen();
        return;
    }

    string name = getValidatedName("Enter Name: ");
    string email = getValidatedEmail("Enter Email: ");
    string type = getValidatedType("Enter Type (Regular/Scholarship/Exchange): ");

    Student* s = NULL;

    if (type == "Regular") {
        double g = safeDoubleInput("Enter GPA (0.0-4.0): ", 0.0, 4.0);
        s = new RegularStudent(id, name, email, g);
    }
    else if (type == "Scholarship") {
        double g = safeDoubleInput("Enter GPA (0.0-4.0): ", 0.0, 4.0);
        double mn = safeDoubleInput("Enter Minimum GPA: ", 0.0, 4.0);
        s = new ScholarshipStudent(id, name, email, g, mn);
    }
    else {
        s = new ExchangeStudent(id, name, email);
    }

    students[studentCount++] = s;
    saveAll();

    cout << "\n " << type << " Student added!\n";
    pauseScreen();
}

void AcademicSystem::listStudents() {
    printHeader("ALL STUDENTS");
    if (studentCount == 0) { cout << "No students found.\n"; pauseScreen(); return; }
    cout << left << setw(10) << "ID" << setw(25) << "Name" << setw(30) << "Email" << setw(20) << "Type" << "GPA\n";
    printLine();
    for (int i = 0; i < studentCount; i++) students[i]->displayProfile();
    pauseScreen();
}

void AcademicSystem::searchStudent() {
    printHeader("SEARCH STUDENT");
    string id; cout << "Enter Student ID: "; cin >> id;
    Student* s = findStudent(id);
    if (!s) cout << "Student not found.\n";
    else s->displayProfile();
    pauseScreen();
}

void AcademicSystem::updateStudent() {
    printHeader("UPDATE STUDENT");
    string id; cout << "Enter Student ID: "; cin >> id;
    Student* s = findStudent(id);
    if (!s) { cout << "Student not found.\n"; pauseScreen(); return; }

    cout << "\nCurrent Information:\n";
    s->displayProfile();
    cout << "\n";

    string nm, em;
    cin.ignore();
    cout << "New Name (press Enter to keep): ";
    getline(cin, nm);
    cout << "New Email (press Enter to keep): ";
    getline(cin, em);

    if (!trim(nm).empty()) s->setName(nm);
    if (!trim(em).empty()) s->setEmail(em);

    cout << "\n Student updated!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::deleteStudent() {
    printHeader("DELETE STUDENT");
    string id; cout << "Enter Student ID: "; cin >> id;

    int index = -1;
    for (int i = 0; i < studentCount; i++) {
        if (students[i] && students[i]->getID() == id) { index = i; break; }
    }

    if (index == -1) { cout << "Student not found!\n"; pauseScreen(); return; }

    Student* s = students[index];
    cout << "\nDelete " << s->getName() << "? (y/n): ";
    char confirm; cin >> confirm;
    if (tolower(confirm) != 'y') { cout << "Deletion cancelled.\n"; pauseScreen(); return; }

    for (int j = 0; j < courseCount; j++) {
        if (courses[j]) courses[j]->removeStudent(id);
    }

    delete students[index];
    for (int j = index; j < studentCount - 1; j++) students[j] = students[j + 1];
    studentCount--;

    cout << "\n Student deleted!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::viewTranscript() {
    string id; cout << "Enter Student ID: "; cin >> id;
    Student* s = findStudent(id);
    if (!s) cout << "Student not found.\n";
    else s->viewTranscript();
    pauseScreen();
}


void AcademicSystem::menuTeachers() {
    const string options[] = { "Add Teacher", "List All Teachers", "Update Teacher",
                               "Delete Teacher", "View Feedback" };
    int ch;
    do {
        ch = showMenu("TEACHER MANAGEMENT", options, 5);
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
        cout << " Maximum teachers reached!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW TEACHER");

    string id = getValidatedID("Enter Teacher ID: ");

    if (idExistsTeacher(id)) {
        cout << " ID already exists!\n";
        pauseScreen();
        return;
    }

    string name = getValidatedName("Enter Name: ");
    string email = getValidatedEmail("Enter Email: ");

    teachers[teacherCount++] = new Teacher(id, name, email);
    saveAll();

    cout << "\n Teacher added!\n";
    pauseScreen();
}

void AcademicSystem::listTeachers() {
    printHeader("ALL TEACHERS");
    if (teacherCount == 0) { cout << "No teachers found.\n"; pauseScreen(); return; }
    cout << left << setw(10) << "ID" << setw(25) << "Name" << setw(30) << "Email" << "Avg Feedback\n";
    printLine();
    for (int i = 0; i < teacherCount; i++) teachers[i]->displayProfile();
    pauseScreen();
}

void AcademicSystem::updateTeacher() {
    printHeader("UPDATE TEACHER");
    string id; cout << "Enter Teacher ID: "; cin >> id;
    Teacher* t = findTeacher(id);
    if (!t) { cout << "Teacher not found.\n"; pauseScreen(); return; }

    cout << "\nCurrent Information:\n";
    t->displayProfile();
    cout << "\n";

    string nm, em;
    cin.ignore();
    cout << "New Name (press Enter to keep): ";
    getline(cin, nm);
    cout << "New Email (press Enter to keep): ";
    getline(cin, em);

    if (!trim(nm).empty()) t->setName(nm);
    if (!trim(em).empty()) t->setEmail(em);

    cout << "\nTeacher updated!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::deleteTeacher() {
    printHeader("DELETE TEACHER");
    string id; cout << "Enter Teacher ID: "; cin >> id;

    int index = -1;
    for (int i = 0; i < teacherCount; i++) {
        if (teachers[i] && teachers[i]->getID() == id) { index = i; break; }
    }

    if (index == -1) { cout << "Teacher not found!\n"; pauseScreen(); return; }

    Teacher* t = teachers[index];
    cout << "\nDelete " << t->getName() << "? (y/n): ";
    char confirm; cin >> confirm;
    if (tolower(confirm) != 'y') { cout << "Deletion cancelled.\n"; pauseScreen(); return; }

    delete teachers[index];
    for (int j = index; j < teacherCount - 1; j++) teachers[j] = teachers[j + 1];
    teacherCount--;

    cout << "\n Teacher deleted!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::viewTeacherFeedback() {
    string id; cout << "Enter Teacher ID: "; cin >> id;
    Teacher* t = findTeacher(id);
    if (!t) cout << "Teacher not found.\n";
    else t->showFeedbacks();
    pauseScreen();
}

void AcademicSystem::menuCourses() {
    const string options[] = { "Add Course", "List All Courses", "Delete Course", "View Course Enrollments" };
    int ch;
    do {
        ch = showMenu("COURSE MANAGEMENT", options, 4);
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
        cout << " Maximum courses reached!\n";
        pauseScreen();
        return;
    }

    printHeader("ADD NEW COURSE");

    string id = getValidatedID("Enter Course ID: ");

    if (idExistsCourse(id)) {
        cout << " Course ID already exists!\n";
        pauseScreen();
        return;
    }

    cin.ignore();
    string title;
    while (true) {
        cout << "Enter Course Title: ";
        getline(cin, title);
        title = trim(title);
        if (!title.empty()) break;
        cout << "Title cannot be empty!\n";
    }

    string tid;
    while (true) {
        cout << "Enter Teacher ID: ";
        getline(cin, tid);
        tid = trim(tid);
        if (findTeacher(tid)) break;
        cout << " Teacher not found!\n";
    }

    string type;
    while (true) {
        cout << "Enter Type (Core/Elective/Lab): ";
        cin >> type;
        type = trim(type);
        if (type == "Core" || type == "Elective" || type == "Lab") break;
        cout << " Invalid type!\n";
    }

    Course* c = NULL;
    if (type == "Core") c = new CoreCourse(id, title, tid);
    else if (type == "Elective") c = new ElectiveCourse(id, title, tid);
    else c = new LabCourse(id, title, tid);

    double ew = WeightageConfig::getExamW(type);
    double aw = WeightageConfig::getAssignW(type);
    double qw = WeightageConfig::getQuizW(type);

    if (ew > 0) c->addAssessment(new Exam(id + "_EX", "Final Exam", ew));
    if (aw > 0) c->addAssessment(new Assignment(id + "_AS", "Assignment", aw));
    if (qw > 0) c->addAssessment(new Quiz(id + "_QZ", "Quiz", qw));

    courses[courseCount++] = c;
    findTeacher(tid)->addCourse(id);

    cout << "\n Course added!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::listCourses() {
    printHeader("ALL COURSES");
    if (courseCount == 0) { cout << "No courses found.\n"; pauseScreen(); return; }
    cout << left << setw(10) << "ID" << setw(30) << "Title" << setw(12) << "Type"
        << setw(12) << "Teacher" << "Students\n";
    printLine();
    for (int i = 0; i < courseCount; i++) courses[i]->display();
    pauseScreen();
}

void AcademicSystem::deleteCourse() {
    printHeader("DELETE COURSE");
    string id; cout << "Enter Course ID: "; cin >> id;

    int index = -1;
    for (int i = 0; i < courseCount; i++) {
        if (courses[i] && courses[i]->getCourseID() == id) { index = i; break; }
    }

    if (index == -1) { cout << "Course not found!\n"; pauseScreen(); return; }

    Course* c = courses[index];
    cout << "\nDelete " << c->getTitle() << "? (y/n): ";
    char confirm; cin >> confirm;
    if (tolower(confirm) != 'y') { cout << "Deletion cancelled.\n"; pauseScreen(); return; }

    for (int i = 0; i < c->getEnrollmentCount(); i++) {
        Student* s = c->getStudentAt(i);
        if (s) s->unenrollCourse(c->getCourseID());
    }

    delete courses[index];
    for (int j = index; j < courseCount - 1; j++) courses[j] = courses[j + 1];
    courseCount--;

    cout << "\n Course deleted!\n";
    saveAll();
    pauseScreen();
}

void AcademicSystem::viewCourseEnrollments() {
    string id; cout << "Enter Course ID: "; cin >> id;
    Course* c = findCourse(id);
    if (!c) { cout << "Course not found.\n"; pauseScreen(); return; }

    printHeader("ENROLLMENTS - " + c->getTitle());
    cout << "Type: " << c->getType() << " | Teacher: " << c->getTeacherID()
        << " | Enrolled: " << c->getEnrollmentCount() << "\n";
    printLine();

    for (int i = 0; i < c->getEnrollmentCount(); i++) {
        Student* s = c->getStudentAt(i);
        if (s) cout << "  " << s->getID() << "  " << s->getName() << "\n";
    }
    pauseScreen();
}

