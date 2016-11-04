// Jacob Matchuny
// TSP solver
// Dataset source

// Extern includes
#include <iostream>

// Includes from this project
#include "dataset.h"

// Function prototypes
static std::string toStrMaxDecimals(double, int);
static gboolean on_draw_event(GtkWidget*, cairo_t*, gpointer);
static void do_drawing(cairo_t*);

// Global dataset, instantiated in main
extern DataSet ds;

// Constructor
DataSet::DataSet(std::string filename)
{
    this->filename = filename;
    this->tourCount = 0;
    this->cheapestTour.cost = 0;
    this->tempTour.cost = 0;
    this->popSize = 150;
    this->mutateFactor = 0.15;
}

// Default constructor
DataSet::DataSet()
{
    this->tourCount = 0;
}

// Deconstructor
DataSet::~DataSet()
{
}

// Read in data
void DataSet::readInData()
{
    int num;
    float x, y;

    std::ifstream file;
    std::string line;

    file.open(filename);

    // If file valid, parse
    if(file.good())
    {
        std::cout << "Reading from: " << filename << std::endl << std::endl;

        while(!file.eof())
        {
            std::getline(file, line);
            std::istringstream iss(line);
            if(!(iss >> num >> x >> y))
            {
                // Header stuff we dont want
                continue;
            }

            // Add city
            cities.push_back(City(x, y, num));
        }
    }
    else
    {
        std::cout << "Bad file: " << filename << std::endl;
    }

    // Close file
    file.close();
}

// Brute force to generate tours
void DataSet::brute()
{
    cheapestTour.time = clock();
    // Calculate all tours
    do
    {
        // Create tour (for now)
        Tour tour;

        // Create links
        for(unsigned int i = 0; i < cities.size() - 1; i++)
        {
            // Build link from  current city to next
            tour.tour.push_back(Link(cities.at(i), cities.at(i + 1)));

            // Adjust cost
            tour.cost += tour.tour.at(i).distance;
        }

        // Generate link to first city
        tour.tour.push_back(Link(cities.back(), cities.front()));
        tour.cost += tour.tour.back().distance;

        // Adjust cheapest cost if need be
        if(tour.cost < cheapestTour.cost || cheapestTour.cost == 0)
            cheapestTour = tour;

        // Adjust tourCount
        tourCount++;
        tour.cost = 0;
    } while(std::next_permutation(cities.begin(), cities.end()));

    // Subtract current time from cheapestTour time
    cheapestTour.time -= clock();

    // Divide time by CPS to get time in secs
    cheapestTour.time /= (double) CLOCKS_PER_SEC;

    // Time is currently negative so flip sign and convert to ms
    cheapestTour.time *= -1000;
}

// Print results
void DataSet::printResults()
{
    // Log final results
    std::cout << std::endl;
    std::cout << "-------- FINAL RESULTS ------" << std::endl;
    std::cout << "Cities: " << cities.size() << std::endl;
    std::cout << "Tours Calculated: " << tourCount << std::endl << std::endl;

    std::cout << "----- Final Path -----" << std::endl << "[ " << cheapestTour.tour.front().a.num << " ";
    for(auto & link : cheapestTour.tour)
        std::cout << link.b.num << " ";
    std::cout << "]" << std::endl;

    std::cout << "----------------------" << std::endl << std::endl;
    std::cout << "Cheapest Tour: " << cheapestTour.cost << " ";
    std::cout << "Execution Time: " << cheapestTour.time << std::endl;

    if(!algorithm.compare("genetic"))
    {
        std::cout << "Mutations: " << mutateCount << std::endl;
        std::cout << "Generations: " << genCount << std::endl;
    }
}

