// Optimisation
// A programme that finds the optimum location of a logistics hub in Great Britain, using data read in from a .csv file

// includes
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include <ctime>

// global variables
double PI = 3.14159;
double R = 3958.75;
int distance_evals = 0;
int w_distance_evals = 0;


// function definitions

// radians
// converts the argument of the function from degrees to radians
double radians(double degrees) {
    // work out the value of the function
    double r = degrees * PI / 180;
    // and return it
    return r;
}

// distance
// calculates the linear distance (in miles) between two places,
// given their lattitude and longitudes (in degrees) using the Haversine formula
double distance(double long1, double lat1, double long2, double lat2) {
    
    double a, dLat, dLong, c, distance;
    dLat = radians(lat2) - radians(lat1);
    dLong = radians(long2) - radians(long1);
    a = pow(sin(dLat/2),2) + cos(radians(lat1)) * cos(radians(lat2)) * pow(sin(dLong/2),2);
    c = 2 * atan2(sqrt(a), sqrt(1-a));
    distance = R * c;
    distance_evals++;
    return distance;
}

// w_distance
// scales the distance between two points, by multiplying by a weighting factor
// that is proportional to the popuation of the place
double w_distance(double long1, double lat1, double long2, double lat2, double weight) {
    
    double a, dLat, dLong, c, distance;
    dLat = radians(lat2) - radians(lat1);
    dLong = radians(long2) - radians(long1);
    a = pow(sin(dLat/2),2) + cos(radians(lat1)) * cos(radians(lat2)) * pow(sin(dLong/2),2);
    c = 2 * atan2(sqrt(a), sqrt(1-a));
    distance = R * c * pow(weight, 1)/10000;
    w_distance_evals++;
    return distance;
}

// random_number
// generates a random number in a particular range
double random_number ( double upper, double lower, int n ) {
    double r;
    r = lower + ( rand() % (n+1) * (1./n) * (upper-lower));
    return r;
}


using namespace std;


