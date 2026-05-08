#include "utils.h"
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
    for (int i = 0; i < len; i++) {
        cout << c;
    }
    cout << endl;
}

void printHeader(const string& title) {
    cout << endl;
    printLine('=');
    int spaces = (60 - (int)title.size()) / 2;
    for (int i = 0; i < spaces; i++) {
        cout << ' ';
    }
    cout << title << endl;
    printLine('=');
}


int showMenu(const string& title, const string options[], int count) {
    printHeader(title);
    for (int i = 0; i < count; i++) {
        cout << "  " << (i + 1) << ". " << options[i] << "\n";
    }
    cout << "  0. Back\n";
    printLine();
    return safeIntInput("Choice: ", 0, count);
}



string trim(const string& s) {
    int start = 0;
    int end = (int)s.size() - 1;

    while (start <= end && (s[start] == ' ' || s[start] == '\t' || s[start] == '\r')) {
        start++;
    }

    while (end >= start && (s[end] == ' ' || s[end] == '\t' || s[end] == '\r')) {
        end--;
    }

    if (start <= end) {
        return s.substr(start, end - start + 1);
    }
    return "";
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
            cout << "ERROR: Invalid input! Please enter a number.\n";
            clearInput();
        }
    }
}


bool isValidID(const string& id) {
    if (id.empty()) return false;
    if (id[0] == '-') return false;
    for (size_t i = 0; i < id.length(); i++) {
        if (!isdigit(id[i])) return false;
    }
    return true;
}

bool isValidName(const string& name) {
    if (name.empty()) return false;
    for (size_t i = 0; i < name.length(); i++) {
        char c = name[i];
        if (!isalpha(c) && c != ' ' && c != '.' && c != '-' && c != '\'') {
            return false;
        }
    }
    return true;
}

bool isValidEmail(const string& email) {
    if (email.empty()) return false;

    bool hasAt = false;
    bool hasDot = false;
    int atPos = -1;

    for (size_t i = 0; i < email.length(); i++) {
        if (email[i] == '@') {
            if (hasAt) return false;
            hasAt = true;
            atPos = i;
        }
        if (hasAt && email[i] == '.' && i > (size_t)atPos) {
            hasDot = true;
        }
    }

    if (!hasAt) return false;
    if (!hasDot) return false;
    if (atPos == 0) return false;
    if (atPos == (int)email.length() - 1) return false;

    return true;
}

bool isValidTimeSlot(const string& slot) {
    if (slot.empty() || slot == "TBD") return true;
    if (slot.length() < 10) return false;
    return true;
}

bool confirmAction(const string& message) {
    cout << "\n" << message << "\n";
    cout << "Type 'yes' to confirm: ";
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


int safeInt(const string& s) {
    return atoi(s.c_str());
}

double safeDouble(const string& s) {
    return atof(s.c_str());
}

// ==================== HELPER INPUT FUNCTIONS ====================

string getValidatedID(const string& prompt) {
    string id;
    while (true) {
        cout << prompt;
        cin >> id;
        id = trim(id);

        if (!isValidID(id)) {
            cout << "ERROR: ID must contain ONLY numbers (0-9) and cannot be negative!\n";
            continue;
        }
        break;
    }
    return id;
}

string getValidatedName(const string& prompt) {
    string name;
    cin.ignore();
    while (true) {
        cout << prompt;
        getline(cin, name);
        name = trim(name);

        if (!isValidName(name)) {
            cout << "ERROR: Name can only contain letters, spaces, dots, hyphens, and apostrophes!\n";
            continue;
        }
        break;
    }
    return name;
}

string getValidatedEmail(const string& prompt) {
    string email;
    while (true) {
        cout << prompt;
        getline(cin, email);
        email = trim(email);

        if (!isValidEmail(email)) {
            cout << "ERROR: Invalid email format! Must contain @ and domain extension (e.g., .com)\n";
            continue;
        }
        break;
    }
    return email;
}

string getValidatedType(const string& prompt) {
    string type;
    while (true) {
        cout << prompt;
        cin >> type;
        type = trim(type);

        if (type == "Regular" || type == "Scholarship" || type == "Exchange") {
            return type;
        }
        cout << "ERROR: Invalid type! Please enter Regular, Scholarship, or Exchange\n";
    }
} 