// Print graphics
void DataSet::printGraph()
{
    // Graphics vars
    GtkWidget *window;
    GtkWidget *darea;

    // Initialize gtk window
    gtk_init(0, NULL);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    // Initialize draw area on gtk window
    darea = gtk_drawing_area_new();
    gtk_container_add(GTK_CONTAINER(window), darea);

    // Connect callbacks to gtk container
    g_signal_connect(G_OBJECT(darea), "draw", G_CALLBACK(on_draw_event), NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    // Setup gtk window params
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
    gtk_window_maximize(GTK_WINDOW(window));
    gtk_window_set_title(GTK_WINDOW(window), "Jacob Matchuny: TSP");
    gtk_widget_show_all(window);

    // Set icon for application
    gtk_window_set_icon_from_file(GTK_WINDOW(window), "icon.png", NULL);
    
    // Start gtk main
    gtk_main();
}

// Do drawing event for GTK
static gboolean on_draw_event(GtkWidget *widget, cairo_t *cr, gpointer user_data)
{
    do_drawing(cr);
    return FALSE;
}

// Draw stuff on GTK window using cairo
static void do_drawing(cairo_t* cr)
{
    int scale = 4;
    if(ds.cities.size() > 10)
        scale = 5;

    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);

    // Print all links
    for(auto & link : ds.cheapestTour.tour)
    {
        cairo_move_to(cr, link.a.x * scale, link.a.y * scale);
        cairo_line_to(cr, link.b.x * scale, link.b.y * scale);
        cairo_stroke(cr);
    }

    // Print all cities 
    for(auto & city : ds.cities)
    {
        // Create gradient for cities
        cairo_pattern_t* r1;
        r1 = cairo_pattern_create_radial(city.x * scale, city.y * scale, 3, city.x * scale, city.y * scale, 11);  
        cairo_pattern_add_color_stop_rgba(r1, 0, 1, 1, 1, 1);
        cairo_pattern_add_color_stop_rgba(r1, 1, 0.6, 0.6, 0.6, 1);

        // Paint city
        cairo_set_source(cr, r1);
        cairo_arc(cr, city.x * scale, city.y * scale, 11, 0, 2*M_PI);
        cairo_fill(cr); 

        // Print city num
        cairo_set_font_size (cr, 18.0);
        cairo_set_source_rgb(cr, 1, 1, 1);

        if(city.num < 10)
            cairo_move_to(cr, city.x * scale - 5.5, city.y * scale + 5.5);
        else
            cairo_move_to(cr, city.x * scale - 9.0, city.y * scale + 5.5);

        std::string name = "";
        name.append(toStrMaxDecimals(city.num, 0));
        cairo_text_path(cr, name.c_str());
        cairo_fill_preserve(cr);
    
        // Border around city number
        cairo_set_source_rgb(cr, 0, 0, 0);
        cairo_set_line_width(cr, 0.70);
        cairo_stroke(cr);
    }
    
    // Setup text format
    cairo_set_font_size (cr, 35.0);

    // Setup spacer and algorithm strings
    std::string spacer = "                   ";
    std::transform(ds.algorithm.begin(), ds.algorithm.end(), ds.algorithm.begin(), toupper);
    
    // Setup cheapest tour string
    std::string cheapestTourString = "Algorithm: " + ds.algorithm;
    cheapestTourString.append(spacer + "File: " + ds.filename);
    cheapestTourString.append(spacer + "Cost: " + toStrMaxDecimals(ds.cheapestTour.cost, 2));
    cheapestTourString.append(spacer + "Runtime: " + toStrMaxDecimals(ds.cheapestTour.time, 2) + " ms");

    // Print string and format it
    cairo_move_to (cr, 15, gdk_screen_height() - 90);
    cairo_text_path(cr, cheapestTourString.c_str());
    cairo_set_source_rgb(cr, 1, 1, 1);
    cairo_fill_preserve(cr);

    // Print border on string
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_set_line_width(cr, 1.56);
    cairo_stroke(cr);
}

// Format string to dec places provided
// Grabbed from: http://stackoverflow.com/questions/900326/
static std::string toStrMaxDecimals(double value, int decimals)
{
    // Setup string stream
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(decimals) << value;
    std::string s = ss.str();

    if(decimals > 0 && s[s.find_last_not_of('0')] == '.')
        s.erase(s.size() - decimals + 1);

    return s;
}

// Nearest neighbor algorithm
void DataSet::greedy()
{
    // Start clock
    cheapestTour.time = clock();

    // Generate greedy solution from every possible start
    for(unsigned int i = 0; i < cities.size(); i++)
    {
        // Only clear after first iteration
        tempTour.tour.clear();
        tempTour.cost = 0;

        // Reset all cities visited status
        for(unsigned int j = 0; j < cities.size(); j++)
            cities.at(j).added = false;

        // Add first edge
        cities.at(i).added = true;
        findClosestCity(cities.at(i));
 
        // Add rest of edges
        while(!allCitiesAdded())
            findClosestCity(tempTour.tour.back().b);

        // Push final edge
        tempTour.tour.push_back(Link(tempTour.tour.back().b, cities.at(i)));

        // Calculate final cost
        for(auto & link : tempTour.tour)
            tempTour.cost += link.distance;

        // If this tour cheaper than current cheapest, replace it
        if(cheapestTour.cost == 0 || tempTour.cost < cheapestTour.cost)
            cheapestTour = tempTour;
    }

    // Subtract current time from cheapestTour time
    cheapestTour.time -= clock();

    // Divide time by CPS to get time in secs
    cheapestTour.time /= (double) CLOCKS_PER_SEC;

    // Time is currently negative so flip sign and convert to ms
    cheapestTour.time *= -1000;

    // Tours calculated is city count
    this->tourCount = cities.size();
}

