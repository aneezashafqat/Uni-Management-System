#ifndef UTILS_H
#define UTILS_H

#include <string>

void clearScreen();
void pauseScreen();
void printLine(char c = '-', int len = 60);
void printHeader(const std::string& title);

std::string trim(const std::string& s);

void clearInput();
int safeIntInput(const std::string& prompt, int minVal, int maxVal);
double safeDoubleInput(const std::string& prompt, double minVal, double maxVal);

bool isValidID(const std::string& id);
bool isValidName(const std::string& name);
bool isValidEmail(const std::string& email);
bool isValidTimeSlot(const std::string& slot);
bool confirmAction(const std::string& message);

double percentToGPA(double pct);
std::string letterGrade(double pct);

int safeInt(const std::string& s);
double safeDouble(const std::string& s);

std::string getValidatedID(const std::string& prompt);
std::string getValidatedName(const std::string& prompt);
std::string getValidatedEmail(const std::string& prompt);
std::string getValidatedType(const std::string& prompt);

int showMenu(const std::string& title, const std::string options[], int count);

#endif
