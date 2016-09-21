// Jacob Matchuny
// TSP solver
// Link header

// Multiple inclusion protection
#ifndef LINK_H
#define LINK_H

// Includes from this project
#include "city.h"

// Extern includes
#include <cmath>

// Link - Holds information about link between cities including euclidian distance
class Link
{
    public:
        // Constructor
        Link(const City &a, const City &b);

        // Default constructor
        Link();

        // Destructor
        ~Link();

        // Getters for city a
        City getA() { return a; }
        void setA(City val) { a = val; }

        // Getters for city b
        City getB() { return b; }

        // Setters getters for distance
        float getDistance() { return distance; }
        void getDistance(float val) { distance = val; }
 
    private:
        // First city
        City a;

        // Second city
        City b;

        // Euclidian distance from a -> b
        float distance;
};

#endif // LINK_H
