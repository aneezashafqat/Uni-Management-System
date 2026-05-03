#include "AcademicSystem.h"
#include <cstdlib>

int main() {
    // Create data directory if it doesn't exist
#ifdef _WIN32
    system("mkdir data 2>nul");
#else
    system("mkdir -p data");
#endif

    AcademicSystem sys;
    sys.run();
    return 0;
}