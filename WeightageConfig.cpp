#include "WeightageConfig.h"
#include "Utils.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

using namespace std;

double WeightageConfig::examW[3] = { 50, 30, 0 };
double WeightageConfig::assignW[3] = { 30, 40, 40 };
double WeightageConfig::quizW[3] = { 20, 30, 60 };

int WeightageConfig::indexOf(const string& t) {
    if (t == "Core") return 0;
    if (t == "Elective") return 1;
    if (t == "Lab") return 2;
    return 0;
}

void WeightageConfig::loadFromFile(const string& filename) {
    ifstream f(filename.c_str());
    if (!f.is_open()) {
        // Default values already set
        return;
    }
    string line;
    while (getline(f, line)) {
        if (trim(line).empty()) continue;
        stringstream ss(line);
        string type;
        ss >> type >> examW[indexOf(type)] >> assignW[indexOf(type)] >> quizW[indexOf(type)];
    }
    f.close();
}

double WeightageConfig::getExamW(const string& t) { return examW[indexOf(t)]; }
double WeightageConfig::getAssignW(const string& t) { return assignW[indexOf(t)]; }
double WeightageConfig::getQuizW(const string& t) { return quizW[indexOf(t)]; }

void WeightageConfig::display() {
    printHeader("WEIGHTAGE CONFIGURATION");
    cout << left << setw(12) << "Type" << setw(10) << "Exam%"
        << setw(14) << "Assignment%" << "Quiz%" << endl;
    printLine();
    string types[] = { "Core", "Elective", "Lab" };
    for (int i = 0; i < 3; i++) {
        cout << left << setw(12) << types[i] << setw(10) << examW[i]
            << setw(14) << assignW[i] << quizW[i] << endl;
    }
}