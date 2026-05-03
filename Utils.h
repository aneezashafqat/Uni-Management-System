#ifndef UTILS_H
#define UTILS_H

#include <string>

void clearScreen(); //clears screen 
void pauseScreen(); //user presses enter to move on 
void printLine(char c = '-', int len = 60); //for formatting prints line of repeated chars
void printHeader(const std::string& title); //centers the title
std::string trim(const std::string& s); //string manipulation removes spaces from start and end 
void clearInput(); //clears for errors 
int safeIntInput(const std::string& prompt, int minVal, int maxVal); //reads range for intengers (validation)
double safeDoubleInput(const std::string& prompt, double minVal, double maxVal); //reads range for decimals (validation)
bool isValidID(const std::string& id); // checks if ID only has nums
bool isValidName(const std::string& name); //validates name
bool isValidEmail(const std::string& email); //formats emails
bool isValidTimeSlot(const std::string& slot); //formats time 
bool confirmAction(const std::string& message); //yes before deletion or others
//grade conversions
double percentToGPA(double pct);
std::string letterGrade(double pct);
int safeInt(const std::string& s);
double safeDouble(const std::string& s);

#endif
