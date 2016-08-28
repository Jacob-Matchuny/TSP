// Jacob Matchuny
// TSP solver
// Link source

// Includes from this project
#include "link.h"

// Constructor
Link::Link(const City &a, const City &b)
{
    this->a = a;
    this->b = b;

    // Euclidian distance
    float x = pow(this->b.getX() - this->a.getX(), 2);
    float y = pow(this->b.getY() - this->a.getY(), 2);
    this->distance = sqrt(x + y);
}

// Deconstructor
Link::~Link()
{
}