// All cities added
bool DataSet::allCitiesAdded()
{
    for(auto & city : cities)
        if(!city.added)
            return false;
    
    return true;
}

// Find closest city to city
void DataSet::findClosestCity(City c1)
{
    Link cheapestLink;
    unsigned int cheapestCity = 0;

    // Iterate over cities
    for(unsigned int i = 0; i < cities.size(); i++)
    {
        // If this edge is cheaper, replace cheapest and its not itself
        if(!cities.at(i).added)
        {
            Link temp = Link(c1, cities.at(i));
        
            if(cheapestLink.distance == -1 || temp.distance < cheapestLink.distance)
            {
                cheapestLink = temp;
                cheapestCity = i;
            }
        }
    }

    // Print cheapest link
    cities.at(cheapestCity).added = true;
    tempTour.tour.push_back(cheapestLink);
}

// Genetic algorithm
void DataSet::genetic()
{
    // Start clock
    //std::srand(std::time(0));
    //cheapestTour.time = clock();

    // Initialize Population
    initPop();

    // Repeat gen times
    while(genCount < 200000)
    {
        // Sort pop
        sortPop();

        // Crossover random parents, prune weakest
        crossPop();

        // Mutate according to mutateFactor
        mutatePop();

        // Update gen count
        genCount++;
    }

    // Final sort
    sortPop();

    // Make fittest individual in population our solution
    //cheapestTour = population.at(0);

    // Subtract current time from cheapestTour time
    //cheapestTour.time -= clock();

    // Divide time by CPS to get time in secs
    //cheapestTour.time /= (double) CLOCKS_PER_SEC;

    // Time is currently negative so flip sign and convert to ms
    //cheapestTour.time *= -1000;
}

// Initializes population for GA
void DataSet::initPop()
{
    // Generate greedy solution from every possible start
    for(unsigned int i = 0; i < cities.size(); i++)
    {
        // Only clear after first iteration
        tempTour.tour.clear();
        tempTour.cost = 0;

        // Reset all cities visited status
        for(unsigned int j = 0; j < cities.size(); j++)
            cities.at(j).added = false;

        // Add first edge
        cities.at(i).added = true;
        findClosestCity(cities.at(i));
 
        // Add rest of edges
        while(!allCitiesAdded())
            findClosestCity(tempTour.tour.back().b);

        // Push final edge
        tempTour.tour.push_back(Link(tempTour.tour.back().b, cities.at(i)));

        // Calculate final cost
        for(auto & link : tempTour.tour)
            tempTour.cost += link.distance;

        population.push_back(tempTour);
    }

    unsigned int remaining = 0;
    if(cities.size() < popSize)
        remaining = popSize - cities.size();

    Tour temp;
    for(unsigned int i = 0; i < remaining; i++)
    {
        temp.tour.clear();
        temp.cost = 0;
        std::random_shuffle(cities.begin(), cities.end());
        for(unsigned int j = 0; j < cities.size() - 1; j++)
            temp.tour.push_back(Link(cities.at(j), cities.at(j+1)));

        temp.tour.push_back(Link(temp.tour.back().b, cities.at(0)));

        for(Link link : temp.tour)
            temp.cost += link.distance;

        population.push_back(temp);
    }
}

// Prints population
void DataSet::printPop()
{
    int i = 0;
    for(auto & tour : population)
    {
        std::cout << std::setfill('0') << std::setw(3) << i << ") [ " << tour.tour.at(0).a.num << " ";

        // Print links from tour
        for(auto & link : tour.tour)
            std::cout << link.b.num << " ";

        std::cout << "]: $" << toStrMaxDecimals(tour.cost, 2) << std::endl;
        i++;
    }
}

// Sort population
void DataSet::sortPop()
{
    // Standard sort
    std::sort(population.begin(), population.end());
}

// Crossover population
void DataSet::crossPop()
{
    int rand1, rand2;
    int children = 3;

    // Kill off weakest parents
    population.resize(population.size() - children);
    for(int i = 0; i < children; i++)
    {
        rand1 = rand() % popSize % (population.size() - 1);

        rand2 = rand() % (population.size() - 1);
        while(rand2 == rand1)
            rand2 = rand() % (population.size() - 1);

        Tour child = crossover(population.at(rand1), population.at(rand2));

        // Assimilate children into population
        population.push_back(child);
    }
}

