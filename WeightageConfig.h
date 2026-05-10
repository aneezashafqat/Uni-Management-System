#ifndef WEIGHTAGE_CONFIG_H
#define WEIGHTAGE_CONFIG_H

#include <string>

class WeightageConfig {
private:
    static double examW[3];
    static double assignW[3];
    static double quizW[3];

    static int indexOf(const std::string& t);

public:
    static void loadFromFile(const std::string& filename);
    static double getExamW(const std::string& t);
    static double getAssignW(const std::string& t);
    static double getQuizW(const std::string& t);
    static void display();
};

#endif