#include "DatabaseManager.h"
#include "Student.h"
#include "Teacher.h"
#include "Course.h"
#include "Assessment.h"
#include "Venue.h"
#include "Section.h"
#include "Utils.h"
#include "WeightageConfig.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

string DatabaseManager::path(const string& fn) const { return dir + fn; }

DatabaseManager::DatabaseManager(const string& d) : dir(d) {}

void DatabaseManager::saveStudents(Student* arr[], int count) {
    ofstream f(path("students.txt").c_str());
    if (!f.is_open()) return;
    for (int i = 0; i < count; i++) {
        if (!arr[i]) continue;
        f << arr[i]->getID() << "|" << arr[i]->getName() << "|"
            << arr[i]->getEmail() << "|" << arr[i]->getType() << "|";

        string t = arr[i]->getType();
        if (t == "Scholarship" || t == "Scholarship(Probation)") {
            ScholarshipStudent* ss = dynamic_cast<ScholarshipStudent*>(arr[i]);
            f << arr[i]->getGPA() << "|" << ss->getMinGPA();
        }
        else if (t == "Regular") {
            f << arr[i]->getGPA() << "|0";
        }
        else {
            f << "0|0";
        }
        f << "\nCOURSES:";
        for (int j = 0; j < arr[i]->getCourseCount(); j++)
            f << arr[i]->getCourseAt(j) << (j < arr[i]->getCourseCount() - 1 ? "," : "");
        f << "\nGRADES:";
        for (int j = 0; j < arr[i]->getCourseCount(); j++) {
            f << arr[i]->getCourseAt(j) << "=" << arr[i]->getGradeAt(j)
                << ":" << (arr[i]->isGradeSet(j) ? "1" : "0");
            if (j < arr[i]->getCourseCount() - 1) f << ",";
        }
        f << "\n";
    }
    f.close();
}

int DatabaseManager::loadStudents(Student* arr[]) {
    ifstream f(path("students.txt").c_str());
    if (!f.is_open()) return 0;
    int count = 0;
    string line;
    while (getline(f, line) && count < MAX_STUDENTS) {
        if (trim(line).empty()) continue;
        if (line.substr(0, 8) == "COURSES:" || line.substr(0, 7) == "GRADES:") continue;
        if (line.find('|') == string::npos) continue;

        stringstream ss(line);
        string id, nm, em, tp, gpaS, minS;
        getline(ss, id, '|'); getline(ss, nm, '|'); getline(ss, em, '|');
        getline(ss, tp, '|'); getline(ss, gpaS, '|'); getline(ss, minS, '|');

        double gpa = safeDouble(gpaS);
        double mn = safeDouble(minS);

        Student* s = NULL;
        if (tp == "Regular") {
            s = new RegularStudent(id, nm, em, gpa);
        }
        else if (tp == "Scholarship" || tp == "Scholarship(Probation)") {
            s = new ScholarshipStudent(id, nm, em, gpa, mn > 0 ? mn : 3.0);
        }
        else {
            s = new ExchangeStudent(id, nm, em);
        }
        arr[count++] = s;

        string cline, gline;
        if (getline(f, cline) && cline.substr(0, 8) == "COURSES:") {
            string coursesPart = cline.substr(8);
            if (!coursesPart.empty()) {
                stringstream cs(coursesPart);
                string cid;
                while (getline(cs, cid, ','))
                    if (!trim(cid).empty()) s->enrollCourse(trim(cid));
            }
        }
        if (getline(f, gline) && gline.substr(0, 7) == "GRADES:") {
            string gp = gline.substr(7);
            if (!gp.empty()) {
                stringstream gs(gp);
                string token;
                while (getline(gs, token, ',')) {
                    size_t eq = token.find('=');
                    size_t cl = token.find(':');
                    if (eq != string::npos && cl != string::npos) {
                        string cid = token.substr(0, eq);
                        double gr = safeDouble(token.substr(eq + 1, cl - eq - 1));
                        bool set = (token.substr(cl + 1) == "1");
                        if (set) s->setFinalGrade(cid, gr);
                    }
                }
            }
        }
    }
    f.close();
    return count;
}

void DatabaseManager::saveTeachers(Teacher* arr[], int count) {
    ofstream f(path("teachers.txt").c_str());
    if (!f.is_open()) return;
    for (int i = 0; i < count; i++) {
        if (!arr[i]) continue;
        f << arr[i]->getID() << "|" << arr[i]->getName() << "|"
            << arr[i]->getEmail() << "|" << arr[i]->getAvgFeedback() << "|"
            << arr[i]->getFeedbackCount() << "\n";
        for (int j = 0; j < arr[i]->getFeedbackCount(); j++) {
            f << "FEEDBACK:" << arr[i]->getFeedbackRating(j) << "|"
                << arr[i]->getFeedbackComment(j) << "\n";
        }
    }
    f.close();
}

int DatabaseManager::loadTeachers(Teacher* arr[]) {
    ifstream f(path("teachers.txt").c_str());
    if (!f.is_open()) return 0;
    int count = 0;
    string line;
    Teacher* currentTeacher = NULL;
    while (getline(f, line) && count < MAX_TEACHERS) {
        if (trim(line).empty()) continue;
        if (line.substr(0, 9) == "FEEDBACK:") {
            if (currentTeacher) {
                size_t pos = line.find('|', 9);
                if (pos != string::npos) {
                    int rating = safeInt(line.substr(9, pos - 9));
                    string comment = line.substr(pos + 1);
                    currentTeacher->addFeedback(rating, comment);
                }
            }
        }
        else if (line.find('|') != string::npos) {
            stringstream ss(line);
            string id, nm, em, avg, cnt;
            getline(ss, id, '|'); getline(ss, nm, '|'); getline(ss, em, '|');
            getline(ss, avg, '|'); getline(ss, cnt, '|');
            currentTeacher = new Teacher(id, nm, em);
            arr[count++] = currentTeacher;
        }
    }
    f.close();
    return count;
}