// Mutate population (according to mutate factor
void DataSet::mutatePop()
{
    if(mutate == 1)
    {
        // If random value falls within mutateFactor
        if(((double) rand() / RAND_MAX) < mutateFactor)
        {
            int popIndex = rand() %  population.size();
            int cityIndex1 = (rand() % (cities.size() - 2)) + 1;

            int cityIndex2 = (rand() % (cities.size() - 2)) + 1;
            while(cityIndex2 == cityIndex1)
                cityIndex2 = (rand() % (cities.size() - 2)) + 1;
            
            // Swap
            City temp = population.at(popIndex).tour.at(cityIndex1).b;
            City temp2 = population.at(popIndex).tour.at(cityIndex2).b;
            population.at(popIndex).tour.at(cityIndex1).b = population.at(popIndex).tour.at(cityIndex2).b;
            population.at(popIndex).tour.at(cityIndex2).b = temp;

            // Repair
            population.at(popIndex).tour.at(cityIndex2 + 1).a = temp;
            population.at(popIndex).tour.at(cityIndex1 + 1).a = temp2;

            // Update cost
            population.at(popIndex).cost = 0.0;
            for(unsigned int i = 0; i < cities.size(); i++)
                population.at(popIndex).tour.at(i).dist(population.at(popIndex).tour.at(i).a, population.at(popIndex).tour.at(i).b);
            
            for(Link link : population.at(popIndex).tour)
                population.at(popIndex).cost += link.distance;

            mutateCount++;
        }
    }
    else if(mutate == 2)
    {
        // If random value falls within mutateFactor
        if(((double) rand() / RAND_MAX) < mutateFactor)
        {
            int popIndex = rand() %  population.size();
            int cityIndex = (rand() % (cities.size() - 2)) + 1;
            
            // Swap
            City temp = population.at(popIndex).tour.at(0).a;
            population.at(popIndex).tour.at(0).a = population.at(popIndex).tour.at(cityIndex).b;
            population.at(popIndex).tour.at(cityIndex).b = temp;

            // Repair
            population.at(popIndex).tour.at(cityIndex + 1).a = temp;
            population.at(popIndex).tour.back().b = population.at(popIndex).tour.front().a;

            // Update cost
            population.at(popIndex).cost = 0.0;
            for(unsigned int i = 0; i < cities.size(); i++)
                population.at(popIndex).tour.at(i).dist(population.at(popIndex).tour.at(i).a, population.at(popIndex).tour.at(i).b);
            
            for(Link link : population.at(popIndex).tour)
                population.at(popIndex).cost += link.distance;

            mutateCount++;
        }
    }
}

// Crossover population helper
Tour DataSet::crossover(const Tour& parent1, const Tour& parent2)
{
    // Child
    Tour child;
    child.cost = 0;
 
    if(cross == 1)
    {
        City children[cities.size()];
        bool childSpots[cities.size()] = { false };
        for(unsigned int i = 0; i < cities.size(); i++)
        {
            // Alternate parents, Grab from parent 2 if we can
            if(i % 2 && !childSpots[parent2.tour.at(i).a.num - 1])
            {
                childSpots[parent2.tour.at(i).a.num - 1] = true;
                children[i] = parent2.tour.at(i).a;
            }
            // Grab from parent 1 if we can
            else if(!childSpots[parent1.tour.at(i).a.num - 1])
            {
                childSpots[parent1.tour.at(i).a.num - 1] = true;
                children[i] = parent1.tour.at(i).a;
            }
            else
            {   
                for(unsigned int j = 0; j < cities.size(); j++)
                {
                    if(!childSpots[cities.at(j).num - 1])
                    {
                        children[i] = cities.at(j);
                        childSpots[cities.at(j).num - 1] = true;
                        break;
                    }
                }
            }
        }

        // Create tour
        for(unsigned int i = 0; i < cities.size() - 1; i++)
            child.tour.push_back(Link(children[i], children[i+1]));

        // Add final edge
        child.tour.push_back(Link(child.tour.back().b, child.tour.front().a));

        // Calculate cost
        for(Link link : child.tour)
            child.cost += link.distance;
        
        return child;
    }
    else if(cross == 2)
    {
        std::vector<City> citylist;
    
        if((rand() % 2) == 0)
            citylist.push_back(parent1.tour.at(0).a);
        else
            citylist.push_back(parent2.tour.at(0).a);

        // Greedy crossover
        for(unsigned int i = 1; i < cities.size(); i++)
        {
            Link l1 = Link(citylist[i - 1], parent1.tour.at(i).a);
            Link l2 = Link(citylist[i - 1], parent2.tour.at(i).a);

            // Pick l1
            if(std::find(citylist.begin(), citylist.end(), parent1.tour.at(i).a) == citylist.end() && l1.distance > 0)
            {
                citylist.push_back(parent1.tour.at(i).a);
            }
            // Else l2
            else if(std::find(citylist.begin(), citylist.end(), parent2.tour.at(i).a) == citylist.end() && l2.distance != 0)
            {
                citylist.push_back(parent2.tour.at(i).a);
            }
            // Else grab next unvisited city
            else
            {
                for(unsigned int i = 0; i < cities.size(); i++)
                {
                    if(std::find(citylist.begin(), citylist.end(), parent1.tour.at(i).a) == citylist.end())
                    {
                        citylist.push_back(parent1.tour.at(i).a);
                        break;
                    }
                    else if(std::find(citylist.begin(), citylist.end(), parent2.tour.at(i).a) == citylist.end())
                    {
                        citylist.push_back(parent2.tour.at(i).a);
                        break;
                    }
                    
                }
            }
        }

        // Build tour
        for(unsigned int i = 0; i < cities.size() - 1; i++)
            child.tour.push_back(Link(citylist.at(i), citylist.at(i+1)));
    
        // Add final edge
        child.tour.push_back(Link(child.tour.back().b, child.tour.front().a));

        // Update cost
        for(Link link : child.tour)
            child.cost += link.distance;
        
        // Reset visited status
        for(unsigned int i = 0; i < cities.size(); i++)
            cities.at(i).added = false;

        return child;
    }

    return child;
}

