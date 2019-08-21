# GBPlaces visualisation
# Visualisation of the data in GBplaces.csv (Place, Population, Lattitude, Longitude)
# Elliot Goodwin

# Import packages
import matplotlib.pyplot as plt
import matplotlib.lines as mlines

#create empty arrays for data to fill
place = []
type = []
population = []
lattitude = []
longitude = []

# Separate cities (C) and town (T)
placeC = []
lattitudeC = []
longitudeC = []
popC = []

placeT = []
lattitudeT = []
longitudeT = []
popT = []

# Open .csv file
opened = False

try:
    readfile = open('GBplaces.csv','r')
    opened = True
except:
    print('\nError. Cannot open file.')
    
if opened:
    # Skip headers
    next(readfile)
    
    # Split the data into lines
    for line in readfile:
        # Splits each line according to ',' separation 
        splitup = line.split(',')
        
        # Append data to arrays
        place.append(splitup[0])
        type.append(splitup[1])
        population.append(float(splitup[2]))
        lattitude.append(float(splitup[3]))
        longitude.append(float(splitup[4]))
        
        # Could try making a dictionary of all place names and then adding labels to high population cities on the map
        
        # Print data to ensure it has been input correctly
        # print(splitup[0]+', '+splitup[1]+', '+splitup[2]+', '+splitup[3]+', '+splitup[4])
    
    # some normalisation factor
    norm_factor = 2000
    
    for i in range(len(type)):
        if type[i] == 'City':
            placeC.append(place[i])
            lattitudeC.append(lattitude[i])
            longitudeC.append(longitude[i])
            popC.append(population[i]/norm_factor)

        else:
            
            placeT.append(place[i])
            lattitudeT.append(lattitude[i])
            longitudeT.append(longitude[i])
            popT.append(population[i]/norm_factor)

    # Create a figure
    # 'Colour blind friendly' colour scheme and hex codes taken from:
    # http://tableaufriction.blogspot.co.uk/2012/11/finally-you-can-use-tableau-data-colors.html
    kOrange    = '#006ba4'
    kBlue      = '#c85200'
    kDarkGrey  = '#595959'
    kLightGrey = '#a5acaf'
    
    plt.figure(figsize=(7,6))
    plt.scatter(longitudeC, lattitudeC, color=kOrange, s=popC, alpha=0.7)
    plt.scatter(longitudeT, lattitudeT, color=kBlue, s=popT, alpha=0.7)
                
    # Create a legend
    townmarker = mlines.Line2D([], [], color=kOrange, linestyle='', marker='s', markersize=12, label='Town')
    citymarker = mlines.Line2D([], [], color=kBlue, linestyle='', marker='s', markersize=12, label='City')
    # Population marker size scaled using Lincoln's population (approx. 100,000)
    popmarker = mlines.Line2D([], [], color=kLightGrey, linestyle='', marker='o',
                          markersize=(population[46]/5000), label='40,000 people')

    plt.legend(numpoints=1, handles=[townmarker, citymarker, popmarker], labelspacing=1)

    # Add axes labels and increase font size, so they can be more easily read
    plt.ylabel('Lattitude', fontsize=16, color=kDarkGrey)
    plt.xlabel('Longitude', fontsize=16, color=kDarkGrey)

    # Add chart title
    plt.title('Relative populations of towns and cities across Great Britain\n', color=kDarkGrey, fontsize=16);

    # Remove border from top and right hand side of graph
    ax = plt.subplot(111);
    ax.spines['top'].set_visible(False);
    ax.spines['right'].set_visible(False);
    ax.spines['bottom'].set_color(grey);
    ax.spines['left'].set_color(grey);
    
    # Remove 'ticks' from top and right hand side of graph
    ax.get_xaxis().tick_bottom();
    ax.get_yaxis().tick_left();
    
    # Increase axes font-size (improve readability)
    plt.yticks(fontsize=16, color=kDarkGrey);
    plt.xticks(fontsize=16, color=kDarkGrey);
    
    # Add a grid
    # plt.grid();
    
    # Show the final plot
    plt.show();
