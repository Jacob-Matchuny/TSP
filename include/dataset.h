// Jacob Matchuny
// TSP solver
// Dataset header

// Multiple inclusion protection
#ifndef DATASET_H
#define DATASET_H

// Define PI if its not defined (windows trick)
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Includes from package
#include "city.h"
#include "link.h"

// Extern includes
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <time.h>

// Graphics
#include <cairo.h>
#include <gtk/gtk.h>

// Tour - holds all information for one particular pass across graph
struct Tour
{
    // Vector of links
    std::vector<Link> tour;
    
    // Cost of tour (summed distances)
    float cost;

    // Execution time
    double time;

    // Operator for next_permutation
    bool operator<(const Tour& val) const
    {
    	return cost < val.cost;
    }
};

// Dataset - holds and analyzes data
class DataSet
{
    public:
        // Constructor
        DataSet(std::string);

        // Default constructor
        DataSet();

        // Destructor
        ~DataSet();

        // Read in cities / generate links
        void readInData();

        // Brute force tours
        void bruteForce();

        // Closest edge tours
        void greedy();

        // Print results (best tour)
        void printResults();

        // Print graph
        void printGraph();

        // Get cities
        std::vector<City> getCities() { return cities; }

        // Get cheapestTour
        Tour getCheapestTour() { return cheapestTour; }

        // Get closest city to city
        void findClosestCity(City c1);

        // Check if all cities are added to graph
        bool allCitiesAdded();

        // Do the drawing with cairo
        void do_drawing(cairo_t*);
 
    private:
        // Filename to open
        std::string filename;

        // List of cities from file
        std::vector<City> cities;

        // Cheapest tour currently calculated
        Tour cheapestTour;

        // Temp tour
        Tour tempTour;

        // Number of tours calculated
        long int tourCount;
};

#endif // DATASET_H