// Wisdom of crowds
void DataSet::wisdom()
{
    // Get time
    std::srand(std::time(0));
    cheapestTour.time = clock();

    unsigned int expertCount = 10;
    unsigned int adjacency[expertCount][cities.size()] = { 0 };
    unsigned int frequency[cities.size()][cities.size()] = { 0 };
    std::vector<unsigned int> max;
    std::vector<Tour> experts;

    // Get our experts
    for(unsigned int i = 0; i < expertCount; i++)
    {
        population.clear();
        genetic();
        experts.push_back(population.at(0));
        std::cout << "E" << std::setw(2) << std::setfill('0') << i + 1 << ": " << population.at(0).cost << std::endl;
        genCount = 0;
    }
    std::cout << std::endl;

    // Get adjacency matrix
    for(unsigned int i = 0; i < experts.size(); i++)
        for(unsigned int j = 0; j < cities.size(); j++)
            adjacency[i][j] = experts.at(i).tour.at(j).a.num;

    // Get frequency matrix
    for(unsigned int position = 0; position < cities.size(); position++)
    {
        for(unsigned int city = 0; city < cities.size(); city++)
        {
            unsigned int sum = 0;
            frequency[position][city] = 0;
            for(unsigned int expert = 0; expert < experts.size(); expert++)
            {   
                if(adjacency[expert][position] == city + 1)
                {
                    sum++;
                }
            }

            frequency[position][city] = sum;
        }
    }

    // Get maximum matrix
    for(unsigned int i = 0; i < cities.size(); i++)
    {
        int maxindex = -1;
        unsigned int maximum = 0;
        for(unsigned int j = 0; j < cities.size(); j++)
        {
            if(std::find(max.begin(), max.end(), j) == max.end() && frequency[i][j] > maximum)
            {
                maximum = frequency[i][j];
                maxindex = j;
            }
        }

        if(maxindex == -1)
        {
            for(unsigned int j = 0; j < cities.size(); j++)
            {
                if(std::find(max.begin(), max.end(), cities.at(j).num) == max.end())
                {
                    maxindex = cities.at(j).num;
                    break;
                }
            }
        }
    
        std::cout << maxindex << " ";
        max.push_back(maxindex);
    }

    // Build tour
    std::sort(cities.begin(), cities.end());
    cheapestTour.tour.clear();
    for(unsigned int i = 0; i < cities.size() - 1; i++)
        cheapestTour.tour.push_back(Link(cities.at(max[i]), cities.at(max[i+1])));

    // Add final edge
    cheapestTour.tour.push_back(Link(cheapestTour.tour.back().b, cheapestTour.tour.front().a));

    // Update cost
    for(Link link: cheapestTour.tour)
        cheapestTour.cost += link.distance;

    // Subtract current time from cheapestTour time
    cheapestTour.time -= clock();

    // Divide time by CPS to get time in secs
    cheapestTour.time /= (double) CLOCKS_PER_SEC;

    // Time is currently negative so flip sign and convert to ms
    cheapestTour.time *= -1000;
}
