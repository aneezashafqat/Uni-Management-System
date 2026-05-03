#include "Utils.h"
#include <iostream>
#include <cstdlib>
#include <cctype>
#include <iomanip>

using namespace std;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen() {
    cout << "\nPress Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}

void printLine(char c, int len) {
    for (int i = 0; i < len; i++) cout << c;
    cout << endl;
}

void printHeader(const string& title) {
    cout << endl;
    printLine('=');
    int spaces = (60 - (int)title.size()) / 2;
    for (int i = 0; i < spaces; i++) cout << ' ';
    cout << title << endl;
    printLine('=');
}

string trim(const string& s) {
    int start = 0, end = (int)s.size() - 1;
    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r')) start++;
    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) end--;
    return (start <= end) ? s.substr(start, end - start + 1) : "";
}

void clearInput() {
    cin.clear();
    cin.ignore(10000, '\n');
}

int safeIntInput(const string& prompt, int minVal, int maxVal) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value < minVal) {
                cout << "ERROR: Value cannot be less than " << minVal << "!\n";
                continue;
            }
            if (value > maxVal) {
                cout << "ERROR: Value cannot be greater than " << maxVal << "!\n";
                continue;
            }
            clearInput();
            return value;
        }
        else {
            cout << "ERROR: Invalid input! Please enter a number.\n";
            clearInput();
        }
    }
}

double safeDoubleInput(const string& prompt, double minVal, double maxVal) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value < minVal) {
                cout << "ERROR: Value cannot be less than " << minVal << "!\n";
                continue;
            }
            if (value > maxVal) {
                cout << "ERROR: Value cannot be greater than " << maxVal << "!\n";
                continue;
            }
            clearInput();
            return value;
        }
        else {
            cout << "ERROR: Invalid input!\n";
            clearInput();
        }
    }
}

bool isValidID(const string& id) {
    if (id.empty()) {
        cout << "ERROR: ID cannot be empty!\n";
        return false;
    }
    for (size_t i = 0; i < id.length(); i++) {
        char c = id[i];
        if (!isdigit(c) && c != '_' && c != '-') {
            cout << "ERROR: ID can only contain NUMBERS, _, -\n";
            return false;
        }
    }
    return true;
}

bool isValidName(const string& name) {
    if (name.empty()) {
        cout << "ERROR: Name cannot be empty!\n";
        return false;
    }
    for (size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        if (!isalpha(c) && c != ' ' && c != '.' && c != '-' && c != '\'') {
            cout << "ERROR: Name can only contain LETTERS, spaces, dots, hyphens\n";
            return false;
        }
    }
    return true;
}

bool isValidEmail(const string& email) {
    if (email.empty()) {
        cout << "ERROR: Email cannot be empty!\n";
        return false;
    }
    bool hasAt = false, hasDot = false;
    int atPos = -1;
    for (size_t i = 0; i < email.length(); i++) {
        if (email[i] == '@') {
            if (hasAt) {
                cout << "ERROR: Multiple @ symbols\n";
                return false;
            }
            hasAt = true;
            atPos = i;
        }
        if (hasAt && email[i] == '.' && i > (size_t)atPos) hasDot = true;
    }
    if (!hasAt) {
        cout << "ERROR: Email must contain @\n";
        return false;
    }
    if (!hasDot) {
        cout << "ERROR: Email must have domain extension\n";
        return false;
    }
    if (atPos == 0 || atPos == (int)email.length() - 1) {
        cout << "ERROR: @ cannot be first or last\n";
        return false;
    }
    return true;
}

bool isValidTimeSlot(const string& slot) {
    if (slot.empty() || slot == "TBD") return true;
    if (slot.length() < 10) {
        cout << "ERROR: Invalid time slot format\n";
        return false;
    }
    return true;
}

bool confirmAction(const string& message) {
    cout << "\n" << message << "\nType 'yes' to confirm: ";
    string response;
    cin >> response;
    clearInput();
    return (response == "yes" || response == "YES");
}

double percentToGPA(double pct) {
    if (pct >= 90) return 4.0;
    if (pct >= 85) return 3.7;
    if (pct >= 80) return 3.3;
    if (pct >= 75) return 3.0;
    if (pct >= 70) return 2.7;
    if (pct >= 65) return 2.3;
    if (pct >= 60) return 2.0;
    if (pct >= 55) return 1.7;
    if (pct >= 50) return 1.3;
    return 0.0;
}

string letterGrade(double pct) {
    if (pct >= 90) return "A+";
    if (pct >= 85) return "A";
    if (pct >= 80) return "A-";
    if (pct >= 75) return "B+";
    if (pct >= 70) return "B";
    if (pct >= 65) return "B-";
    if (pct >= 60) return "C+";
    if (pct >= 55) return "C";
    if (pct >= 50) return "D";
    return "F";
}

int safeInt(const string& s) { return atoi(s.c_str()); }
double safeDouble(const string& s) { return atof(s.c_str()); }