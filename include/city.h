// Jacob Matchuny
// TSP solver
// City header

// Multiple inclusion protection
#ifndef CITY_H
#define CITY_H

// City - holds information for a city (x, y, representation)
class City
{
    public:
        // Constructor
        City(float, float, unsigned int);

        // Default constructor
        City();

        // Destructor
        ~City();

        // X coord of city
        float x;

        // Y coord of city
        float y;

        // Representation of city
        unsigned int num;

        // If city added to graph
        bool added;
        
        // Operator for next_permutation
        bool operator<(const City& val) const
        {
            return num < val.num;
        }

        // Operator for find
        bool operator==(const City& val) const
        {
            return num == val.num;
        }

        void operator=(const City& val) 
        {
            x = val.x;
            y = val.y;
            num = val.num;
        }
};

#endif // CITY_H
