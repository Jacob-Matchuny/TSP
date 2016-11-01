// Extern includes
#include <iostream>

// Includes from project
#include "city.h"
#include "link.h"
#include "dataset.h"

// Global dataset
DataSet ds;

using namespace std;

// Command line functions
void help();
void parseArgs(int, char**);

// Main function
int main(int argc, char** argv)
{
    // Disable gtk logging
    FILE* file = freopen("/dev/null", "w", stderr);
    fclose(file);

    // Parse command line args
    parseArgs(argc, argv);

    // Display resulting cheapest tour
    ds.printResults();

    // Print graph
    ds.printGraph();

    return 0;
}

// Help function for command line
void help()
{
    std::cout << std::endl;
    std::cout << "----------------------- HELP -----------------------" << std::endl;
    std::cout << " ./tsp-solver <filename> <algorithm> <args> " << std::endl << std::endl;
    std::cout << "<filename>  : must be concorde format .tsp file" << std::endl << std::endl;
    std::cout << "<algorithm> : must be [ brute, greedy, genetic ]" << std::endl << std::endl;
    std::cout << "<args>      : brute   : NONE" << std::endl;
    std::cout << "            : greedy  : NONE" << std::endl;
    std::cout << "            : genetic : <crossover> <mutator> " << std::endl;
    std::cout << "-----------------------------------------------------" << std::endl;
}

// Parse command line args
void parseArgs(int argc, char** argv)
{
    // Parse args
    int rc = 1;
    if(argc > 2)
    {    
        // Create new dataset
        ds = DataSet(argv[1]);
        
        // Set algorithm
        ds.algorithm = argv[2];

        // Read in cities from file
        ds.readInData();

        // Determine Algorithm
        if(ds.algorithm.compare("brute") == 0)
        {
            ds.brute();
            rc = 0;
        }
        else if(ds.algorithm.compare("greedy") == 0)
        {
            ds.greedy();
            rc = 0;
        }
        else if(ds.algorithm.compare("genetic") == 0)
        {
            if(argc > 4)
            {
                ds.cross = atoi(argv[3]);
                ds.mutate = atoi(argv[4]);
                ds.genetic();
                rc = 0;
            }
        }
    }
    
    if(rc)
    {
        help();
        exit(0);
    }
}
