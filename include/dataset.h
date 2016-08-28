// Jacob Matchuny
// TSP solver
// Dataset header

// Multiple inclusion protection
#ifndef DATASET_H
#define DATASET_H

// Includes from package
#include "city.h"
#include "link.h"

// Extern includes
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

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

        // Generate tours from dataset
        void generateTours();

        // Sort tours by cost
        void sortTours();

        // Print results (best tour)
        void printResults();

        // Print graph
        void printGraph(int, char**);

        // Get cities
        std::vector<City> getCities() { return cities; }

        // Get cheapest tour
        Tour getCheapestTour() { return cheapestTour; }

        // Cleanup
        void cleanup();

        // Do the drawing with cairo
        void do_drawing(cairo_t*);
 
    private:
        // Filename to open
        std::string filename;

        // List of cities from file
        std::vector<City> cities;

        // Cheapest tour currently calculated
        Tour cheapestTour;

        // Number of tours calculated
        long int tourCount;
};

#endif // DATASET_H
