#include "Venue.h"
#include <iostream>
#include <iomanip>

using namespace std;

Venue::Venue() : capacity(0), hasComputers(false) {}
Venue::Venue(const string& rid, int cap, bool hc) : roomID(rid), capacity(cap), hasComputers(hc) {}

void Venue::display() const {
    cout << left << setw(12) << roomID << setw(12) << capacity
        << (hasComputers ? "Yes" : "No") << endl;
}