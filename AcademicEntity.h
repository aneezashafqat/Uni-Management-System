#ifndef ACADEMIC_ENTITY_H
#define ACADEMIC_ENTITY_H

#include <string>

class AcademicEntity {
protected:
    std::string ID;
    std::string name;
    std::string email;

public:
    AcademicEntity();
    AcademicEntity(const std::string& id, const std::string& n, const std::string& e);
    virtual ~AcademicEntity();

    std::string getID() const { return ID; }
    std::string getName() const { return name; }
    std::string getEmail() const { return email; }
    void setName(const std::string& n) { name = n; }
    void setEmail(const std::string& e) { email = e; }

    virtual void displayProfile() const = 0;
};

#endif