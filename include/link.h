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
#include <iostream>

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

        // First city
        City a;

        // Second city
        City b;

        // Euclidian distance from a -> b
        float distance;

        // Dist
        void dist(const City&, const City&);

        // Operator for find
        bool operator< (const Link& l) 
        {
            return (l.distance < distance);
        }
};

#endif // LINK_H
