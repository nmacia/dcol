
#ifndef RESTULTSCONTAINER_H
#define RESTULTSCONTAINER_H

#include <fstream>
#include <iostream>
#include <iomanip>


/*!

\brief Stores the results.

This class stores the results of runs over a single or multiple data sets.
It maintains the names of the data sets and the complexity measures results for each one.

@author   Albert Orriols-Puig and Nuria Macia <br>
          Grup de Recerca en Sistemes Intel.ligents <br>
          La Salle - Universitat Ramon Llull <br>
          C/ Quatre Camins, 2. 08022, Barcelona (Spain) <br>
@date     Created April, 2009 <br>
          Last modified December, 2010

Copyright (C) 2009  Albert Orriols-Puig and Nuria Macia

This file is part of DCoL.

DCoL is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

DCoL is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with DCoL.  If not, see <http://www.gnu.org/licenses/>.

*/


class ResultsContainer {

    /** Names of the data sets. */
    std::string* datasetNames;

    /** Results for each data set. */
    float** results;

    /** Number of data sets for which there are valid results. */
    int numberOfDatasets;

    /** Size of the container. */
    int capacity;

    /** Minimum capacity of container of results. */
    static const int MIN_CAPACITY = 100;

    /** Capacity with which the container has to be increased in case of not enough space. */
    static const int INC_CAPACITY = 100;

public:

    /** Constructs a new ResultsContainer object. */
    ResultsContainer () {
    
        numberOfDatasets = 0;
        capacity         = MIN_CAPACITY;
        datasetNames     = new std::string [ MIN_CAPACITY ];
        results          = new float* [ MIN_CAPACITY ];

        for ( int i = 0; i < MIN_CAPACITY; i++ ) {
            results[i] = 0;
        }
        
    } // end ResultsContainer

    /** Destructs the ResultsContainer object. */
    ~ResultsContainer () {

    	int i;

        for ( i = 0; i < numberOfDatasets; i++ ) {
            delete [] results[i];
        }

        delete [] datasetNames;
        delete [] results;
        
    } // end ~ResultsContainer

    /**
     * Adds a specific element to the result container.
     * @param datasetName is the name of the data set for which the result is provided.
     * @param elem is a float* with the results.
     */
    void addElement ( std::string datasetName, float* elem ) {

        int i;

        if ( numberOfDatasets >= capacity ) {
            std::string* newDatasetNames = new std::string [ capacity + INC_CAPACITY ];
            float** newResults      = new float* [ capacity + INC_CAPACITY ];
            
            for ( i = 0; i < capacity; i++ ) {
                newDatasetNames[i] = datasetNames[i];
                newResults[i]      = results[i];
            }

            capacity += INC_CAPACITY;

            delete [] datasetNames;
            delete [] results;

            datasetNames = newDatasetNames;
            results      = newResults;
        }

        datasetNames[ numberOfDatasets ] = datasetName;
        results[ numberOfDatasets ] = elem;
        numberOfDatasets ++;
        
    } // end addElement

    /**
     * Returns the result of the consulted data sets.
     * @param position is the position of the data set to be consulted.
     * @return A float* with the results.
     */
    float* getResult ( int position ) {

        if ( position >= 0 && position < capacity ) {
            return results[ position ];
        }

        return 0;

    } // end getResult

    /**
     * Returns the data set name of a specific data set.
     * @param position is the position of the data set to be consulted.
     * @return A string with the data set name.
     */
    std::string getDatasetName ( int position ) {

        if ( position >= 0 && position < capacity ) {
            return datasetNames[ position ];
        }

        return "";

    } // end getDatasetName

    /**
     * Returns a short of the data set name of a specific data set.
     * @param position is the position of the data set to be consulted.
     * @param maxSize is the maximum size for the name.
     * @return A string with the short name of the data set.
     */
    std::string getShortDatasetName ( int position, int maxSize ) {

    	std::string shortName = datasetNames[ position ];
        int i, index;
        const int initialPositions = 4;

        index =  maxSize / 2 ;

        if ( position >= 0 && position < capacity ) {
            if ( datasetNames[ position ].size () >= ( unsigned int ) maxSize ) {
                shortName.erase ( initialPositions, datasetNames[ position ].size () - maxSize + initialPositions - 1 );
                shortName.insert ( initialPositions, ".." );
            }
            else { // Build a name of 30 characters.
                if ( datasetNames[ position ].size () < ( unsigned int ) maxSize ) {
                    for ( i = 0; i < fabs ( maxSize - datasetNames[ position ].size () ) - 1; i++ ) {
                        shortName += " ";
                    }
                }
            }
        }

        return shortName;

    } // end getShortDatasetName

    /**
     * Returns the number of data sets.
     * @return An integer with the number of data sets.
     */
    int getNumberOfDatasets () {
        return numberOfDatasets;
    } // end getNumberOfDatasets

};

#endif