// main programme starts here
int main() {
    
    // declare variables
    vector <string> Place, Type;
    vector <double> Pop, Long, Lat, s_Lat, s_Long, n_Lat, n_Long;
    string line;
    double step = 0.01, globalMin = 9999999, globalMin1 = 999999;
    double value, oldValue, newValue, x, y, glx, gly, dst, newdst, min_dst, n_glx,
           n_gly, n_globalMin, n_distance_evals, s_distance_evals, s_glx, s_gly, s_globalMin;
    int dx, dy;
    
    // seed pseudo-random number generator (to ensure numbers are actually random!)
    srand(time(NULL));
    
    // read in GBplaces.csv data
    ifstream thefile("GBplaces.csv");
    
    // check the file is open
    if (thefile.is_open()) {
        
        // skip the headers
        thefile.ignore(1000, '\n');
        
        // while the end of the file hasn't been reached
        while (!thefile.eof()) {
            
            // read in each line of data and append it to the string 'line'
            getline(thefile, line);
            
            // if the line is not empty
            if ( line.length() > 0 ) {
                
                // add data before the first comma to vector 'Place'
                Place.push_back(line.substr(0, line.find(",")));
                // define positions of next substring
                long pos1 = line.find(",") + 1;
                long pos2 = line.find(",", pos1);
                // add substring to vector 'Type'
                Type.push_back(line.substr(pos1, pos2 - pos1));
                // repeat for vectors "Pop", "Lat" and "Long"
                long pos3 = line.find(",", pos2 + 1);
                Pop.push_back(atof(line.substr(pos2 + 1, pos3 - pos2 - 1).c_str()));
                long pos4 = line.find(",", pos3 + 1);
                Lat.push_back(atof(line.substr(pos3 + 1, pos4 - pos3 - 1).c_str()));
                long pos5 = line.find(",", pos4 + 1);
                Long.push_back(atof(line.substr(pos4 + 1, pos5 - pos4 - 1).c_str()));
                
            }
        }
        
        // close the file
        thefile.close();
        
        
        /* assuming one hub, from which only one place can be travelled to at any one time,
         the optimum location is the position with the minimum sum of distances to all places */
        
        
        // find position with minimum sum of distances
        
        // do 50 iterations
        for ( int k = 0; k < 50; k++ ) {
            
            // pick a random start point inside the range of
            // longitude and lattitude fom GBPlaces
            x = random_number (-4.25763, 1.29834, 100000);
            y = random_number (50.37153, 57.14369, 100000);
            
            // find sum of distances from initial point to each place
            value = 0;
            for (int m = 0; m < Place.size(); m++) {
                value += distance(x, y, Long[m], Lat[m]);
            }
            
            // look around the point for one with a lower sum of distances
            do {
                
                oldValue = value;
                newValue = 0;
                
                // take a unit 'step' in each direction around the initial point
                for (int i = -1; i < 1; i++) {
                    for (int j = -1; j < 1; j++) {
                        // do nothing at the initial point
                        if ( i == 0 && j == 0 ) {
                        }
                        // find the sum of distances at each of the new points
                        else {
                            for (int m = 0; m < Place.size(); m++) {
                                newValue += distance(x + (step*i), y + (step*j), Long[m], Lat[m]);
                            }
                            // if value is smaller than the previous distance, update variables
                            if ( newValue <= value ) {
                                dx = i;
                                dy = j;
                                value = newValue;
                            }
                        }
                    }
                }
                
                // update the position
                x += step * dx;
                y += step * dy;
                
            } while ( value < oldValue );
            
            // if value is smaller, update global variables
            if (value < globalMin) {
                globalMin = value;
                glx = x;
                gly = y;
            }
        }
        
        // print out optimum position
        cout << "Assuming a singular hub, from which only one place can be travelled to at any one time:\n" << endl;
        cout << "Optimum location is " << gly << " " << glx << "." << endl;
        cout << "Sum of distances = " << globalMin << " miles." << endl;
        cout << "Number of iterations = " << distance_evals << ".\n\n\n" <<endl;
        
        
        /* An improved optimum location can be found by weighting the distances to a place by its population.
           A place with a larger population will therefore be weighted more heavily, and have a smaller 'weighted distance'. */
        
        
        // find the place with the minimum sum of weighted distances
        
        // do 50 iterations
        for ( int k = 0; k < 50; k++ ) {
            
            // pick a random start point inside the range of
            // longitude and lattitude from GBPlaces
            x = random_number (-4.25763, 1.29834, 100000);
            y = random_number (50.37153, 57.14369, 100000);
            
            // find sum of weighted distances from initial point to each place
            value = 0;
            for (int m = 0; m < Place.size(); m++) {
                value += w_distance(x, y, Long[m], Lat[m], Pop[m]);
            }
            
            // look around the point for one with a lower sum of weighted distances
            do {
                
                oldValue = value;
                newValue = 0;
                dst = 0;
                
                // take a unit 'step' in each direction around the initial point
                for (int i = -1; i < 1; i++) {
                    for (int j = -1; j < 1; j++) {
                        // do nothing at the initial point
                        if ( i == 0 && j == 0 ) {
                        }
                        // find the sum of weighted distances at each of the new points
                        // and the sum of distances to compare to the first case
                        else {
                            for (int m = 0; m < Place.size(); m++) {
                                newValue += w_distance(x + (step*i), y + (step*j), Long[m], Lat[m], Pop[m]);
                                dst += distance(x + (step*i), y + (step*j), Long[m], Lat[m]);
                            }
                            // if value is smaller than the previous weighted distance, update variables
                            if ( newValue <= value ) {
                                dx = i;
                                dy = j;
                                value = newValue;
                                newdst = dst;
                            }
                        }
                    }
                }
                
                // update the position
                x += step * dx;
                y += step * dy;
                
            } while ( value < oldValue );
            
            // if value is smaller, update global variables
            if (value < globalMin1) {
                globalMin1 = value;
                min_dst = newdst;
                glx = x;
                gly = y;
            }
        }
        
        // print out optimum position weighted by population size
        cout << "For a single hub, with distances weighted by population size:\n" << endl;
        cout << "Optimum location is " << gly << " " << glx << "." << endl;
        // cout << "Sum of weighted distances = " << globalMin1 << " miles." << endl;
        cout << "Sum of distances = " << min_dst << " miles." << endl;
        cout << "Number of iterations = " << w_distance_evals << ".\n\n\n" << endl;
        
        
        /*  Another improvement can be made by assuming two hubs, each located in the North
            and South of the country, respectively.  This can be done by further splitting
            the vectors containing Longitude and Lattitude data, in to North and South Longitudes
            and Lattitudes.  The Lattitude dividing North and South was defined as the midpoint
            of the range of Lattitudes given in GBPlaces, which was 53.75761.  */
        
        
        // split arrays in to places in the north and south
        for (int i = 0; i < Lat.size(); i++) {
            if (Lat[i] <= 53.75761) {
                s_Lat.push_back(Lat[i]);
                s_Long.push_back(Long[i]);
            }
            else if (Lat[i] > 53.75761) {
                n_Lat.push_back(Lat[i]);
                n_Long.push_back(Long[i]);
            }
        }
        
        // find optimum place in the North
        for (int l = 0; l < n_Lat.size(); l++); {
            // do 50 iterations
            for ( int k = 0; k < 50; k++ ) {
                
                // pick a random start point inside the range of
                // Northern longitude and lattitude from GBPlaces
                x = random_number (-4.25763, 1.29834, 100000);
                y = random_number (53.75761, 57.14369, 100000);
                
                // find sum of distances from initial point to each place
                value = 0;
                for (int m = 0; m < n_Lat.size(); m++) {
                    value += distance(x, y, n_Long[m], n_Lat[m]);
                }
                
                // look around the point for one with a lower sum of distances
                do {
                    
                    oldValue = value;
                    newValue = 0;
                    
                    // take a unit 'step' in each direction around the initial point
                    for (int i = -1; i < 1; i++) {
                        for (int j = -1; j < 1; j++) {
                            // do nothing at the initial point
                            if ( i == 0 && j == 0 ) {
                            }
                            // find the sum of distances at each of the new points
                            else {
                                for (int m = 0; m < n_Lat.size(); m++) {
                                    newValue += distance(x + (step*i), y + (step*j), n_Long[m], n_Lat[m]);
                                }
                                // if value is smaller than the previous distance, update variables
                                if ( newValue <= value ) {
                                    dx = i;
                                    dy = j;
                                    value = newValue;
                                }
                            }
                        }
                    }
                    
                    // update the position
                    x += step * dx;
                    y += step * dy;
                    
                } while ( value < oldValue );
                
                // if value is smaller, update global variables
                if (value < globalMin) {
                    n_globalMin = value;
                    n_glx = x;
                    n_gly = y;
                }
            }
            n_distance_evals = distance_evals;
        }
        
        // print out the optimum position
        cout << "Assuming two hubs, one in the North and one in the South (above and below Lat = 53.75761, respectively):\n" << endl;
        cout << "Optimum location of Northern hub is " << n_gly << " " << n_glx << "." << endl;
        cout << "Sum of distances from the Northern hub = " << n_globalMin << " miles." << endl;
        cout << "Number of iterations = " << n_distance_evals << ".\n" <<endl;
        
        // find optimum place in the South
        for (int l = 0; l < s_Lat.size(); l++); {
            // do 50 iterations
            for ( int k = 0; k < 50; k++ ) {
                
                // pick a random start point inside the range of
                // Southern longitude and lattitude from GBPlaces
                x = random_number (-4.25763, 1.29834, 100000);
                y = random_number (50.37153, 53.75761, 100000);
                
                // find sum of distances from initial point to each place
                value = 0;
                for (int m = 0; m < s_Lat.size(); m++) {
                    value += distance(x, y, s_Long[m], s_Lat[m]);
                }
                
                // look around the point for one with a lower sum of distances
                do {
                    
                    oldValue = value;
                    newValue = 0;
                    
                    // take a unit 'step' in each direction around the initial point
                    for (int i = -1; i < 1; i++) {
                        for (int j = -1; j < 1; j++) {
                            // do nothing at the initial point
                            if ( i == 0 && j == 0 ) {
                            }
                            // find the sum of distances at each of the new points
                            else {
                                for (int m = 0; m < s_Lat.size(); m++) {
                                    newValue += distance(x + (step*i), y + (step*j), s_Long[m], s_Lat[m]);
                                }
                                // if value is smaller than the previous distance, update variables
                                if ( newValue <= value ) {
                                    dx = i;
                                    dy = j;
                                    value = newValue;
                                }
                            }
                        }
                    }
                    
                    // update the position
                    x += step * dx;
                    y += step * dy;
                    
                } while ( value < oldValue );
                
                // if value is smaller, update global variables
                if (value < globalMin) {
                    s_globalMin = value;
                    s_glx = x;
                    s_gly = y;
                }
            }
            s_distance_evals = distance_evals;
        }
        
        // print out optimum position
        cout << "Optimum location of Southern hub is " << s_gly << " " << s_glx << "." << endl;
        cout << "Sum of distances from the Southern hub = " << s_globalMin << " miles." << endl;
        cout << "Number of iterations = " << s_distance_evals << ".\n" <<endl;
        
    }
    
    else { //matches 'if (thefile.is_open())'
        
        // write out an error if data not read in
        cout << "Unable to read file.\n";
        exit(1);
    }
    
    return 0;
}
