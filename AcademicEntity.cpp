#include "AcademicEntity.h"

AcademicEntity::AcademicEntity() {}

AcademicEntity::AcademicEntity(const std::string& id, const std::string& n, const std::string& e)
    : ID(id), name(n), email(e) {
}

AcademicEntity::~AcademicEntity() {}