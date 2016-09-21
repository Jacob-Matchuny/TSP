// Jacob Matchuny
// TSP solver
// City source

// Includes from this project
#include "city.h"

// Constructor
City::City(float x, float y, unsigned int num)
{
    this->x = x;
    this->y = y;
    this->num = num;
    this->added = false;
}

// Default constructor
City::City()
{
    this->added = false;
}

// Deconstructor
City::~City()
{
}
