
/*!

\brief This file contains the implementation of the VDMFunction methods.

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


#include "VDMFunction.h"


VDMFunction::VDMFunction ( float **freqPerClass, int numOfClasses, int numOfValues ) {

    int i, j;

    // We are pointing to the same pointer of frequencies as the Dataset object.
    // So, VDMFunctions must not delete this pointer, since it contains shared memory!!
    frequencyPerClass = freqPerClass;
    numberOfClasses = numOfClasses;
    numberOfValues = numOfValues;

    if ( Utils::doScreenStatistics ) 
        std::cout << "    > Creating a VDM distance function object with frequencies per value " << std::endl;

    for ( i = 0; i < numberOfClasses; i++ ) {
        if ( Utils::doScreenStatistics ) {
            std::cout << "      > Class " << i << ": ";
            for ( j = 0; j < numberOfValues; j++ ) {
                std::cout << " " << frequencyPerClass[i][j];
            }
            std::cout << std::endl;
        }
    }

    if ( Utils::doScreenStatistics ) std::cout << std::endl;

} // end VDMFunction


VDMFunction::~VDMFunction () {
} // end ~VDMFunction


float VDMFunction::computeDistance ( float att1, float att2 ) {

    register int c;
    float dist = 0;

    for ( c = 0; c < numberOfClasses; c++ ) {
        dist += fabs ( frequencyPerClass[c][ ( int ) att1 ] - frequencyPerClass[c][ ( int ) att2 ] );
    }

    return dist;
    
} // end computeDistance
