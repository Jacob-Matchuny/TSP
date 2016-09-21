// Extern includes
#include <iostream>

// Includes from project
#include "city.h"
#include "link.h"
#include "dataset.h"

DataSet ds;

using namespace std;

const std::string testfiles[] =
{
    "./testfiles/Random4.tsp",
    "./testfiles/Random5.tsp",
    "./testfiles/Random6.tsp",
    "./testfiles/Random7.tsp",
    "./testfiles/Random8.tsp",
    "./testfiles/Random9.tsp",
    "./testfiles/Random10.tsp",
    "./testfiles/Random11.tsp",
    "./testfiles/Random12.tsp",
};

int main(int argc, char** argv)
{
    // Create new dataset
    if(argc < 2)
        ds = DataSet(testfiles[0]);
    else
        ds = DataSet(argv[1]);

    // Read in cities from file
    ds.readInData();

    // Calculate all possible tours
    ds.greedy();

    // Display resulting cheapest tour
    ds.printResults();

    // Print graph
    ds.printGraph();

    return 0;
}
