#include "AcademicSystem.h" //yet to write code for this class
#include <cstdlib>  // for dictionaries 

int main() {
#ifdef _WIN32
    system("mkdir data 2>nul");  //windows
#else
    system("mkdir -p data");  //linux n mac
#endif

    AcademicSystem sys;
    sys.run(); // for display n user input menu() 
    return 0;
}
