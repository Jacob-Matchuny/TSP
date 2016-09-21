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

        // Setters getters for x
        float getX() { return x; }
        void setX(float val) { x = val; }

        // Setters getters for y
        float getY() { return y; }
        void setY(float val) { y = val; }

        // Setters getters for city number
        unsigned int getNum() { return num; }
        void setNum(unsigned int val) { num = val; }

        // Setters getters for added
        bool getAdded() { return added; }
        void setAdded(bool val) { added = val; }

        // Operator for next_permutation
        bool operator<(const City& val) const
        {
            return num < val.num;
        }

    private:
        // X coord of city
        float x;

        // Y coord of city
        float y;

        // Representation of city
        unsigned int num;

        // If city added to graph
        bool added;
};

#endif // CITY_H
