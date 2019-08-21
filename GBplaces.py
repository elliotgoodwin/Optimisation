#GBPlaces graph
#Uses the GBplaces data to plot a graph
#student iD - 9621958

#import packages
import matplotlib.pyplot as plt;
import matplotlib.lines as mlines;

#create empty arrays for data to fill
place = [];
type = [];
population = [];
lattitude = [];
longitude = [];

placeC = [];
lattitudeC = [];
longitudeC = [];
popC = [];

placeT = [];
lattitudeT = [];
longitudeT = [];
popT = [];


#variable to test if file opened
opened = 0

try:
    
    #try to open file
    readfile = open('GBplaces.csv','r');
    opened = 1
    print('\nThe data set is:\n');

except:
    
    #programme comes here if file doesn't open
    print('\nAn error has occurred.');

if opened:
    
    #skip headers
    next(readfile);
    
    #programme comes here if file opens
    #split the data into lines
    for line in readfile:
        
        #splits each line according to ',' separation 
        splitup = line.split(',');
        
        #append data to arrays created earlier
        place.append(splitup[0]);
        type.append(splitup[1]);
        population.append(float(splitup[2]));
        lattitude.append(float(splitup[3]));
        longitude.append(float(splitup[4]));
        
        #print data to ensure it has been input correctly
        print(splitup[0]+', '+splitup[1]+', '+splitup[2]+', '+splitup[3]+', '+splitup[4]);
    

    for i in range(len(type)):
        
        if type[i] == 'City':
            
            placeC.append(place[i]);
            lattitudeC.append(lattitude[i]);
            longitudeC.append(longitude[i]);
            popC.append(population[i]/2000);

        else:
            
            placeT.append(place[i]);
            lattitudeT.append(lattitude[i]);
            longitudeT.append(longitude[i]);
            popT.append(population[i]/2000);

    #create a figure
    #'colourblind friendly' colour scheme and hex codes taken from:
    #http://tableaufriction.blogspot.co.uk/2012/11/finally-you-can-use-tableau-data-colors.html
    plt.figure(figsize=(7,6))
    plt.scatter(longitudeC, lattitudeC, color='#006ba4', s=popC, alpha=0.7);
    plt.scatter(longitudeT, lattitudeT, color='#c85200', s=popT, alpha=0.7);

    #create a legend
    townmarker = mlines.Line2D([], [], color='#006ba4', linestyle='', marker='s', markersize=14, label='Town');
    citymarker = mlines.Line2D([], [], color='#c85200', linestyle='', marker='s', markersize=14, label='City');
    #population marker size scaled using Lincoln's population (~100,000)
    popmarker = mlines.Line2D([], [], color='#a5acaf', linestyle='', marker='o',
                          markersize=(population[46]/5000), label='40,000 people');

    plt.legend(numpoints=1, handles=[townmarker, citymarker, popmarker]);

    #define variable for colour of axes and labels
    grey='#595959';

    #add axes labels and increase font size, so they can be more easily read
    plt.ylabel('Lattitude', fontsize=16, color=grey);
    plt.xlabel('Longitude', fontsize=16, color=grey);

    #add chart title
    plt.title('Relative populations in towns and cities across Great Britain\n', color=grey, fontsize=16);

    #remove border from top and right hand side of graph
    ax = plt.subplot(111);
    ax.spines['top'].set_visible(False);
    ax.spines['right'].set_visible(False);
    ax.spines['bottom'].set_color(grey);
    ax.spines['left'].set_color(grey);
    
    #remove 'ticks' from top and right hand side of graph
    ax.get_xaxis().tick_bottom();
    ax.get_yaxis().tick_left();
    
    #increase the font size of the scale, so it can be more easily read
    plt.yticks(fontsize=13, color=grey);
    plt.xticks(fontsize=13, color=grey);
    
    #superpose a grid over the plot to help read off values
    plt.grid();
    
    #show the final plot
    plt.show();