void DatabaseManager::saveCourses(Course* arr[], int count) {
    ofstream f(path("courses.txt").c_str());
    if (!f.is_open()) return;
    for (int i = 0; i < count; i++) {
        if (arr[i]) {
            f << arr[i]->getCourseID() << "|" << arr[i]->getTitle() << "|"
                << arr[i]->getTeacherID() << "|" << arr[i]->getType() << "\n";
        }
    }
    f.close();
}

int DatabaseManager::loadCourses(Course* arr[]) {
    ifstream f(path("courses.txt").c_str());
    if (!f.is_open()) return 0;
    int count = 0;
    string line;
    while (getline(f, line) && count < MAX_COURSES) {
        if (trim(line).empty() || line.find('|') == string::npos) continue;
        stringstream ss(line);
        string id, title, tid, type;
        getline(ss, id, '|'); getline(ss, title, '|');
        getline(ss, tid, '|'); getline(ss, type, '|');
        type = trim(type);
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

        arr[count++] = c;
    }
    f.close();
    return count;
}

void DatabaseManager::saveVenues(Venue arr[], int count) {
    ofstream f(path("venues.txt").c_str());
    if (!f.is_open()) return;
    for (int i = 0; i < count; i++) {
        f << arr[i].getRoomID() << "|" << arr[i].getCapacity() << "|"
            << arr[i].getHasComputers() << "\n";
    }
    f.close();
}

int DatabaseManager::loadVenues(Venue arr[]) {
    ifstream f(path("venues.txt").c_str());
    if (!f.is_open()) return 0;
    int count = 0;
    string line;
    while (getline(f, line)) {
        if (trim(line).empty() || line.find('|') == string::npos) continue;
        stringstream ss(line);
        string id, cap, hc;
        getline(ss, id, '|'); getline(ss, cap, '|'); getline(ss, hc, '|');
        arr[count++] = Venue(trim(id), safeInt(cap), safeInt(trim(hc)) == 1);
    }
    f.close();
    return count;
}

void DatabaseManager::saveSections(Section arr[], int count) {
    ofstream f(path("sections.txt").c_str());
    if (!f.is_open()) return;
    for (int i = 0; i < count; i++) {
        f << arr[i].getSectionID() << "|" << arr[i].getCourseID() << "|"
            << arr[i].getTeacherID() << "|" << arr[i].getVenueID() << "|"
            << arr[i].getTimeSlot() << "|" << arr[i].getStudentCount() << "\n";
    }
    f.close();
}

int DatabaseManager::loadSections(Section arr[]) {
    ifstream f(path("sections.txt").c_str());
    if (!f.is_open()) return 0;
    int count = 0;
    string line;
    while (getline(f, line) && count < MAX_SECTIONS) {
        if (trim(line).empty() || line.find('|') == string::npos) continue;
        stringstream ss(line);
        string sid, cid, tid, vid, ts, sc;
        getline(ss, sid, '|'); getline(ss, cid, '|'); getline(ss, tid, '|');
        getline(ss, vid, '|'); getline(ss, ts, '|'); getline(ss, sc, '|');
        arr[count++] = Section(trim(sid), trim(cid), trim(tid), trim(vid), trim(ts));
    }
    f.close();
    return count;
}

void DatabaseManager::saveAssessmentScores(Course* courses[], int courseCount) {
    ofstream f(path("assessments.txt").c_str());
    if (!f.is_open()) return;
    for (int ci = 0; ci < courseCount; ci++) {
        if (!courses[ci]) continue;
        for (int ai = 0; ai < courses[ci]->getAssessmentCount(); ai++) {
            Assessment* a = courses[ci]->getAssessmentAt(ai);
            for (int si = 0; si < a->getScoreCount(); si++) {
                string sid = a->getScoreStudentID(si);
                f << courses[ci]->getCourseID() << "|" << a->getAssessID() << "|"
                    << a->getType() << "|" << sid << "|" << a->getRaw(sid) << "|"
                    << a->getMax(sid) << "\n";
            }
        }
    }
    f.close();
}

void DatabaseManager::loadAssessmentScores(Course* courses[], int courseCount) {
    ifstream f(path("assessments.txt").c_str());
    if (!f.is_open()) return;
    string line;
    while (getline(f, line)) {
        if (trim(line).empty() || line.find('|') == string::npos) continue;
        stringstream ss(line);
        string cid, aid, atype, sid, raw, mx;
        getline(ss, cid, '|'); getline(ss, aid, '|'); getline(ss, atype, '|');
        getline(ss, sid, '|'); getline(ss, raw, '|'); getline(ss, mx, '|');
        for (int ci = 0; ci < courseCount; ci++) {
            if (!courses[ci] || courses[ci]->getCourseID() != cid) continue;
            for (int ai = 0; ai < courses[ci]->getAssessmentCount(); ai++) {
                Assessment* a = courses[ci]->getAssessmentAt(ai);
                if (a->getAssessID() == aid) {
                    a->setScore(sid, safeDouble(raw), safeDouble(mx));
                    break;
                }
            }
        }
    }
    f.close();
}

void DatabaseManager::saveSchedule(const string& scheduleText) {
    ofstream f(path("exam_schedule.txt").c_str());
    if (f.is_open()) {
        f << scheduleText;
        f.close();
    }
}