
#include "Dataset.h"


/*!

\brief This file contains the implementation of the methods that refer to the statistics.

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


void Dataset::makeInitialStatistics () {

    // 1. Reserve memory.
    reserveMemoryForStatistics ();

    // 2. Count frequencies.
    calculateAverages ();

    // 3. Calculate standard deviations.
    calculateDeviations ();

} // end makeStatistics


void Dataset::reserveMemoryForStatistics () {

    int i, j, k;

    if ( Utils::doScreenStatistics ) std::cout << "      > Reserving memory for statistics " << std::endl;

    // Initialize statistics to store minimum and maximum values per class and attribute.
    if ( Utils::doScreenStatistics ) std::cout << "      > Reserving memory for min/max per class " << std::endl;

    minAttValuePerClass = new float* [ numberOfAttributes ];
    maxAttValuePerClass = new float* [ numberOfAttributes ];

    for ( i = 0; i < numberOfAttributes; i++ ) {
        minAttValuePerClass[i] = new float [ numberOfClasses ];
        maxAttValuePerClass[i] = new float [ numberOfClasses ];

        for ( j = 0; j < numberOfClasses; j++ ) {
            minAttValuePerClass[i][j] = FLT_MAX;
            maxAttValuePerClass[i][j] = -FLT_MAX;
        }
    }

    // Initialize statistics to store average and std value of each attribute.
    avgAttValue     = new float [ numberOfAttributes ];
    stdAttValue     = new float [ numberOfAttributes ];
    numberValuedAtt = new int   [ numberOfAttributes ];

    // Initialize statistics to store average and std value of each attribute per class.
    avgNominalValue = new float** [ numberOfAttributes ];
    avgRealValue    = new float*  [ numberOfAttributes ];
    stdRealValue    = new float*  [ numberOfAttributes ];
    avgAttCounter   = new float*  [ numberOfAttributes ];

    if ( avgNominalValue == 0 || avgRealValue == 0 || avgAttCounter == 0 || stdRealValue == 0 ) {
        std::string logMessage = "    > [ERROR STATISTICS] Memory not given. \n ";
        Utils::logWarningError ( logMessage );
        exit ( -1 );
    }

    for ( i = 0; i < numberOfAttributes; i++ ) {
        avgAttValue[i]     = 0;
        stdAttValue[i]     = 0;
        numberValuedAtt[i] = 0;

        avgRealValue[i]    = new float [ numberOfClasses ];
        stdRealValue[i]    = new float [ numberOfClasses ];
        avgAttCounter[i]   = new float [ numberOfClasses ];
        avgNominalValue[i] = 0;

        if ( typeOfRepresentation[i] == 'N' ) {
            avgNominalValue[i] = new float* [ numberOfClasses ];
        }

        for ( j = 0; j < numberOfClasses; j++ ) {
            avgRealValue[i][j]  = 0;
            stdRealValue[i][j]  = 0;
            avgAttCounter[i][j] = 0;

            if ( typeOfRepresentation[i] == 'N' ) {
                int num = ( int ) maxAttValue[i] + 1;
                avgNominalValue[i][j] = new float [ num ];

                for ( k = 0; k < num; k++ ) {
                    avgNominalValue[i][j][k] = 0;
                }
            }
        }
    }
    
} // end reserveMemoryForStatistics


void Dataset::calculateAverages () {

    int i, j, k;

    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( j = 0; j < numberOfAttributes; j++ ) {
            if ( example[i][j] != Dataset::UNKNOWN_VALUE ) {
              
                // Calculate a plain average per attribute for any type of attribute.
                avgAttValue[j] += example[i][j];
                numberValuedAtt[j] ++;

                // To calculate per class, we differentiate between continuous and nominal attributes.
                if ( typeOfRepresentation[j] == 'R' || typeOfRepresentation[j] == 'I' ) {
                    avgRealValue [j][ classOfExample[i] ] += example[i][j];
                    avgAttCounter[j][ classOfExample[i] ] ++;
                }
                else { // The attribute is nominal.
                    avgNominalValue[j][ classOfExample[i] ][ ( int ) example[i][j] ] ++;
                    avgAttCounter[j][ classOfExample[i] ] ++;
                }

                // Update maximum and minimum values per class.
                if ( example[i][j] > maxAttValuePerClass[j][ classOfExample[i] ] ) {
                    maxAttValuePerClass[j][ classOfExample[i] ] = example[i][j];
                }

                if ( example[i][j] < minAttValuePerClass[j][ classOfExample[i] ] ) {
                    minAttValuePerClass[j][ classOfExample[i] ] = example[i][j];
                }
            }
        }
    }

    // 3. Make averages.
    for ( i = 0; i < numberOfAttributes; i++ ) {

        avgAttValue[i] /= ( float ) numberValuedAtt[i];

        if ( typeOfRepresentation[i] == 'R' || typeOfRepresentation[i] == 'I' ) {
            for ( j = 0; j < numberOfClasses; j++ ) {
                if ( avgAttCounter[i][j] > 0 ) avgRealValue[i][j] /= avgAttCounter[i][j];
            }
        }
        else { // For nominal attributes, calculate the median.
            for ( j = 0; j < numberOfClasses; j++ ) {
                // Put the number equivalent to the nominal in avgRealValue if nominal representation is used.
                float max = avgNominalValue[i][j][0];
                int pos   = 0;

                for ( k = 1; k <= maxAttValue[i]; k++ ) {
                    if ( avgNominalValue[i][j][k] > max ) {
                        max = avgNominalValue[i][j][k];
                        pos = k;
                    }
                }

                // Store the median value.
                avgRealValue[i][j] = pos;

                // Write the frequencies of each value.
                for ( k = 0; k <= maxAttValue[i]; k++ ) {
                    if ( avgAttCounter[i][j] > 0 ) avgNominalValue[i][j][k] /= avgAttCounter[i][j];
                }
            }
        }
    }
    
} // end calculateAverages


void Dataset::calculateDeviations () {
    
    int i, j;
    int positionOfMedian;
    float p;

    // 1. Sum for all the examples the difference between the attribute and the average.
    for ( i = 0; i < numberOfExamples; i++ ) {
        for ( j = 0; j < numberOfAttributes; j++ ) {

            if ( example[i][j] != Dataset::UNKNOWN_VALUE ) {
                stdAttValue[j] += pow ( example[i][j] - avgAttValue[j], 2 );
            }

            if ( typeOfRepresentation[j] == 'R' || typeOfRepresentation[j] == 'I' ) {
                if ( example[i][j] != Dataset::UNKNOWN_VALUE ) {
                    stdRealValue[j][ classOfExample[i] ] += pow ( example[i][j] - avgRealValue[j][ classOfExample[i] ] , 2 );
                }
            }
            else { // Nominal representation.
                // Nothing is done here. It is calculated in the next step.
            }
        }
    }

    // 2. Divide by N and apply the root square.
    for ( i = 0; i < numberOfAttributes; i++ ) {

        if ( numberValuedAtt[i] > 0 ) stdAttValue[i] /= numberValuedAtt[i];

        stdAttValue[i] = sqrt ( stdAttValue[i] );

        for ( j = 0; j < numberOfClasses; j++ ) {
            if ( typeOfRepresentation[i] == 'R' || typeOfRepresentation[i] == 'I' ) {
                if ( avgAttCounter[i][j] > 0 ) stdRealValue[i][j] /= avgAttCounter[i][j];

                stdRealValue[i][j] = sqrt ( stdRealValue[i][j] );
            }
            else { // For nominal representation, use the binomial distribution, where s = sqrt( p( 1 - p ) * n ).
                positionOfMedian = ( int ) avgRealValue[i][j];
                p = avgNominalValue[i][j][ positionOfMedian ] ;
                stdRealValue[i][j] = sqrt ( p * ( 1 - p ) * avgAttCounter[i][j] );
            }
        }
    }
    
} // end calculateDeviations
