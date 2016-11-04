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
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <ctime>

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
        // ------ GENERAL ------
        // Constructor
        DataSet(std::string);

        // Default constructor
        DataSet();

        // Destructor
        ~DataSet();

        // Algorithm used
        std::string algorithm;
        
        // Filename to open
        std::string filename;
        
        // List of cities from file
        std::vector<City> cities;

        // Read in cities / generate links
        void readInData();
        
        // Print results (best tour)
        void printResults();

        // Print graph
        void printGraph();
        
        // Do the drawing with cairo
        void do_drawing(cairo_t*);
        
        // Cheapest tour currently calculated
        Tour cheapestTour;

        // Temp tour
        Tour tempTour;

        // Number of tours calculated
        long int tourCount;
        // ---------------------


        // ------ BRUTE ------
        void brute();
        // -------------------


        // ------ GREEDY ------
        void greedy();

        // Get closest city to city
        void findClosestCity(City c1);

        // Check if all cities are added to graph
        bool allCitiesAdded();
        // --------------------


        // ------ GENETIC ------
        void genetic();
        
        // Population for GA
        std::vector<Tour> population;
        
        // Population size for GA
        unsigned int popSize;

        // Generation count
        unsigned int genCount;

        // Mutation factor
        double mutateFactor;

        // Initialize population for GA
        void initPop();

        // Print population for GA
        void printPop();

        // Sort population
        void sortPop();

        // Mutate population
        void mutatePop();
        
        // Crossover population
        void crossPop();

        // Crossover population helper
        Tour crossover(const Tour&, const Tour&);

        // Crossover function to pick
        int cross;

        // Mutation function to pick
        int mutate;

        // Mutation count
        int mutateCount;
        // ---------------------
        

        // ----- WISDOM OF CROWDS -----
        void wisdom();
        // ----------------------------
};

#endif // DATASET_H
