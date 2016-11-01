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
    float x = pow(this->b.x - this->a.x, 2);
    float y = pow(this->b.y - this->a.y, 2);
    this->distance = sqrt(x + y);
}

// Default constructor
Link::Link()
{
    this->distance = -1;
}

// Deconstructor
Link::~Link()
{
}

void Link::dist(const City& a, const City& b)
{
    // Euclidian distance
    float x = pow(this->b.x - this->a.x, 2);
    float y = pow(this->b.y - this->a.y, 2);
    this->distance = sqrt(x + y);
}
