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
        std::cout << "Reading from: " << filename << std::endl;

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

// Generate tours (BRUTE FORCE)
void DataSet::generateTours()
{
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
            tour.cost += tour.tour.at(i).getDistance();
        }

        // Generate link to first city
        tour.tour.push_back(Link(cities.back(), cities.front()));
        tour.cost += tour.tour.back().getDistance();

        // Adjust cheapest cost if need be
        if(tour.cost < cheapestTour.cost || cheapestTour.cost == 0)
            cheapestTour = tour;

        // Adjust tourCount
        tourCount++;
        tour.cost = 0;
    } while(std::next_permutation(cities.begin(), cities.end()));
}

// Print results
void DataSet::printResults()
{
    // Log final results
    std::cout << std::endl;
    std::cout << "-------- FINAL RESULTS ------" << std::endl;
    std::cout << "Cities: " << cities.size() << std::endl;
    std::cout << "Tours Calculated: " << tourCount << std::endl;

    std::cout << std::endl;
    for(Link link : cheapestTour.tour)
        std::cout << "Link Joins: " << link.getA().getNum() << " " << link.getB().getNum() << "  \t$" << link.getDistance() << std::endl;

    std::cout << "Cheapest Tour: " << cheapestTour.cost << std::endl;
}

// Print graphics
void DataSet::printGraph(int argc, char** argv)
{
    // Graphics vars
    GtkWidget *window;
    GtkWidget *darea;
    
    // Initialize gtk window
    gtk_init(&argc, &argv);
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
    cairo_set_source_rgb(cr, 0.8, 0.8, 0.8);
    cairo_paint(cr);
    cairo_set_source_rgb(cr, 0, 0, 0);

    // Print all links
    for(Link link : ds.getCheapestTour().tour)
    {
        cairo_move_to(cr, link.getA().getX() * 3, link.getA().getY() * 3);
        cairo_line_to(cr, link.getB().getX() * 3, link.getB().getY() * 3);
        cairo_stroke (cr);
    }

    // Print all cities 
    for(City city : ds.getCities())
    {
        // Create gradient for cities
        cairo_pattern_t* r1;
        r1 = cairo_pattern_create_radial(city.getX() * 3, city.getY() * 3, 3, city.getX() * 3, city.getY() * 3, 11);  
        cairo_pattern_add_color_stop_rgba(r1, 0, 1, 1, 1, 1);
        cairo_pattern_add_color_stop_rgba(r1, 1, 0.6, 0.6, 0.6, 1);

        // Paint city
        cairo_set_source(cr, r1);
        cairo_arc(cr, city.getX() * 3, city.getY() * 3, 11, 0, 2*M_PI);
        cairo_fill(cr); 
    }

    // Setup text format
    cairo_select_font_face (cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (cr, 35.0);

    // Setup cheapest tour string
    std::string cheapestTourString = "Cheapest Tour: ";
    cheapestTourString.append(toStrMaxDecimals(ds.getCheapestTour().cost, 2));

    // Print string and format it
    cairo_move_to (cr, 15, gdk_screen_height() - 40);
    cairo_text_path (cr, cheapestTourString.c_str());
    cairo_set_source_rgb (cr, 1, 1, 1);
    cairo_fill_preserve (cr);

    // Print border on string
    cairo_set_source_rgb (cr, 0, 0, 0);
    cairo_set_line_width (cr, 1.56);
    cairo_stroke (cr);
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
    {
        s.erase(s.size() - decimals + 1);
    }

    return s;
}
